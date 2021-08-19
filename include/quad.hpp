#ifndef OPONGL_QUAD_HPP
#define OPONGL_QUAD_HPP

#include "debug.hpp"

#include <array>
#include <memory>

struct Quad
{
    std::array<float, 20> vertices;
    std::array<unsigned int, 6> indices;

    Quad(std::array<float, 20> v, std::array<unsigned int, 6> i)
    {
        vertices = v;
        indices = i;
    }
    Quad(float width, float height)
        : Quad({ 0.5f * width,  0.5f * height,  0.0f, 1.0f, 1.0f,   // Vertex 1
                 -0.5f * width, 0.5f * height,  0.0f, 0.0f, 1.0f,   // Vertex 2
                 -0.5f * width, -0.5f * height, 0.0f, 0.0f, 0.0f,   // Vertex 3
                 0.5f * width,  -0.5f * height, 0.0f, 1.0f, 0.0f }, // Vertex 4
               {
                   0u, 1u, 2u, // Triangle 1
                   2u, 3u, 0u  // Triangle 2
               })
    {
    }
};

#endif