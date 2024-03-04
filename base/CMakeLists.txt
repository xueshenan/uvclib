
project(base)

aux_source_directory("." SRC_BASE)

add_library(${PROJECT_NAME} STATIC ${SRC_BASE})