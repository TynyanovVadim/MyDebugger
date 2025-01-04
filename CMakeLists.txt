cmake_minimum_required(VERSION 3.30)
project(debugger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wextra -Wall")

set(LINENOISE_LIB linenoise.c)
set(LINENOISE_PATH "linenoise/linenoise.c")

if((NOT LINENOISE_PATH) OR (NOT EXISTS ${LINENOISE_PATH}) OR (NOT EXISTS ${LINENOISE_PATH}))
    # we couldn't find files for linenoise or they don't exist
    message("Unable to find " ${LINENOISE_PATH})

    # we have a submodule setup for linenoise, assume it is under
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- linenoise
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

aux_source_directory(src SRC_LIST)

include_directories("include/")
include_directories("linenoise")

add_executable(${PROJECT_NAME} ${SRC_LIST} ${LINENOISE_PATH})