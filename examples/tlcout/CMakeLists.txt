cmake_minimum_required(VERSION 3.20)

# Set policy for new CMake versions BEFORE project()
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

set(PLUGIN_NAME tlcout)

project(${PLUGIN_NAME})

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the DLL target and its source files
add_library(${PLUGIN_NAME} SHARED
    TlcWriter.cpp
    TlcWriter.h
    TlcWriterPlugIn.cpp
    TlcWriterPlugIn.h
)

# Include directories
target_include_directories(${PLUGIN_NAME} PRIVATE
    ${SDK_INCLUDE_DIR}
)

# If Boost was found, add its include directories
if(Boost_FOUND)
    target_include_directories(${PLUGIN_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
endif()

# Link SDK libraries
target_link_sdk_libraries(${PLUGIN_NAME})

# Platform-specific settings
if(MSVC)
    # Add MSVC specific compile options
    target_compile_options(${PLUGIN_NAME} PRIVATE
        /W4  # Warning level 4
        $<$<CONFIG:Debug>:/Od /RTC1>  # Debug optimizations
        $<$<CONFIG:Release>:/O2>       # Release optimizations
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add GCC/Clang specific compile options
    target_compile_options(${PLUGIN_NAME} PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3>
    )
endif()

# Common compile definitions
target_compile_definitions(${PLUGIN_NAME} PRIVATE
    TLCOUT_EXPORTS
    $<$<CONFIG:Debug>:DEBUG _DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# Windows specific settings
if(WIN32)
    target_compile_definitions(${PLUGIN_NAME} PRIVATE
        _WINDOWS
        _USRDLL
        NOMINMAX
        _CRT_SECURE_NO_WARNINGS
    )

    # Set output name and properties
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        OUTPUT_NAME "tlcout"
        PREFIX ""
        WINDOWS_EXPORT_ALL_SYMBOLS OFF
    )
    
    # Generate export header if needed
    include(GenerateExportHeader)
    generate_export_header(${PLUGIN_NAME}
        BASE_NAME TLCOUT
        EXPORT_MACRO_NAME TLCOUT_API
    )
endif()

# Set RPATH for Linux/macOS
if(UNIX AND NOT APPLE)
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
elseif(APPLE)
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        INSTALL_RPATH "@loader_path"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

# Install target
install(TARGETS ${PLUGIN_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install PDB files for Windows Debug builds
if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    install(FILES $<TARGET_PDB_FILE:${PLUGIN_NAME}>
        DESTINATION bin
        OPTIONAL
    )
endif()
