cmake_minimum_required(VERSION 3.10)
project(Lab346)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Threads REQUIRED)

macro(add_project DIR)
file(
    GLOB SRCS
    "${DIR}/*.cpp"
    "${DIR}/*.c"

    "${DIR}/*.h"
    "${DIR}/*.hpp"
)
include_directories("${DIR}/")
include_directories("${DIR}/include")
add_executable(
    "${DIR}"

    ${SRCS}
)

target_link_libraries("${DIR}" PRIVATE Threads::Threads)
endmacro()

add_subdirectory(projects)