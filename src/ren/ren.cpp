#include "ren.h"

#include <cstdio>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <vector>

#include "shader.h"

namespace ren {
// Window

Window::Window(GLFWwindow* window)
    : window(window)
{
    std::printf("Log: Window constructed.\n");
}

Window::Window(const std::string& title)
{
    window = glfwCreateWindow(800, 600, title.c_str(), nullptr, nullptr);
    std::printf("Log: Window constructed.\n");
}

Window::Window(Window&& window)
    : window(std::move(window.window))
{
    window.window = nullptr;
}

Window::~Window()
{
    if (window == nullptr)
        return;
    glfwDestroyWindow(window);
    std::printf("Log: Window destroyed.\n");
}

void Window::make_current()
{
    glfwMakeContextCurrent(window);
    std::printf("Log: Window made current.\n");
}

bool Window::sould_close()
{
    return glfwWindowShouldClose(window);
}

void Window::set_title(const std::string& title)
{
    glfwSetWindowTitle(window, title.c_str());
}

float Window::get_aspect_ratio()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return (float)width / (float)height;
}

void Window::swap_buffers()
{
    glfwSwapBuffers(window);
}

void Window::add_listener(void* pointer, GLFWkeyfun callback)
{
    glfwSetWindowUserPointer(window, pointer);
    glfwSetKeyCallback(window, callback);
}

GLFWwindow* Window::get_pointer()
{
    return window;
}

// GLFW

bool glfw_is_initialized = false;
std::unordered_map<const Mesh*, const std::vector<glm::mat4>*> instances;

bool init()
{
    if (glfw_is_initialized)
        return true;
    std::printf("Log: GLFW init attempt.\n");
    glfw_is_initialized = glfwInit();
    return glfw_is_initialized;
}

Window create_window(const std::string& title)
{
    GLFWwindow* window = glfwCreateWindow(800, 600, title.c_str(), nullptr, nullptr);
    return { window };
}

void set_window_hints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::printf("Log: GLFW window hints set.\n");
}

void set_clear_color(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    std::printf("Log: GL clear color set.\n");
}

void terminate()
{
    glfwTerminate();
    glfw_is_initialized = false;
    std::printf("Log: GLFW terminated.\n");
}

void set_swap_interval(int interval)
{
    glfwSwapInterval(interval);
    std::printf("Log: GLFW swap interval set to %d.\n", interval);
}

// template <>
// void set_uniform(const std::string &id, const glm::mat4 &value)
// {
//     glProgramUniformMatrix4fv(shaderProgram, uniforms[id], 1, false, glm::value_ptr(value));
// }

unsigned int instanciate(const Mesh* mesh, const std::vector<glm::mat4>* transforms)
{
    instances[mesh] = transforms;
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, transforms->size() * sizeof(glm::mat4), transforms->data(), GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new
    // vertexAttribPointers normally you'd want to do this in a more organized fashion, but for learning purposes this
    // will do.
    // -----------------------------------------------------------------------------------------------------------------------------------
    unsigned int VAO = mesh->VAO;
    glBindVertexArray(VAO);
    // set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
    return buffer;
}

void draw_instances(const Mesh* mesh, unsigned int buffer)
{
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, instances[mesh]->size() * sizeof(glm::mat4), instances[mesh]->data(), GL_STATIC_DRAW);
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0,
        instances[mesh]->size());
    glBindVertexArray(0);
}

void clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void poll_events()
{
    glfwPollEvents();
}
}
