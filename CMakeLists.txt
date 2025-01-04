cmake_minimum_required(VERSION 3.30)
project(debugger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wextra -Wall")

set(LINENOISE_LIBRARY linenoise/linenoise.c)

aux_source_directory(src SRC_LIST)

include_directories("include/")
include_directories("linenoise")

add_executable(${PROJECT_NAME} ${SRC_LIST} ${LINENOISE_LIBRARY})
 