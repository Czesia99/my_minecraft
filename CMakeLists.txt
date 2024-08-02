cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_MINECRAFT "my_minecraft")

project(${PROJECT_MINECRAFT} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(SRC ./src)
set(MYGL ./src/mygl)

add_subdirectory(include/glfw)
add_subdirectory(include/mygl)

set(SRC_FILES
    ${SRC}/main.cpp
    ${SRC}/GameScene.cpp
    ${SRC}/Chunk.cpp
    ${SRC}/Client.cpp
)

set(LINK_LIB
    glfw
    OpenGL::GL
    mygl
)
# set(CMAKE_BUILD_TYPE Debug)

include_directories(${INCLUDE_DIR})
add_executable(${PROJECT_MINECRAFT} ${SRC_FILES})

target_link_libraries(${PROJECT_MINECRAFT} ${LINK_LIB})
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")