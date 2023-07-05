cmake_minimum_required(VERSION 3.12)
project(opengl-pano)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})

set(SOURCES
    src/main.cc
    src/program.cc
    src/window.cc
    src/mesh.cc
    src/camera.cc
    src/object.cc
    src/scene.cc
    src/lodepng.cc
    src/texture.cc
    src/light.cc
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic.vert ${CMAKE_CURRENT_BINARY_DIR}/shaders/basic.vert COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic.frag ${CMAKE_CURRENT_BINARY_DIR}/shaders/basic.frag COPYONLY)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    glfw
    Threads::Threads
)
