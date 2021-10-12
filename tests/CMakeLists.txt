# Version range of CMake required
cmake_minimum_required(VERSION 3.12...3.21)

# Add test executable
add_executable(rtga_test rtga_test.c)

# Set the C standard
set_target_properties(rtga_test
    PROPERTIES C_STANDARD 99)

# Compile and link rtga_test
target_link_libraries(rtga_test PUBLIC rtga)
