#ifndef OPONGL_INPUT_SYSTEM_HPP
#define OPONGL_INPUT_SYSTEM_HPP

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#endif

#include <GLFW/glfw3.h>

class InputSystem
{
  public:
    static void Bind(void* pointer, GLFWkeyfun callback);
};

#endif