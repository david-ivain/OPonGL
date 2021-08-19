#ifndef OPONGL_RENDERER_HPP
#define OPONGL_RENDERER_HPP

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#endif

#include "quad.hpp"
#include "uniform_id.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class InputSystem;

class Renderer
{
    static bool isInitialized;
    static GLFWwindow* window;
    static GLuint vao;
    static GLuint vbo;
    static GLuint ebo;
    static GLuint vs;
    static GLuint fs;
    static GLuint shaderProgram;
    static std::unordered_map<UniformID, GLuint> uniforms;
    friend InputSystem;

  public:
    static void Init();
    static void Init(const char* title);
    static void CleanUp();
    static bool ShouldClose() { return glfwWindowShouldClose(window); }
    static void Clear(float r = 0, float g = 0, float b = 0, float = 1);
    static void SwapBuffers();
    static void AdaptAspect();
    static void Render(Quad quad, glm::mat4 transform);
    static void SetWindowTitle(const char* title);
    template <typename T> static void SetUniform(UniformID id, T value);
};

#endif