cmake_minimum_required(VERSION 3.30)
project(debugger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall")

set(LINENOISE_LIB linenoise.c)
set(LINENOISE_PATH "linenoise/linenoise.c")

# install submodules
execute_process(COMMAND git submodule update --init
               WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(src SRC_LIST)

include_directories(include linenoise libelfin)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${LINENOISE_PATH})

add_custom_target(
   libelfin
   COMMAND make
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/libelfin
)
target_link_libraries(debugger
                      ${PROJECT_SOURCE_DIR}/libelfin/dwarf/libdwarf++.so
                      ${PROJECT_SOURCE_DIR}/libelfin/elf/libelf++.so)
add_dependencies(debugger libelfin)