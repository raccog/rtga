# Version range of CMake required
cmake_minimum_required(VERSION 3.7...3.21)

# Support versions 3.7 to 3.12 by changing cmake_policy version
if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Initialize project
project(rtga VERSION 0.1.3 LANGUAGES C
    DESCRIPTION "A toy library for interfacing with TGA files.")

# Set the default build type to Release
set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    # Possible build types
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Get package version
include(${CMAKE_CURRENT_LIST_DIR}/cmake/package_version.cmake)

# Create version header
configure_file(${CMAKE_CURRENT_LIST_DIR}/include/rtga/rtga_version.h.in ${CMAKE_BINARY_DIR}/include/rtga/rtga_version.h)

# Source file and header file for rtga library
add_library(rtga ${CMAKE_CURRENT_LIST_DIR}/src/rtga.c ${CMAKE_CURRENT_LIST_DIR}/include/rtga/rtga.h ${CMAKE_BINARY_DIR}/include/rtga/rtga_version.h)

# Set the include directories
target_include_directories(rtga PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_BINARY_DIR}/include)

# Set the C standard
set_target_properties(rtga 
    PROPERTIES C_STANDARD 99)

# Compile and link rtga
target_link_libraries(rtga)

# Compile and link tests
add_subdirectory(tests)
