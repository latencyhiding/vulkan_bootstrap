cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
cmake_policy(VERSION 3.7)

set(NAME sprite)

project(${NAME})

# Dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

find_package(OpenGL)
add_subdirectory(thirdparty/single_header)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/physfs)

# Executable

file(GLOB src
    "src/*.h"
    "src/*.cpp"
)

add_executable(${NAME} ${src})
set_target_properties(${NAME} 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/release"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/debug"
)

if (WIN32)
    message(WARNING "This project assumes that all SDL2 modules (image, ttf, mixer etc) are installed in the same directory, which you can indicate through the PATH variable SDL2DIR")
    list(GET SDL2_LIBRARY 0 FIRST_SDL_LIB)

    get_filename_component(
        SDL2_LIB_DIR 
        ${FIRST_SDL_LIB}
        DIRECTORY
    )

    file(GLOB SDL2_DLLS "${SDL2_LIB_DIR}/*.dll")
    
    add_custom_command(
        TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_DLLS}
        $<TARGET_FILE_DIR:${NAME}>
    )
endif()

target_include_directories(${NAME} PUBLIC ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})
target_link_libraries(${NAME} physfs-static glm OpenGL::GL glad ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})