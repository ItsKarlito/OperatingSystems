cmake_minimum_required(VERSION 3.10)
project(Lab346)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

macro(add_project DIR)
file(
    GLOB SRCS
    "${DIR}/*.cpp"
    "${DIR}/*.c"

    "${DIR}/*.h"
    "${DIR}/*.hpp"
)

add_executable(
    "${DIR}"

    ${SRCS}
)

# target_include_directories(${DIR}

#     PRIVATE "${DIR}"
# )
endmacro()

add_subdirectory(projects)