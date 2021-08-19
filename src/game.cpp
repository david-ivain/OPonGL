#include "game.hpp"

#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum class Axis
{
    X,
    Y,
};

bool checkCollision(float position, float size, float wall);                                   // 1D
bool checkCollision(glm::vec2 position, glm::vec2 size, glm::vec2 oPosition, glm::vec2 oSize); // 2D Box
int aiDecideDumb(float batPosition, float ballPosition);

Quad MakeBat() { return Quad(0.1, 0.4); }
Quad MakeBall() { return Quad(0.1, 0.1); }

Game::Game()
{
    batTransforms[0] = glm::translate(batTransforms[0], { -1.0f, 0, 0 });
    batTransforms[1] = glm::translate(batTransforms[1], { 1.0f, 0, 0 });
}

void Game::Update(float delta)
{
    batAxis[1] = aiDecideDumb(batTransforms[1][3].y, ballTransform[3].y);
    for (int i = 0; i < 2; i++)
    {
        float batMovement = (float)batAxis[i] * batVelocities[i] * delta;
        auto halfBatSizeY = batSize.y / 2;
        batTransforms[i][3].y = checkCollision(batTransforms[i][3].y + batMovement, batSize.y, walls.y) //
                                    ? walls.y - halfBatSizeY
                                    : checkCollision(batTransforms[i][3].y + batMovement, batSize.y, -walls.y) //
                                          ? -(walls.y - halfBatSizeY)
                                          : batTransforms[i][3].y + batMovement;
    }
    glm::vec2 ballMovementC = ballMovement * ballVelocity * delta;
    auto calculatedT = glm::translate(ballTransform, glm::vec3(ballMovementC, 0));
    // Collisions Ball Bat
    for (size_t i = 0; i < 2; i++)
    {
        if (checkCollision({ calculatedT[3].x, calculatedT[3].y }, ballSize,
                           { batTransforms[i][3].x, batTransforms[i][3].y }, batSize))
        {
            ballMovement = glm::normalize(ballTransform[3] - batTransforms[i][3]);
            ballMovementC = ballMovement * ballVelocity * delta;
        }
    }
    calculatedT = glm::translate(ballTransform, glm::vec3(ballMovementC, 0));
    // Collisions Ball Wall
    if (checkCollision(calculatedT[3].x, ballSize.x, walls.x) || checkCollision(calculatedT[3].x, ballSize.x, -walls.x))
    {
        ballTransform[3] = glm::vec4(0, 0, 0, 1);
        ballMovement = { 0, 0 };
    }
    if (checkCollision(calculatedT[3].y, ballSize.y, walls.y) || checkCollision(calculatedT[3].y, ballSize.y, -walls.y))
    {
        ballMovementC.y = -ballMovementC.y;
        ballMovement.y = -ballMovement.y;
    }

    ballTransform = glm::translate(ballTransform, glm::vec3(ballMovementC, 0));
}

bool checkCollision(float position, float size, float wall)
{
    auto halfSize = size / 2;
    return wall > position ? (position + halfSize > wall) : wall < position ? (position - halfSize < wall) : true;
}

bool checkCollision(glm::vec2 position, glm::vec2 size, glm::vec2 oPosition, glm::vec2 oSize)
{
    glm::vec2 halfSize = size / 2.0f;
    glm::vec2 oHalfSize = oSize / 2.0f;
    auto minX = position.x - halfSize.x, //
        maxX = position.x + halfSize.x,  //
        minY = position.y - halfSize.y,  //
        maxY = position.y + halfSize.y;
    auto oMinX = oPosition.x - oHalfSize.x, //
        oMaxX = oPosition.x + oHalfSize.x,  //
        oMinY = oPosition.y - oHalfSize.y,  //
        oMaxY = oPosition.y + oHalfSize.y;
    auto collideX = minX >= oMinX && minX <= oMaxX    // min in borders
                    || maxX >= oMinX && maxX <= oMaxX // max in borders
                    || minX < oMinX && maxX > oMaxX;  // min max around
    auto collideY = minY >= oMinY && minY <= oMaxY    //
                    || maxY >= oMinY && maxY <= oMaxY //
                    || minY < oMinY && maxY > oMaxY;
    return collideX && collideY;
}

int aiDecideDumb(float batPosition, float ballPosition)
{
    static bool moving = false;
    float margin = 0.2f, lowerMargin = 0.05f;
    if (!moving)
    {
        if (batPosition - margin > ballPosition)
        {
            moving = true;
            return -1;
        }
        else if (batPosition + margin < ballPosition)
        {
            moving = true;
            return 1;
        }
        return 0;
    }
    if (batPosition - lowerMargin < ballPosition && batPosition + lowerMargin > ballPosition)
    {
        moving = false;
        return 0;
    }

    if (batPosition > ballPosition)
        return -1;
    return 1;
}
