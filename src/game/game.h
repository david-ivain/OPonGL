#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vector>

#include "../ren/mesh.h"

#define BAT_VELOCITY 2.0f
#define BALL_VELOCITY 3.0f
constexpr float WALLS_ASPECT_RATIO = 4.0f / 3;
constexpr glm::vec2 BAT_SIZE { 0.1, 0.4 };
constexpr glm::vec2 BALL_SIZE { 0.1, 0.1 };
constexpr glm::vec2 walls { WALLS_ASPECT_RATIO, 1 };

/// @brief The game object. Instanciated at start and updated each frame.
struct Game {
    Game();
    /// @brief To be called each frame. Updates the game's state
    /// @param delta_seconds The delta in seconds since last frame
    void update(float delta_seconds);
    /// @brief Launches the ball
    void launch();

    ren::Mesh bat_mesh = ren::Mesh::quadFrom(0.1, 0.4), ball_mesh = ren::Mesh::quadFrom(0.1, 0.1);
    std::vector<glm::mat4> bat_transforms { glm::mat4(1.0), glm::mat4(1.0) }, ball_transforms { glm::mat4(1.0) };
    std::array<int, 2> bat_axis { 0, 0 };
    glm::vec2 ball_movement { 0, 0 };
    std::array<unsigned int, 2> scores { 0, 0 };

private:
    bool ball_was_launched { false };
};
