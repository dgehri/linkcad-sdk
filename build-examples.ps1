#Requires -Version 5.1
#Requires -RunAsAdministrator

<#
.SYNOPSIS
    Builds the LinkCAD Plugin SDK examples
.DESCRIPTION
    This script ensures all required build tools are installed and builds the example plugins in Release mode.
    It will install missing prerequisites using winget if they are not found.
.EXAMPLE
    .\build-examples.ps1
#>

[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'

Write-Host "LinkCAD Plugin SDK Build Script" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host

# Function to check if a command exists
function Test-Command {
    param([string]$Command)
    $null -ne (Get-Command $Command -ErrorAction SilentlyContinue)
}

# Function to check if winget is available
function Test-Winget {
    if (-not (Test-Command "winget")) {
        Write-Error "winget is not available. Please install App Installer from the Microsoft Store or ensure you're running Windows 10 1709 or higher."
        exit 1
    }
}

# Function to install a package using winget
function Install-WithWinget {
    param(
        [string]$PackageId,
        [string]$DisplayName
    )
    
    Write-Host "Installing $DisplayName..." -ForegroundColor Yellow
    $result = winget install --id=$PackageId -e --source winget --accept-package-agreements --accept-source-agreements
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Failed to install $DisplayName"
        exit 1
    }
    Write-Host "$DisplayName installed successfully." -ForegroundColor Green
}

# Check and install CMake
Write-Host "Checking for CMake..." -ForegroundColor Cyan
if (-not (Test-Command "cmake")) {
    Test-Winget
    Install-WithWinget "Kitware.CMake" "CMake"
    
    # Refresh PATH
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path", "User")
    
    if (-not (Test-Command "cmake")) {
        Write-Error "CMake was installed but is not available in PATH. Please restart your terminal and run this script again."
        exit 1
    }
} else {
    $cmakeVersion = cmake --version | Select-Object -First 1
    Write-Host "CMake found: $cmakeVersion" -ForegroundColor Green
}

# Check and install Visual Studio Build Tools
Write-Host "`nChecking for Visual Studio Build Tools..." -ForegroundColor Cyan
$vswhereExe = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$hasVSBuildTools = $false

if (Test-Path $vswhereExe) {
    $installations = & $vswhereExe -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json | ConvertFrom-Json
    if ($installations.Count -gt 0) {
        $hasVSBuildTools = $true
        Write-Host "Visual Studio Build Tools found: $($installations[0].displayName)" -ForegroundColor Green
    }
}

if (-not $hasVSBuildTools) {
    Test-Winget
    Install-WithWinget "Microsoft.VisualStudio.2022.BuildTools" "Visual Studio 2022 Build Tools"
    
    Write-Host "`nVisual Studio Build Tools have been installed." -ForegroundColor Yellow
    Write-Host "You may need to restart your terminal for the changes to take effect." -ForegroundColor Yellow
}

# Check for vcpkg
Write-Host "`nChecking for vcpkg..." -ForegroundColor Cyan
$vcpkgFound = $false
$vcpkgRoot = $null

# Check environment variable first
if ($env:VCPKG_ROOT -and (Test-Path "$env:VCPKG_ROOT\vcpkg.exe")) {
    $vcpkgFound = $true
    $vcpkgRoot = $env:VCPKG_ROOT
    Write-Host "vcpkg found at: $vcpkgRoot (via VCPKG_ROOT)" -ForegroundColor Green
} elseif (Test-Command "vcpkg") {
    $vcpkgFound = $true
    Write-Host "vcpkg found in PATH" -ForegroundColor Green
} else {
    # Try common locations
    $commonPaths = @(
        "$env:USERPROFILE\vcpkg",
        "$env:USERPROFILE\source\repos\vcpkg",
        "C:\vcpkg",
        "C:\tools\vcpkg"
    )
    
    foreach ($path in $commonPaths) {
        if (Test-Path "$path\vcpkg.exe") {
            $vcpkgFound = $true
            $vcpkgRoot = $path
            Write-Host "vcpkg found at: $vcpkgRoot" -ForegroundColor Green
            break
        }
    }
}

if (-not $vcpkgFound) {
    Write-Host "vcpkg not found. Installing vcpkg..." -ForegroundColor Yellow
    
    $vcpkgInstallPath = "$env:USERPROFILE\vcpkg"
    
    if (Test-Path $vcpkgInstallPath) {
        Write-Host "Found existing vcpkg directory at $vcpkgInstallPath. Using it." -ForegroundColor Yellow
    } else {
        Write-Host "Cloning vcpkg repository..." -ForegroundColor Yellow
        git clone https://github.com/Microsoft/vcpkg.git $vcpkgInstallPath
        if ($LASTEXITCODE -ne 0) {
            Write-Error "Failed to clone vcpkg repository"
            exit 1
        }
    }
    
    Push-Location $vcpkgInstallPath
    try {
        Write-Host "Bootstrapping vcpkg..." -ForegroundColor Yellow
        .\bootstrap-vcpkg.bat -disableMetrics
        if ($LASTEXITCODE -ne 0) {
            Write-Error "Failed to bootstrap vcpkg"
            exit 1
        }
        
        $vcpkgRoot = $vcpkgInstallPath
        Write-Host "vcpkg installed successfully at: $vcpkgRoot" -ForegroundColor Green
        
        # Set VCPKG_ROOT for current session
        $env:VCPKG_ROOT = $vcpkgRoot
        
        Write-Host "`nTo make vcpkg permanently available, run one of the following:" -ForegroundColor Yellow
        Write-Host "  1. Set VCPKG_ROOT environment variable: [Environment]::SetEnvironmentVariable('VCPKG_ROOT', '$vcpkgRoot', 'User')" -ForegroundColor White
        Write-Host "  2. Or integrate vcpkg globally: $vcpkgRoot\vcpkg.exe integrate install" -ForegroundColor White
    } finally {
        Pop-Location
    }
}

# Build the examples
Write-Host "`nBuilding examples in Release mode..." -ForegroundColor Cyan
Write-Host "====================================" -ForegroundColor Cyan

$examplesDir = Join-Path $PSScriptRoot "examples"
if (-not (Test-Path $examplesDir)) {
    Write-Error "Examples directory not found at: $examplesDir"
    exit 1
}

Push-Location $examplesDir
try {
    # Configure
    Write-Host "`nConfiguring build..." -ForegroundColor Yellow
    if ($vcpkgRoot) {
        cmake --preset=x64-release "-DCMAKE_TOOLCHAIN_FILE=$vcpkgRoot/scripts/buildsystems/vcpkg.cmake"
    } else {
        cmake --preset=x64-release
    }
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed"
        exit 1
    }
    
    # Build
    Write-Host "`nBuilding..." -ForegroundColor Yellow
    cmake --build --preset=x64-release
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed"
        exit 1
    }
    
    Write-Host "`nBuild completed successfully!" -ForegroundColor Green
    
    # Check output
    $binDir = Join-Path $examplesDir "bin"
    if (Test-Path $binDir) {
        Write-Host "`nGenerated plugins:" -ForegroundColor Cyan
        Get-ChildItem -Path $binDir -Filter "*.dll" | ForEach-Object {
            Write-Host "  - $($_.Name)" -ForegroundColor White
        }
        Write-Host "`nPlugins are located at: $binDir" -ForegroundColor Green
    }
    
} finally {
    Pop-Location
}

Write-Host "`nDone!" -ForegroundColor Green
