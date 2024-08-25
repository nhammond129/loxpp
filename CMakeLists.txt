cmake_minimum_required(VERSION 3.24)

project(
    "lox"
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")
list(FILTER CPP_SOURCES EXCLUDE REGEX "src/main.cpp")

file(GLOB_RECURSE CPP_HEADERS "include/*.hpp")

add_executable(${PROJECT_NAME}
    src/main.cpp
    ${CPP_SOURCES}
    ${CPP_HEADERS}
)

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_subdirectory(tests)

# make sure that the googletest 'test' target is available
enable_testing()