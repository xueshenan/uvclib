cmake_minimum_required(VERSION 3.14)

option(BUILD_EXAMPLE "Build example" ON)

project(uvclib)

add_subdirectory(base)

add_subdirectory(uvc)

if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
