#include "ren.h"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>

namespace ren
{
    // Window

    Window::Window(GLFWwindow *window) : window(window)
    {
        std::cout << "Log: Window constructed." << std::endl;
    }

    Window::Window(const std::string &title)
    {
        window = glfwCreateWindow(800, 600, title.c_str(), nullptr, nullptr);
        std::cout << "Log: Window constructed." << std::endl;
    }

    Window::Window(Window &&window) : window(std::move(window.window))
    {
        window.window = nullptr;
    }

    Window::~Window()
    {
        if (window == nullptr)
            return;
        glfwDestroyWindow(window);
        std::cout << "Log: Window destroyed." << std::endl;
    }

    void Window::make_current()
    {
        glfwMakeContextCurrent(window);
        std::cout << "Log: Window made current." << std::endl;
    }

    bool Window::sould_close()
    {
        return glfwWindowShouldClose(window);
    }

    void Window::set_title(const std::string &title)
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

    void Window::add_listener(void *pointer, GLFWkeyfun callback)
    {
        glfwSetWindowUserPointer(window, pointer);
        glfwSetKeyCallback(window, callback);
    }

    // GLFW

    bool glfw_is_initialized = false;
    GLuint vs;
    GLuint fs;
    GLuint shaderProgram;
    std::unordered_map<std::string, GLuint> uniforms;
    std::unordered_map<const Mesh *, const std::vector<glm::mat4> *> instances;

    bool init()
    {
        if (glfw_is_initialized)
            return true;
        std::cout << "Log: GLFW init attempt." << std::endl;
        glfw_is_initialized = glfwInit();
        return glfw_is_initialized;
    }

    std::optional<Window> create_window(const std::string &title)
    {
        GLFWwindow *window = glfwCreateWindow(800, 600, title.c_str(), nullptr, nullptr);
        return window ? std::optional<Window>{window} : std::nullopt;
    }

    void set_window_hints()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        std::cout << "Log: GLFW window hints set." << std::endl;
    }

    bool init_shaders()
    {
        vs = glCreateShader(GL_VERTEX_SHADER);
        std::ifstream ifs("./assets/shaders/white.vert");
        std::string vSource((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        const char *vsSource = vSource.c_str();
        ifs.close();
        glShaderSource(vs, 1, &vsSource, NULL);
        glCompileShader(vs);
        int success = 0;
        GLsizei logLength;
        GLchar log[1024];
        glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE)
        {
            glGetShaderInfoLog(vs, sizeof(log), &logLength, log);
            std::fprintf(stderr, "VS: %s", log);
            return false;
        }
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        std::ifstream ifs2("./assets/shaders/white.frag");
        std::string fSource((std::istreambuf_iterator<char>(ifs2)), (std::istreambuf_iterator<char>()));
        const char *fsSource = fSource.c_str();
        ifs2.close();
        glShaderSource(fs, 1, &fsSource, NULL);
        glCompileShader(fs);
        success = 0;
        glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE)
        {
            glGetShaderInfoLog(fs, sizeof(log), &logLength, log);
            std::fprintf(stderr, "FS: %s", log);
            return false;
        }
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);
        success = 0;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (success != GL_TRUE)
        {
            glGetProgramInfoLog(shaderProgram, sizeof(log), &logLength, log);
            std::fprintf(stderr, "SP: %s", log);
            return false;
        }
        glUseProgram(shaderProgram);
        uniforms["aspect"] = glGetUniformLocation(shaderProgram, "aspect");
        std::printf("Log: aspect uniform %d.\n", uniforms["aspect"]);
        return true;
    }

    void set_clear_color(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        std::cout << "Log: GL clear color set." << std::endl;
    }

    void terminate()
    {
        glfwTerminate();
        glfw_is_initialized = false;
        std::cout << "Log: GLFW terminated." << std::endl;
    }

    void set_swap_interval(int interval)
    {
        glfwSwapInterval(interval);
        std::cout << "Log: GLFW swap interval set to " << interval << "." << std::endl;
    }

    template <>
    void set_uniform(const std::string &id, const float &value)
    {
        glProgramUniform1f(shaderProgram, uniforms[id], value);
    }
    // template <>
    // void set_uniform(const std::string &id, const glm::mat4 &value)
    // {
    //     glProgramUniformMatrix4fv(shaderProgram, uniforms[id], 1, false, glm::value_ptr(value));
    // }

    unsigned int instanciate(const Mesh *mesh, const std::vector<glm::mat4> *transforms)
    {
        instances[mesh] = transforms;
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, transforms->size() * sizeof(glm::mat4), transforms->data(), GL_STATIC_DRAW);

        // set transformation matrices as an instance vertex attribute (with divisor 1)
        // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
        // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
        // -----------------------------------------------------------------------------------------------------------------------------------
        unsigned int VAO = mesh->VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
        return buffer;
    }

    void draw_instances(const Mesh *mesh, unsigned int buffer)
    {
        // std::cout << glm::to_string(instances[mesh]->at(0)) << std::endl;
        glBindVertexArray(mesh->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, instances[mesh]->size() * sizeof(glm::mat4), instances[mesh]->data(), GL_STATIC_DRAW);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0, instances[mesh]->size());
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