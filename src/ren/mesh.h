#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace ren {
/// @brief Contains a vertex's data
struct Vertex {
    /// @brief The vertex's position
    glm::vec3 position;
    /// @brief The coordinates of this vertex based on loaded texture
    glm::vec2 texture_coord;
};

/// @brief A mesh to be displayed with ren::draw_instances
struct Mesh {
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    /// @brief Makes a quad with specified width and height
    /// @param width
    /// @param height
    /// @return
    static Mesh quadFrom(float width, float height);

    /// @brief The mesh's vertices
    std::vector<Vertex> vertices;
    /// @brief The index for each vertex
    std::vector<unsigned int> indices;
    /// @brief Vertex Array Object
    unsigned int VAO;
    /// @brief Vertex Buffer Object
    unsigned int VBO;
    /// @brief Element Buffer Object
    unsigned int EBO;

private:
    /// @brief Sets up the mesh in opengl
    void setup();
};
}
