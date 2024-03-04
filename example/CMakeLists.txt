project(example)

message(STATUS "Begin build project ${PROJECT_NAME}")

aux_source_directory("." EXAMPLE_SRC)

add_executable(${PROJECT_NAME} ${EXAMPLE_SRC})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    base
    uvc
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBUDEV libudev REQUIRED IMPORTED_TARGET)

pkg_check_modules(LIBV4L2 libv4l2 REQUIRED IMPORTED_TARGET)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    PkgConfig::LIBUDEV
    PkgConfig::LIBV4L2
)