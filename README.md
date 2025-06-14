# LinkCAD Plugin SDK

This repository contains example plugins for LinkCAD that demonstrate how to create reader and writer plugins for the TLC (LASI) file format.

- [LinkCAD Plugin SDK](#linkcad-plugin-sdk)
  - [Getting Started with Git and GitHub](#getting-started-with-git-and-github)
    - [What is Git?](#what-is-git)
    - [Setting up Git and GitHub](#setting-up-git-and-github)
    - [Working with Your Fork](#working-with-your-fork)
  - [Quick Start](#quick-start)
  - [Prerequisites](#prerequisites)
    - [CMake](#cmake)
    - [Visual Studio Build Tools](#visual-studio-build-tools)
    - [Setting up vcpkg](#setting-up-vcpkg)
  - [Building the Examples](#building-the-examples)
  - [Output](#output)
  - [Project Structure](#project-structure)
  - [Installing the Plugins](#installing-the-plugins)

## Getting Started with Git and GitHub

### What is Git?

Git is a version control system that helps you track changes to your code. GitHub is a website that hosts Git repositories (projects) online. You'll need both to work with this SDK.

### Setting up Git and GitHub

1. **Create a GitHub account**
   - Go to [https://github.com](https://github.com)
   - Click "Sign up" and create your account

2. **Install Git on Windows**

   ```powershell
   winget install --id=Git.Git -e --source winget
   ```

3. **Fork this repository**
   - Go to the SDK repository on GitHub using the link you received
   - Click the "Fork" button in the top-right corner
   - This creates your own copy of the SDK on GitHub under your account

4. **Clone your fork to your computer**

   ```powershell
   # Replace YOUR-USERNAME with your GitHub username
   git clone https://github.com/YOUR-USERNAME/linkcad-plugin-sdk.git
   cd linkcad-plugin-sdk
   ```

5. **Set up the original repository as upstream**

   ```powershell
   # This allows you to get updates from the original SDK
   git remote add upstream https://github.com/ORIGINAL-OWNER/linkcad-plugin-sdk.git
   ```

### Working with Your Fork

When developing your own plugin:

1. **Create a new branch for your plugin**

   ```powershell
   git checkout -b my-plugin-name
   ```

2. **Make your changes and commit them**

   ```powershell
   git add .
   git commit -m "Add my custom plugin"
   ```

3. **Push your changes to your fork**

   ```powershell
   git push origin my-plugin-name
   ```

4. **Get updates from the original SDK**

   ```powershell
   git fetch upstream
   git checkout main
   git merge upstream/main
   git push origin main
   ```

## Quick Start

Run the PowerShell script `.\build-examples.ps1` with administrative privileges to build the example plugins for LinkCAD. This script will handle all prerequisites, including installing CMake, Visual Studio Build Tools, and vcpkg if they are not already installed.

```powershell
.\build-examples.ps1
```

## Prerequisites

- Windows 10 1709 (build 16299) or higher
- CMake 3.20 or higher
- Visual Studio BuildTools 2022
- vcpkg package manager

### CMake

```powershell
winget install --id=Kitware.CMake -e --source winget
```

### Visual Studio Build Tools

```powershell
winget install --id=Microsoft.VisualStudio.2022.BuildTools -e --source winget
```

### Setting up vcpkg

1. Clone and bootstrap vcpkg:

   ```powershell
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   ```

2. Make vcpkg available to CMake by either:
   - Setting the environment variable `VCPKG_ROOT` to your vcpkg installation directory
   - Or integrating vcpkg globally: `.\vcpkg integrate install`

## Building the Examples

1. Navigate to the examples directory:

   ```powershell
   cd examples
   ```

2. Configure and build using presets:

   For Debug build:

   ```powershell
   cmake --preset=x64-debug
   cmake --build --preset=x64-debug
   ```

   For Release build:

   ```powershell
   cmake --preset=x64-release
   cmake --build --preset=x64-release
   ```

## Output

The build process will create two Windows DLL files in the `bin` directory:

- `tlcin.dll` - TLC file reader plugin
- `tlcout.dll` - TLC file writer plugin

## Project Structure

- `tlcin/` - TLC reader plugin implementation
  - `TlcReader.cpp/h` - Main reader implementation
  - `TlcReaderPlugIn.cpp` - Plugin definition
  - `CMakeLists.txt` - Build configuration

- `tlcout/` - TLC writer plugin implementation
  - `TlcWriter.cpp/h` - Main writer implementation
  - `TlcWriterPlugIn.cpp` - Plugin definition
  - `CMakeLists.txt` - Build configuration

## Installing the Plugins

After building, copy the generated DLL files to your LinkCAD plugins directory to use them with LinkCAD.

> [!WARNING]
> The plugins will not be accepted as is by LinkCAD because they first need to be digitally signed. Contact the LinkCAD team for signing instructions.
