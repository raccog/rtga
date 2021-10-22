cmake_minimum_required(VERSION 3.7...3.21)

# Use git to retrieve version
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    OUTPUT_VARIABLE RTGA_VERSION
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    get_target_property(RTGA_VERSION rtga VERSION)
endif()

# Split version
string(REPLACE "." ";" RTGA_VERSION_LIST ${RTGA_VERSION})
list(GET RTGA_VERSION_LIST 0 RTGA_VERSION_MAJOR)
list(GET RTGA_VERSION_LIST 1 RTGA_VERSION_MINOR)
list(GET RTGA_VERSION_LIST 2 RTGA_VERSION_PATCH)
