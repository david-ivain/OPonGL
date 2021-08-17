cmake_minimum_required(VERSION 3.20)

project(opongl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(pong src/main.cpp)
target_include_directories(pong PRIVATE include/)
target_link_libraries(pong glfw ${OPENGL_LIBRARIES})