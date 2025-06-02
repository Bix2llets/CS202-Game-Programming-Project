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

# Set output directories based on build type
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# Set SFML path (prebuilt)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/lib/SFML-3.0.0/lib/cmake/SFML")
find_package(SFML REQUIRED COMPONENTS Graphics Window System Audio Network)

# Add GoogleTest as subdirectory (source build) - only for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)   # For Windows
    set(BUILD_GMOCK ON CACHE BOOL "" FORCE)              # Build GMock
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)           # Don't install
    add_subdirectory(${CMAKE_SOURCE_DIR}/lib/googletest-1.17.0 ${CMAKE_BINARY_DIR}/googletest)
endif()

# Collect source files (exclude test files from main executable)
file(GLOB_RECURSE GAME_SOURCES "src/*.cpp")
file(GLOB_RECURSE GAME_HEADERS "src/*.hpp" "include/*.hpp")

# Create main game executable with build type suffix
set(MAIN_EXECUTABLE_NAME "${PROJECT_NAME}_${CMAKE_BUILD_TYPE}")
add_executable(${MAIN_EXECUTABLE_NAME} ${GAME_SOURCES} ${GAME_HEADERS})

# Link SFML libraries to main executable
target_link_libraries(${MAIN_EXECUTABLE_NAME} 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    SFML::Network
)

# Include directories for main executable
target_include_directories(${MAIN_EXECUTABLE_NAME} PRIVATE include)

# Copy assets to build directory
add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMENT "Copying assets to build directory"
)

# Create test executable (only in Debug mode and if test files exist)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    if(TEST_SOURCES)
        set(TEST_EXECUTABLE_NAME "${PROJECT_NAME}_tests_${CMAKE_BUILD_TYPE}")
        add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SOURCES})
        
        # Link libraries to test executable
        target_link_libraries(${TEST_EXECUTABLE_NAME} 
            gtest              # Note: lowercase 'gtest', not 'GTest::gtest'
            gtest_main         # Note: lowercase 'gtest_main'
            SFML::Graphics 
            SFML::Window 
            SFML::System 
            SFML::Audio 
            SFML::Network
        )
        
        # Include directories for tests
        target_include_directories(${TEST_EXECUTABLE_NAME} PRIVATE 
            include 
            src
            ${CMAKE_SOURCE_DIR}/lib/googletest-1.17.0/googletest/include
        )
        
        # Enable testing
        enable_testing()
        include(GoogleTest)
        gtest_discover_tests(${TEST_EXECUTABLE_NAME})
    endif()
endif()

# Platform-specific configurations for Windows
if(WIN32)
    # Set SFML DLL directory
    set(SFML_BIN_DIR "${CMAKE_SOURCE_DIR}/lib/SFML-3.0.0/bin")
    
    # Copy SFML DLLs for executables
    set(TARGETS_TO_COPY ${MAIN_EXECUTABLE_NAME})
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND TARGET ${TEST_EXECUTABLE_NAME})
        list(APPEND TARGETS_TO_COPY ${TEST_EXECUTABLE_NAME})
    endif()
    
    foreach(target ${TARGETS_TO_COPY})
        if(TARGET ${target})
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SFML_BIN_DIR}/sfml-graphics$<$<CONFIG:Debug>:-d>-3.dll"
                "${SFML_BIN_DIR}/sfml-window$<$<CONFIG:Debug>:-d>-3.dll"
                "${SFML_BIN_DIR}/sfml-system$<$<CONFIG:Debug>:-d>-3.dll"
                "${SFML_BIN_DIR}/sfml-audio$<$<CONFIG:Debug>:-d>-3.dll"
                "${SFML_BIN_DIR}/sfml-network$<$<CONFIG:Debug>:-d>-3.dll"
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                COMMENT "Copying SFML DLLs for ${target}"
            )
        endif()
    endforeach()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release") 
add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    ${CMAKE_SOURCE_DIR}/Release
    COMMENT "Copying assets to build directory"
)
endif()