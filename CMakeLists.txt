# Version range of CMake required
cmake_minimum_required(VERSION 3.12...3.21)

# Initialize project
project(rtga VERSION 1.0 LANGUAGES C)

# Source file and header file for rtga library
add_library(rtga src/rtga.c include/rtga/rtga.h)

# Set the include directory
target_include_directories(rtga PUBLIC include)

# Set the C standard
set_target_properties(rtga 
    PROPERTIES C_STANDARD 99)

# Compile and link rtga
target_link_libraries(rtga)

# Compile and link tests
add_subdirectory(tests)
