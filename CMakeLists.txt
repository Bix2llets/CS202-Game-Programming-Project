cmake_minimum_required(VERSION 3.16)
project(CS202_Game_Programming_Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ENV{CMAKE_TLS_VERIFY} "0")

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

# Set nlohmann/json download URL and paths
set(NLOHMANN_JSON_URL "https://github.com/nlohmann/json/releases/download/v3.12.0/json.hpp")
set(NLOHMANN_JSON_PATH "${CMAKE_SOURCE_DIR}/lib/json/json.hpp")
set(NLOHMANN_JSON_DIR "${CMAKE_SOURCE_DIR}/lib/json")

# Create lib/json directory if it doesn't exist

# Download json.hpp if not already present
file(MAKE_DIRECTORY ${NLOHMANN_JSON_DIR})
# Create lib directories if they don't exist
file(MAKE_DIRECTORY ${SFML_LIB_PATH})
file(MAKE_DIRECTORY ${GTEST_LIB_PATH})

if(NOT EXISTS ${NLOHMANN_JSON_PATH})
    message(STATUS "Downloading nlohmann/json.hpp...")
    file(DOWNLOAD ${NLOHMANN_JSON_URL} ${NLOHMANN_JSON_PATH}
        SHOW_PROGRESS
        STATUS download_status)
    list(GET download_status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Failed to download nlohmann/json.hpp")
    endif()
endif()
# Download SFML if not already present in lib/SFML/bin
if(NOT EXISTS "${SFML_LIB_PATH}/bin")
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
    message(STATUS "Copying SFML to lib/SFML/ folder...")
    file(GLOB SFML_CONTENT "${SFML_EXTRACT_PATH}/SFML-3.0.0/*")
    foreach(item ${SFML_CONTENT})
        get_filename_component(item_name ${item} NAME)
        file(COPY ${item} DESTINATION ${SFML_LIB_PATH})
    endforeach()
endif()

# Download Google Test if not already present in lib/gTest/googletest (only for Debug builds)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT EXISTS "${GTEST_LIB_PATH}/googletest")
    if(NOT EXISTS ${GTEST_DOWNLOAD_PATH})
        message(STATUS "Downloading Google Test...")
        file(DOWNLOAD ${GTEST_URL} ${GTEST_DOWNLOAD_PATH}
            SHOW_PROGRESS
            STATUS download_status)
        list(GET download_status 0 status_code)
        if(NOT status_code EQUAL 0)
            message(FATAL_ERROR "Failed to download Google Test")
        endif()
    endif()
    # Extract Google Test if not already extracted
    if(NOT EXISTS ${GTEST_EXTRACT_PATH})
        message(STATUS "Extracting Google Test...")
        file(ARCHIVE_EXTRACT INPUT ${GTEST_DOWNLOAD_PATH}
            DESTINATION ${GTEST_EXTRACT_PATH})
    endif()
    # Copy Google Test content to lib/gTest/ folder
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
include_directories(${CMAKE_SOURCE_DIR}/tests/mockClasses)

# Add SFML include directories
include_directories(${SFML_LIB_PATH}/include)

# Add JSON include directoris
include_directories(${NLOHMANN_JSON_DIR})

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

# Collect all source files for the main program
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

# Create a static library from the main program sources
add_library(CS202GameLib STATIC ${SOURCES})

# Set output directory for executables (per target, not global)
# Remove or comment out the global setting:
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Set a single output directory for all executables and tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Add your executable (main program)
if(SOURCES)
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${SFML_LIB_PATH}/include
    )
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_include_directories(${PROJECT_NAME} PRIVATE
            ${GTEST_LIB_PATH}/googletest/include
            ${GTEST_LIB_PATH}/googlemock/include
        )
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_libraries(${PROJECT_NAME}
            ${SFML_LIB_PATH}/lib/libsfml-system-d.a
            ${SFML_LIB_PATH}/lib/libsfml-window-d.a
            ${SFML_LIB_PATH}/lib/libsfml-graphics-d.a
            ${SFML_LIB_PATH}/lib/libsfml-audio-d.a
            ${SFML_LIB_PATH}/lib/libsfml-network-d.a
        )
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
else()
    message(FATAL_ERROR "No source files found in src/ directory.")
endif()

# Ensure bin directory exists before copying DLLs and executables
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Ensuring bin directory exists before copying files"
)

# Copy SFML DLLs to bin only once after building the main target
if(EXISTS "${SFML_LIB_PATH}/bin")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SFML_LIB_PATH}/bin
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Copying SFML DLLs to output directory"
    )
else()
    message(WARNING "SFML bin directory does not exist: ${SFML_LIB_PATH}/bin. Skipping DLL copy.")
endif()


# Always copy assets and content after every build, before running
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/content ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/content
    COMMENT "Always copying assets and content to output directory"
)
add_dependencies(${PROJECT_NAME} copy_resources)
# # Copy assets to output directory after building
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/mod
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mod
#     COMMENT "Copying mod to output directory"
# )

# Copy assets to Release directory for Release buildsz
# if (CMAKE_BUILD_TYPE STREQUAL "Release") 
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_SOURCE_DIR}/assets
#         ${CMAKE_SOURCE_DIR}/Release/assets
#         COMMENT "Copying assets to Release directory"
#     )
# endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Enable testing and add test discovery
    include(GoogleTest)
    enable_testing()

    # Add test executables for all test files in tests/ recursively
    file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cc")

    foreach(test_src ${TEST_SOURCES})
        get_filename_component(test_name ${test_src} NAME_WE)
        add_executable(${test_name} ${test_src})
        target_link_libraries(${test_name} PRIVATE gtest gtest_main CS202GameLib
            ${SFML_LIB_PATH}/lib/libsfml-system-d.a
            ${SFML_LIB_PATH}/lib/libsfml-window-d.a
            ${SFML_LIB_PATH}/lib/libsfml-graphics-d.a
            ${SFML_LIB_PATH}/lib/libsfml-audio-d.a
            ${SFML_LIB_PATH}/lib/libsfml-network-d.a
        )
        target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
        gtest_discover_tests(${test_name} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach()

    # Ensure all test executables depend on the main target so DLLs and assets are copied before tests run
    foreach(test_src ${TEST_SOURCES})
        get_filename_component(test_name ${test_src} NAME_WE)
        add_dependencies(${test_name} ${PROJECT_NAME})
    endforeach()

    # Copy all executables (main and tests) to bin after build
    foreach(test_src ${TEST_SOURCES})
        get_filename_component(test_name ${test_src} NAME_WE)
        add_custom_command(TARGET ${test_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${test_name}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
            COMMENT "Copying test executable ${test_name} to bin directory"
        )
    endforeach()

endif()

# Copy all DLL files from SFML and lib directories to bin after building the main target
if(EXISTS "${SFML_LIB_PATH}/bin")
    file(GLOB SFML_DLLS
        "${SFML_LIB_PATH}/bin/*.dll"
        "${SFML_LIB_PATH}/bin/*.DLL"
    )
    foreach(dll ${SFML_DLLS})
        get_filename_component(dll_name "${dll}" NAME)
        set(target_path "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${dll_name}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${dll}" "${target_path}"
            COMMENT "Copying ${dll} to bin directory (overwrite if exists)"
        )
    endforeach()
endif()

# Delete zip files after extracting
if(EXISTS ${SFML_DOWNLOAD_PATH})
    file(REMOVE ${SFML_DOWNLOAD_PATH})
endif()
if(EXISTS ${GTEST_DOWNLOAD_PATH})
    file(REMOVE ${GTEST_DOWNLOAD_PATH})
endif()

# Delete extracted SFML and Google Test folders after extraction and copying
if(EXISTS ${SFML_EXTRACT_PATH})
    file(REMOVE_RECURSE ${SFML_EXTRACT_PATH})
endif()
if(EXISTS ${GTEST_EXTRACT_PATH})
    file(REMOVE_RECURSE ${GTEST_EXTRACT_PATH})
endif()
