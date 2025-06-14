cmake_minimum_required(VERSION 3.20)

# Define the plugin name
set(PLUGIN_NAME skeleton)

# Create the plugin library
add_library(${PLUGIN_NAME} SHARED
    SkeletonReader.cpp
    SkeletonReader.h
    SkeletonReaderPlugIn.cpp
)

# Set properties
set_target_properties(${PLUGIN_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME ${PLUGIN_NAME}
)

# Use SDK includes and libraries
use_linkcad_sdk(${PLUGIN_NAME})

# Plugin-specific compile definitions
target_compile_definitions(${PLUGIN_NAME} PRIVATE
    SKELETON_EXPORTS
)

# Install rules
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
