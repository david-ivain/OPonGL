cmake_minimum_required(VERSION 3.22)

project(opongl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(pong
src/main.cpp
src/game/game.h
src/game/game.cpp
src/ren/glheaders.h
src/ren/ren.h
src/ren/ren.cpp
src/ren/mesh.h
src/ren/mesh.cpp
src/ren/shader.h
src/ren/shader.cpp
src/utils/timer.h
src/utils/timer.cpp
src/utils/framecounter.h
src/utils/framecounter.cpp
)

target_link_libraries(pong glfw ${OPENGL_LIBRARIES})
set_target_properties(pong PROPERTIES
MACOSX_BUNDLE TRUE
MACOSX_FRAMEWORK_IDENTIFIER me.davidivain.pong)

if (APPLE)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/pong.app/Contents/Resources)
else ()
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif ()