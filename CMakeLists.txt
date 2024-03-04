cmake_minimum_required(VERSION 3.14)

option(BUILD_EXAMPLE "Build example" ON)

project(uvclib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(base)

add_subdirectory(uvc)

if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
