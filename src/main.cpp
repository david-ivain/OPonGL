#include "debug.hpp"
#include "game.hpp"
#include "input_system.hpp"
#include "quad.hpp"
#include "renderer.hpp"
#include "uniform_id.hpp"

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#endif
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void handleKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Game* game = (Game*)glfwGetWindowUserPointer(window);
    switch (action)
    {
    case GLFW_PRESS:
        switch (key)
        {
        case GLFW_KEY_UP:
            game->batAxis[0]++;
            break;
        case GLFW_KEY_DOWN:
            game->batAxis[0]--;
            break;
        case GLFW_KEY_SPACE:
            game->ballMovement = { 1, 0 };
        default:
            break;
        }
        break;
    case GLFW_RELEASE:
        switch (key)
        {
        case GLFW_KEY_UP:
            game->batAxis[0]--;
            break;
        case GLFW_KEY_DOWN:
            game->batAxis[0]++;
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
}

class Timer
{
    std::chrono::steady_clock::time_point oldT;
    std::chrono::steady_clock::time_point newT;

  public:
    Timer()
        : oldT(std::chrono::high_resolution_clock::now())
        , newT(oldT)
    {
    }
    float GetDeltaS()
    {
        oldT = newT;
        newT = std::chrono::high_resolution_clock::now();
        return (float)(newT - oldT).count() / 1000000000;
    }
};

int main(int argc, char const* argv[])
{
    auto baseWindowTitle = "Pong FPS: 00.00";
    char windowTitle[32];
    std::strcpy(windowTitle, baseWindowTitle);
    Renderer::Init(windowTitle);
    Game game;
    float* t = glm::value_ptr(game.ballTransform);
    InputSystem::Bind(&game, &handleKey);
    Timer timer;
    float sDelta = 0;
    unsigned int c = 0;
    while (!Renderer::ShouldClose())
    {
        auto delta = timer.GetDeltaS();
        if (sDelta < 1)
        {
            c++;
            sDelta += delta;
        }
        else
        {
            std::snprintf(windowTitle, sizeof(windowTitle), "Pong FPS: %.2f", 1.0f / (sDelta / c));
            Renderer::SetWindowTitle(windowTitle);
            c = 0;
            sDelta = 0;
        }
        game.Update(delta);
        Renderer::Clear();
        Renderer::AdaptAspect();
        Renderer::Render(game.batMesh, game.batTransforms[0]);
        Renderer::Render(game.batMesh, game.batTransforms[1]);
        Renderer::Render(game.BallMesh, game.ballTransform);
        Renderer::SwapBuffers();
        glfwPollEvents();
    }
    Renderer::CleanUp();
    return 0;
}
