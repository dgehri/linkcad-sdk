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

# Function to link SDK libraries to a target
function(target_link_sdk_libraries target)
    if(MSVC)
        target_link_libraries(${target} PRIVATE
            ${SDK_LIB_DIR}/$<IF:$<CONFIG:Debug>,${SDK_DEBUG_SUBDIR},${SDK_RELEASE_SUBDIR}>/${SDK_LIB_NAME}
        )
    else()
        target_link_libraries(${target} PRIVATE
            ${SDK_LIB_DIR}/${SDK_LIB_NAME}
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
