cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
cmake_policy(VERSION 3.7)

set(NAME vulkan_project)

project(${NAME})

# Dependencies

find_package(Vulkan)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/single_header)

# Executable

file(GLOB src
    "src/*.h"
    "src/*.cpp"
)

add_executable(${NAME} ${src})
set_target_properties(${NAME} 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)
target_link_libraries(${NAME} glfw Vulkan::Vulkan)