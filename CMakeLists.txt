cmake_minimum_required(VERSION 3.0)

project(
    "MyProject"
    VERSION 0.0.1
    DESCRIPTION "My project description"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")
file(GLOB_RECURSE CPP_HEADERS "include/*.hpp")

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${CPP_HEADERS})

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)