project(glad)

add_library(${PROJECT_NAME} src/glad.c)
add_library(glad::glad ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)