#include "debug.hpp"

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#endif
#include <GLFW/glfw3.h>

int main(int argc, char const* argv[])
{
    if (!glfwInit())
    {
        DEBUG_FPRINTF(stderr, "Error: Failed to initialize glfw.");
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto window = glfwCreateWindow(800, 600, "Pong", nullptr, nullptr);
    if (!window)
    {
        DEBUG_FPRINTF(stderr, "Error: Failed to create window.");
        glfwTerminate();
        return 2;
    }
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
