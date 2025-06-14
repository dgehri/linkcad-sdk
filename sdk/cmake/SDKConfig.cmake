# SDK Configuration Module
# This module sets up paths and libraries for the LinkCAD SDK

# Detect platform and architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDK_ARCH_DIR "x64")
else()
    set(SDK_ARCH_DIR "x86")
endif()

# Get SDK root directory (parent of the cmake directory)
get_filename_component(SDK_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

# Set SDK include directory
set(SDK_INCLUDE_DIR "${SDK_ROOT_DIR}/include")

# Verify SDK include directory exists
if(NOT EXISTS "${SDK_INCLUDE_DIR}")
    message(FATAL_ERROR "SDK include directory not found at ${SDK_INCLUDE_DIR}")
endif()

# Set SDK library path based on platform
if(WIN32)
    set(SDK_LIB_DIR "${SDK_ROOT_DIR}/lib/windows-${SDK_ARCH_DIR}")
    set(SDK_LIB_NAME "lcdb4.lib")
    set(SDK_DEBUG_SUBDIR "debug")
    set(SDK_RELEASE_SUBDIR "release")
elseif(APPLE)
    set(SDK_LIB_DIR "${SDK_ROOT_DIR}/lib/macos-${SDK_ARCH_DIR}")
    set(SDK_LIB_NAME "liblcdb4.dylib")
    set(SDK_DEBUG_SUBDIR "")
    set(SDK_RELEASE_SUBDIR "")
elseif(UNIX)
    set(SDK_LIB_DIR "${SDK_ROOT_DIR}/lib/linux-${SDK_ARCH_DIR}")
    set(SDK_LIB_NAME "liblcdb4.so")
    set(SDK_DEBUG_SUBDIR "")
    set(SDK_RELEASE_SUBDIR "")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Verify SDK library directory exists
if(NOT EXISTS "${SDK_LIB_DIR}")
    message(FATAL_ERROR "SDK library directory not found at ${SDK_LIB_DIR}")
endif()

# Find required packages
find_package(Boost REQUIRED)

# Function to configure a LinkCAD plugin target
function(use_linkcad_sdk target)
    # Add include directories
    target_include_directories(${target} PRIVATE
        ${SDK_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
    )
    
    # Link SDK libraries
    if(MSVC)
        target_link_libraries(${target} PRIVATE
            ${SDK_LIB_DIR}/$<IF:$<CONFIG:Debug>,${SDK_DEBUG_SUBDIR},${SDK_RELEASE_SUBDIR}>/${SDK_LIB_NAME}
            ${Boost_LIBRARIES}
        )
    else()
        target_link_libraries(${target} PRIVATE
            ${SDK_LIB_DIR}/${SDK_LIB_NAME}
            ${Boost_LIBRARIES}
        )
    endif()
    
    # Platform-specific settings
    if(MSVC)
        # Add MSVC specific compile options
        target_compile_options(${target} PRIVATE
            /W4 # Warning level 4
            $<$<CONFIG:Debug>:/Od /RTC1> # Debug optimizations
            $<$<CONFIG:Release>:/O2> # Release optimizations
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Add GCC/Clang specific compile options
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic
            $<$<CONFIG:Debug>:-g -O0>
            $<$<CONFIG:Release>:-O3>
        )
    endif()

    # Common compile definitions
    target_compile_definitions(${target} PRIVATE
        $<$<CONFIG:Debug>:DEBUG _DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
    )

    # Windows specific settings
    if(WIN32)
        target_compile_definitions(${target} PRIVATE
            _WINDOWS
            _USRDLL
            NOMINMAX
            _CRT_SECURE_NO_WARNINGS
        )

        # Set Windows-specific properties
        set_target_properties(${target} PROPERTIES
            PREFIX ""
            WINDOWS_EXPORT_ALL_SYMBOLS OFF
        )
    endif()

    # Set RPATH for Linux/macOS
    if(UNIX AND NOT APPLE)
        set_target_properties(${target} PROPERTIES
            INSTALL_RPATH "$ORIGIN"
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    elseif(APPLE)
        set_target_properties(${target} PROPERTIES
            INSTALL_RPATH "@loader_path"
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    endif()
endfunction()

# Export variables for backward compatibility
set(SDK_LIBRARY_DIR "${SDK_LIB_DIR}" CACHE INTERNAL "SDK library directory")

# Provide informational message
message(STATUS "LinkCAD SDK found:")
message(STATUS "  Include: ${SDK_INCLUDE_DIR}")
message(STATUS "  Library: ${SDK_LIB_DIR}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Architecture: ${SDK_ARCH_DIR}")
message(STATUS "  Boost: ${Boost_INCLUDE_DIRS}")
