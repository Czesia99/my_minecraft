cmake_minimum_required(VERSION 3.22.1)

project(mygl)

set(CMAKE_CXX_STANDARD 20)

set(MYGL_SRC
    glad.c
    Camera3D.cpp
    Context.cpp
    Scene.cpp
    Shape.cpp
    Skybox.cpp
    Texture.cpp
    stb_image.cpp
)

add_library(mygl STATIC ${MYGL_SRC})

target_include_directories(mygl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)