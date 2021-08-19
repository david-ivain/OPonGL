cmake_minimum_required(VERSION 3.20)

project(opongl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(pong
src/main.cpp
src/quad.cpp
src/game.cpp
src/uniform_id.cpp
src/renderer.cpp
src/input_system.cpp
assets/shaders/white.vert
assets/shaders/white.frag)

set(RESOURCES assets/shaders/white.vert assets/shaders/white.frag)
target_include_directories(pong PRIVATE include/)
target_link_libraries(pong glfw ${OPENGL_LIBRARIES})
set_target_properties(pong PROPERTIES
MACOSX_BUNDLE TRUE
MACOSX_FRAMEWORK_IDENTIFIER me.davidivain.pong)

if (APPLE)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/pong.app/Contents/Resources)
else ()
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif ()
