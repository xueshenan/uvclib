
project(base)

aux_source_directory("." BASE_SRC)

add_library(${PROJECT_NAME} STATIC ${BASE_SRC})