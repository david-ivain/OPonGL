#ifndef OPONGL_GAME_HPP
#define OPONGL_GAME_HPP

#include "quad.hpp"

#include <glm/glm.hpp>

Quad MakeBat();
Quad MakeBall();

struct Game
{
    glm::vec2 batSize { 0.1f, 0.4f };
    glm::vec2 ballSize { 0.1f, 0.1f };
    Quad batMesh { MakeBat() };
    Quad BallMesh { MakeBall() };
    glm::mat4 batTransforms[2] { glm::mat4(1.0), glm::mat4(1.0) };
    int batAxis[2] { 0, 0 };
    float batVelocities[2] { 2, 2 };
    glm::mat4 ballTransform { glm::mat4(1.0) };
    glm::vec2 ballMovement { 1, 0 };
    float ballVelocity { 3.0f };
    glm::vec2 walls { 4.0f / 3, 1 };

    Game();
    void Update(float delta);
};

#endif