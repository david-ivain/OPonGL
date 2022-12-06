#pragma once

#include <string>
#include <unordered_map>

#include "glheaders.h"

namespace ren {
struct Shader {
    Shader(const std::string& source, GLenum type);
    ~Shader();

    static Shader from_file(const std::string& path, GLenum type);

    const GLuint object;
};
struct ShaderProgram {
    ShaderProgram();
    ~ShaderProgram();

    void attach(const Shader& shader);
    void link();
    void use();
    GLint get_uniform_location(const std::string& id);
    template <typename T>
    void set_uniform(GLint location, const T& value);

    const GLuint object;
};
}
