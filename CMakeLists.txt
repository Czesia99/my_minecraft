cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_MINECRAFT "my_minecraft")

project(${PROJECT_MINECRAFT} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(SRC ./src)

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
    ${SRC}/Chunk.cpp
    ${SRC}/Client.cpp
    ${SRC}/Entity.cpp
    ${IMGUI_SOURCES}
)

set(LINK_LIB
    glfw
    OpenGL::GL
    mygl
)

if (WIN32)
    add_definitions(-DNOMINMAX -DWIN32_LEAN_AND_MEAN)
endif()
# set(CMAKE_BUILD_TYPE Debug)

include_directories(${INCLUDE_DIR})
add_executable(${PROJECT_MINECRAFT} ${SRC_FILES})

target_link_libraries(${PROJECT_MINECRAFT} ${LINK_LIB})
target_include_directories(${PROJECT_MINECRAFT} PRIVATE ${IMGUI_DIR} ${IMGUI_DIR_BACK})
# target_include_directories(${PROJECT_MINECRAFT} PRIVATE ${IMGUI_DIR} ${IMGUI_DIR_BACK})
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")