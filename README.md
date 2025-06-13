# LinkCAD Plugin SDK

This repository contains example plugins for LinkCAD that demonstrate how to create reader and writer plugins for the TLC (LASI) file format.

- [LinkCAD Plugin SDK](#linkcad-plugin-sdk)
  - [Quick Start](#quick-start)
  - [Prerequisites](#prerequisites)
    - [CMake](#cmake)
    - [Visual Studio Build Tools](#visual-studio-build-tools)
    - [Setting up vcpkg](#setting-up-vcpkg)
  - [Building the Examples](#building-the-examples)
  - [Output](#output)
  - [Project Structure](#project-structure)
  - [Installing the Plugins](#installing-the-plugins)

## Quick Start

Run the Powershell script `.\build-examples.ps1` with administrative privileges to build the example plugins for LinkCAD. This script will handle all prerequisites, including installing CMake, Visual Studio Build Tools, and vcpkg if they are not already installed.

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
  - `TlcReaderPlugIn.cpp/h` - Plugin interface implementation
  - `vcpkg.json` - vcpkg manifest for dependencies
  - `CMakeLists.txt` - Build configuration

- `tlcout/` - TLC writer plugin implementation
  - `TlcWriter.cpp/h` - Main writer implementation
  - `TlcWriterPlugIn.cpp/h` - Plugin interface implementation
  - `CMakeLists.txt` - Build configuration

## Installing the Plugins

After building, copy the generated DLL files to your LinkCAD plugins directory to use them with LinkCAD.
