cmake_minimum_required(VERSION 3.16)
project(CS202_Game_Programming_Project)

set(CMAKE_CXX_STANDARD 17)

# Set SFML download URL and paths
set(SFML_URL "https://github.com/SFML/SFML/releases/download/3.0.0/SFML-3.0.0-windows-gcc-14.2.0-mingw-64-bit.zip")
set(SFML_DOWNLOAD_PATH "${CMAKE_BINARY_DIR}/sfml.zip")
set(SFML_EXTRACT_PATH "${CMAKE_BINARY_DIR}/sfml_extracted")
set(SFML_LIB_PATH "${CMAKE_SOURCE_DIR}/lib/SFML")

# Set Google Test download URL and paths
set(GTEST_URL "https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip")
set(GTEST_DOWNLOAD_PATH "${CMAKE_BINARY_DIR}/googletest.zip")
set(GTEST_EXTRACT_PATH "${CMAKE_BINARY_DIR}/gtest_extracted")
set(GTEST_LIB_PATH "${CMAKE_SOURCE_DIR}/lib/gTest")

# Create lib directories if they don't exist
file(MAKE_DIRECTORY ${SFML_LIB_PATH})
file(MAKE_DIRECTORY ${GTEST_LIB_PATH})

# Download SFML if not already downloaded
if(NOT EXISTS ${SFML_DOWNLOAD_PATH})
    message(STATUS "Downloading SFML...")
    file(DOWNLOAD ${SFML_URL} ${SFML_DOWNLOAD_PATH}
         SHOW_PROGRESS
         STATUS download_status)
    
    list(GET download_status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Failed to download SFML")
    endif()
endif()

# Extract SFML if not already extracted
if(NOT EXISTS ${SFML_EXTRACT_PATH})
    message(STATUS "Extracting SFML...")
    file(ARCHIVE_EXTRACT INPUT ${SFML_DOWNLOAD_PATH}
         DESTINATION ${SFML_EXTRACT_PATH})
endif()

# Copy SFML content to lib/SFML/ folder
if(NOT EXISTS "${SFML_LIB_PATH}/bin")
    message(STATUS "Copying SFML to lib/SFML/ folder...")
    file(GLOB SFML_CONTENT "${SFML_EXTRACT_PATH}/SFML-3.0.0/*")
    foreach(item ${SFML_CONTENT})
        get_filename_component(item_name ${item} NAME)
        file(COPY ${item} DESTINATION ${SFML_LIB_PATH})
    endforeach()
endif()

# Download Google Test if not already downloaded (only for Debug builds)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT EXISTS ${GTEST_DOWNLOAD_PATH})
    message(STATUS "Downloading Google Test...")
    file(DOWNLOAD ${GTEST_URL} ${GTEST_DOWNLOAD_PATH}
         SHOW_PROGRESS
         STATUS download_status)
    
    list(GET download_status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Failed to download Google Test")
    endif()
endif()

# Extract Google Test if not already extracted (only for Debug builds)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT EXISTS ${GTEST_EXTRACT_PATH})
    message(STATUS "Extracting Google Test...")
    file(ARCHIVE_EXTRACT INPUT ${GTEST_DOWNLOAD_PATH}
         DESTINATION ${GTEST_EXTRACT_PATH})
endif()

# Copy Google Test content to lib/gTest/ folder (only for Debug builds)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT EXISTS "${GTEST_LIB_PATH}/googletest")
    message(STATUS "Copying Google Test to lib/gTest/ folder...")
    file(GLOB GTEST_CONTENT "${GTEST_EXTRACT_PATH}/googletest-1.14.0/*")
    foreach(item ${GTEST_CONTENT})
        get_filename_component(item_name ${item} NAME)
        file(COPY ${item} DESTINATION ${GTEST_LIB_PATH})
    endforeach()
endif()

# Set SFML_DIR for find_package
set(SFML_DIR "${SFML_LIB_PATH}/lib/cmake/SFML")

# Add project include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add SFML include directories
include_directories(${SFML_LIB_PATH}/include)

# Add Google Test include directories (only for Debug builds)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include_directories(${GTEST_LIB_PATH}/googletest/include)
    include_directories(${GTEST_LIB_PATH}/googlemock/include)
endif()

# Add library search path
link_directories(${SFML_LIB_PATH}/lib)

# Find SFML
find_package(SFML 3.0 COMPONENTS System Window Graphics Audio Network REQUIRED)

# Add Google Test as subdirectory (only for Debug builds)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND EXISTS "${GTEST_LIB_PATH}/CMakeLists.txt")
    add_subdirectory(${GTEST_LIB_PATH} ${CMAKE_BINARY_DIR}/googletest)
    
    # Disable install for Google Test
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
endif()

# Collect all source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

# Add your executable
if(SOURCES)
    add_executable(${PROJECT_NAME} ${SOURCES})
    
    # Add include directories to the target (modern CMake approach)
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${SFML_LIB_PATH}/include
    )
    
    # Add Google Test include directories for Debug builds only
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_include_directories(${PROJECT_NAME} PRIVATE 
            ${GTEST_LIB_PATH}/googletest/include
            ${GTEST_LIB_PATH}/googlemock/include
        )
    endif()
    
    # Link SFML libraries with proper names
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_libraries(${PROJECT_NAME} 
            ${SFML_LIB_PATH}/lib/libsfml-system-d.a
            ${SFML_LIB_PATH}/lib/libsfml-window-d.a
            ${SFML_LIB_PATH}/lib/libsfml-graphics-d.a
            ${SFML_LIB_PATH}/lib/libsfml-audio-d.a
            ${SFML_LIB_PATH}/lib/libsfml-network-d.a
        )
        
        # Link Google Test libraries for Debug builds only
        if(EXISTS "${GTEST_LIB_PATH}/CMakeLists.txt")
            target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock gmock_main)
        endif()
    else()
        target_link_libraries(${PROJECT_NAME} 
            ${SFML_LIB_PATH}/lib/libsfml-system.a
            ${SFML_LIB_PATH}/lib/libsfml-window.a
            ${SFML_LIB_PATH}/lib/libsfml-graphics.a
            ${SFML_LIB_PATH}/lib/libsfml-audio.a
            ${SFML_LIB_PATH}/lib/libsfml-network.a
        )
    endif()
    
    # Copy SFML DLLs to output directory for runtime
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SFML_LIB_PATH}/bin
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
else()
    message(FATAL_ERROR "No source files found in src/ directory.")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release") 
add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    ${CMAKE_SOURCE_DIR}/Release
    COMMENT "Copying assets to build directory"
)
endif()