#include "game.h"

#include <glm/gtc/matrix_transform.hpp>

bool check_collision(float position, float size, float wall); // 1D
bool check_collision(glm::vec2 position, glm::vec2 size, glm::vec2 other_position, glm::vec2 other_size); // 2D Box
int ai_decide_dumb(float bat_position, float ball_position);

Game::Game()
{
    bat_transforms[0] = glm::translate(bat_transforms[0], { -1.0f, 0, 0 });
    bat_transforms[1] = glm::translate(bat_transforms[1], { 1.0f, 0, 0 });
}

void Game::update(float delta_seconds)
{
    bat_axis[1] = ai_decide_dumb(bat_transforms[1][3].y, ball_transforms[0][3].y);
    for (int i = 0; i < 2; i++) {
        float bat_movement = (float)bat_axis[i] * BAT_VELOCITY * delta_seconds;
        auto half_bat_size_y = BAT_SIZE.y / 2;
        bat_transforms[i][3].y = check_collision(bat_transforms[i][3].y + bat_movement, BAT_SIZE.y, walls.y) //
            ? walls.y - half_bat_size_y
            : check_collision(bat_transforms[i][3].y + bat_movement, BAT_SIZE.y, -walls.y) //
                ? -(walls.y - half_bat_size_y)
                : bat_transforms[i][3].y + bat_movement;
    }
    if (!ball_was_launched)
        return;
    glm::vec2 ball_movement_calculated = ball_movement * BALL_VELOCITY * delta_seconds;
    auto ball_transform_calculated = glm::translate(ball_transforms[0], glm::vec3(ball_movement_calculated, 0));
    // Collisions Ball Bat
    for (size_t i = 0; i < 2; i++) {
        if (check_collision({ ball_transform_calculated[3].x, ball_transform_calculated[3].y }, BALL_SIZE,
                { bat_transforms[i][3].x, bat_transforms[i][3].y }, BAT_SIZE)) {
            ball_movement = glm::normalize(ball_transforms[0][3] - bat_transforms[i][3]);
            ball_movement_calculated = ball_movement * BALL_VELOCITY * delta_seconds;
        }
    }
    ball_transform_calculated = glm::translate(ball_transforms[0], glm::vec3(ball_movement_calculated, 0));
    // Collisions Ball Wall
    auto ball_collides_with_left_wall = check_collision(ball_transform_calculated[3].x, BALL_SIZE.x, -walls.x);
    auto ball_collides_with_right_wall = check_collision(ball_transform_calculated[3].x, BALL_SIZE.x, walls.x);
    if (ball_collides_with_right_wall || ball_collides_with_left_wall) {
        scores[ball_collides_with_right_wall ? 0 : 1]++;
        ball_transforms[0][3] = glm::vec4(0, 0, 0, 1);
        ball_movement = { 0, 0 };
        ball_was_launched = false;
    }
    if (check_collision(ball_transform_calculated[3].y, BALL_SIZE.y, walls.y) || check_collision(ball_transform_calculated[3].y, BALL_SIZE.y, -walls.y)) {
        ball_movement_calculated.y = -ball_movement_calculated.y;
        ball_movement.y = -ball_movement.y;
    }

    ball_transforms[0] = glm::translate(ball_transforms[0], glm::vec3(ball_movement_calculated, 0));
}

void Game::launch()
{
    if (ball_was_launched)
        return;
    ball_movement = { 1, 0 };
    ball_was_launched = true;
}

bool check_collision(float position, float size, float wall)
{
    auto half_size = size / 2;
    return wall > position ? (position + half_size > wall) : wall < position ? (position - half_size < wall)
                                                                             : true;
}

bool check_collision(glm::vec2 position, glm::vec2 size, glm::vec2 other_position, glm::vec2 other_size)
{
    glm::vec2 half_size = size / 2.0f;
    glm::vec2 other_half_size = other_size / 2.0f;
    auto min_x = position.x - half_size.x, //
        max_x = position.x + half_size.x, //
        min_y = position.y - half_size.y, //
        max_y = position.y + half_size.y;
    auto other_min_x = other_position.x - other_half_size.x, //
        other_max_x = other_position.x + other_half_size.x, //
        other_min_y = other_position.y - other_half_size.y, //
        other_max_y = other_position.y + other_half_size.y;
    auto collide_x = min_x >= other_min_x && min_x <= other_max_x // min in borders
        || max_x >= other_min_x && max_x <= other_max_x // max in borders
        || min_x < other_min_x && max_x > other_max_x; // min max around
    auto collide_y = min_y >= other_min_y && min_y <= other_max_y //
        || max_y >= other_min_y && max_y <= other_max_y //
        || min_y < other_min_y && max_y > other_max_y;
    return collide_x && collide_y;
}

int ai_decide_dumb(float bat_position, float ball_position)
{
    static bool moving = false;
    float margin = 0.2f, lower_margin = 0.05f;
    if (!moving) {
        if (bat_position - margin > ball_position) {
            moving = true;
            return -1;
        } else if (bat_position + margin < ball_position) {
            moving = true;
            return 1;
        }
        return 0;
    }
    if (bat_position - lower_margin < ball_position && bat_position + lower_margin > ball_position) {
        moving = false;
        return 0;
    }

    if (bat_position > ball_position)
        return -1;
    return 1;
}
