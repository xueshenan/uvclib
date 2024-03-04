project(uvc)

message(STATUS "Begin build project ${PROJECT_NAME}")

aux_source_directory("." UVC_SRC)

add_library(${PROJECT_NAME} STATIC ${UVC_SRC})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    base
)
