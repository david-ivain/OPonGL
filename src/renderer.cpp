#include "renderer.hpp"

#include "debug.hpp"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

bool Renderer::isInitialized = false;
GLFWwindow* Renderer::window = nullptr;
GLuint Renderer::vao = 0;
GLuint Renderer::vbo = 0;
GLuint Renderer::ebo = 0;
GLuint Renderer::vs = 0;
GLuint Renderer::fs = 0;
GLuint Renderer::shaderProgram = 0;
std::unordered_map<UniformID, GLuint> Renderer::uniforms {};

template <> void Renderer::SetUniform<float>(UniformID id, float value)
{
    glProgramUniform1f(shaderProgram, uniforms[id], value);
}
template <> void Renderer::SetUniform<glm::mat4>(UniformID id, glm::mat4 value)
{
    glProgramUniformMatrix4fv(shaderProgram, uniforms[id], 1, false, glm::value_ptr(value));
}

void Renderer::Init()
{
    if (!glfwInit())
    {
        DEBUG_FPRINTF(stderr, "Error: Failed to initialize glfw.");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(800, 600, "Pong", nullptr, nullptr);
    if (!window)
    {
        DEBUG_FPRINTF(stderr, "Error: Failed to create window.");
        glfwTerminate();
        exit(2);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vs = glCreateShader(GL_VERTEX_SHADER);
    std::ifstream ifs("./assets/shaders/white.vert");
    std::string vSource((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    const char* vsSource = vSource.c_str();
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
        DEBUG_FPRINTF(stderr, "VS: %s", log);
    }
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream ifs2("./assets/shaders/white.frag");
    std::string fSource((std::istreambuf_iterator<char>(ifs2)), (std::istreambuf_iterator<char>()));
    const char* fsSource = fSource.c_str();
    ifs2.close();
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);
    success = 0;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE)
    {
        glGetShaderInfoLog(fs, sizeof(log), &logLength, log);
        DEBUG_FPRINTF(stderr, "FS: %s", log);
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
        DEBUG_FPRINTF(stderr, "SP: %s", log);
    }
    glUseProgram(shaderProgram);
    uniforms[UniformID::Aspect] = glGetUniformLocation(shaderProgram, uniformIdString(UniformID::Aspect).c_str());
    uniforms[UniformID::Transform] = glGetUniformLocation(shaderProgram, uniformIdString(UniformID::Transform).c_str());
    isInitialized = true;
}

void Renderer::CleanUp()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Renderer::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SwapBuffers() { glfwSwapBuffers(window); }

void Renderer::AdaptAspect()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;
    SetUniform(UniformID::Aspect, aspect);
}

void Renderer::Render(Quad quad, glm::mat4 transform)
{
    SetUniform(UniformID::Transform, transform);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quad.vertices.size(), quad.vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * quad.indices.size(), quad.indices.data(),
                 GL_STATIC_DRAW);

    /* Draw Elements */
    glDrawElements(GL_TRIANGLES, quad.indices.size(), GL_UNSIGNED_INT, 0);
}