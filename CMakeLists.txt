cmake_minimum_required(VERSION 3.16)
project(CS202GameProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Configure Debug and Release profiles
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set SFML path
set(SFML_DIR "${CMAKE_SOURCE_DIR}/lib/sfml-3.0.0/lib/cmake/SFML")
# Find SFML
find_package(SFML REQUIRED COMPONENTS Graphics Window System Audio Network)

# Collect source files (adjust paths as needed)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp" "include/*.hpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    SFML::Network
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMENT "Copying assets to build directory"
)

# Platform-specific configurations
if(WIN32)
    # Copy SFML DLLs to build directory on Windows
    set(SFML_BIN_DIR "${CMAKE_SOURCE_DIR}/lib/SFML-3.0.0/bin")
    
    # Copy Release DLLs
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Debug>,copy_if_different,copy_if_different>
        "${SFML_BIN_DIR}/sfml-graphics$<$<CONFIG:Debug>:-d>-3.dll"
        "${SFML_BIN_DIR}/sfml-window$<$<CONFIG:Debug>:-d>-3.dll"
        "${SFML_BIN_DIR}/sfml-system$<$<CONFIG:Debug>:-d>-3.dll"
        "${SFML_BIN_DIR}/sfml-audio$<$<CONFIG:Debug>:-d>-3.dll"
        "${SFML_BIN_DIR}/sfml-network$<$<CONFIG:Debug>:-d>-3.dll"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Copying SFML DLLs to build directory"
    )
endif()
