cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_MINECRAFT "my_minecraft")

project(${PROJECT_MINECRAFT} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(SRC ./src)


set(ASIO_INCLUDE_DIR ${INCLUDE_DIR}/asio-1.30.2/include)

add_subdirectory(include/glfw)
add_subdirectory(include/MYGL)

set(IMGUI_DIR "./include/imgui")
set(IMGUI_DIR_BACK "./include/imgui/backends")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set(SRC_FILES
    ${SRC}/main.cpp
    ${SRC}/GameScene.cpp
    ${SRC}/ChunkVertices.cpp
    ${SRC}/ChunkMesh.cpp
    ${SRC}/Renderer.cpp
    ${SRC}/Client.cpp
    ${SRC}/Entity.cpp
    ${SRC}/World.cpp
    ${SRC}/Shadowmap.cpp
    ${IMGUI_SOURCES}
)

set(LINK_LIB
    glfw
    OpenGL::GL
    mygl
)
# set(CMAKE_BUILD_TYPE Debug)

include_directories(${INCLUDE_DIR})
include_directories(${ASIO_INCLUDE_DIR})
add_executable(${PROJECT_MINECRAFT} ${SRC_FILES})

if (MSVC)
    target_compile_options(${PROJECT_MINECRAFT} PRIVATE
    #-W -Wall -Wextra -Wpedantic
    # -Wno-unused-variable
    # -Wno-unused-parameter
    # -Wno-strict-aliasing
    #-fno-omit-frame-pointer
    /Wall
    /W3
    )
else(
    target_compile_options(${PROJECT_MINECRAFT} PRIVATE
        # -fsanitize=address
        # -fno-omit-frame-pointer
        #-g
        # -static-libasan
        # -g3
        -O3
    ))
endif()


# target_compile_options(${PROJECT_MINECRAFT} PUBLIC -fsanitize=address -g3 -fno-omit-frame-pointer)
# target_link_options(${PROJECT_MINECRAFT} PUBLIC -fsanitize=address -g3 -fno-omit-frame-pointer)
target_link_libraries(${PROJECT_MINECRAFT} ${LINK_LIB})
target_include_directories(${PROJECT_MINECRAFT} PRIVATE ${IMGUI_DIR} ${IMGUI_DIR_BACK})