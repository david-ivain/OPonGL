#include "shader.h"

#include <fstream>

namespace ren {
Shader::Shader(const std::string& source, GLenum type)
    : object(glCreateShader(type))
{
    const char* code = source.c_str();
    glShaderSource(object, 1, &code, NULL);
    glCompileShader(object);
    int success = 0;
    GLsizei logLength;
    GLchar log[1024];
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(object, sizeof(log), &logLength, log);
        std::fprintf(stderr, "Shader Compilation: %s", log);
        throw std::runtime_error("Shader Compilation Error");
    }
}

Shader::~Shader()
{
    glDeleteShader(object);
}

Shader Shader::from_file(const std::string& path, GLenum type)
{
    std::ifstream stream(path.c_str());
    std::string source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    stream.close();
    std::printf("Log: Successfully compiled shader %s.\n", path.c_str());
    return Shader(source, type);
}

ShaderProgram::ShaderProgram()
    : object(glCreateProgram())
{
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(object);
}

void ShaderProgram::attach(const Shader& shader)
{
    glAttachShader(object, shader.object);
}

void ShaderProgram::link()
{
    glLinkProgram(object);
    int success = 0;
    GLsizei logLength;
    GLchar log[1024];
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        glGetProgramInfoLog(object, sizeof(log), &logLength, log);
        std::fprintf(stderr, "Shader Program Compilation: %s", log);
        throw std::runtime_error("Shader Program Compilation Error");
    }
    std::printf("Log: Successfully linked shader program %u.\n", object);
}

void ShaderProgram::use()
{
    glUseProgram(object);
}

GLint ShaderProgram::get_uniform_location(const std::string& id)
{
    return glGetUniformLocation(object, id.c_str());
}

template <>
void ShaderProgram::set_uniform(GLint location, const float& value)
{
    glProgramUniform1f(object, location, value);
}
}
