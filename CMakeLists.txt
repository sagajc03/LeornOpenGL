cmake_minimum_required(VERSION 3.10)
project(LeornOpenGL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app
    src/main.cpp
    src/vendors/glad.c
    src/vendors/stb_image/dtb_image.cpp
)

target_include_directories(app PRIVATE
    includes
    src/vendors
)

find_package(glfw3 REQUIRED)

target_link_libraries(app
    glfw
    GL
)

