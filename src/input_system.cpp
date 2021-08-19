#include "input_system.hpp"

#include "renderer.hpp"

void InputSystem::Bind(void* pointer, GLFWkeyfun callback)
{
    glfwSetWindowUserPointer(Renderer::window, pointer);
    glfwSetKeyCallback(Renderer::window, callback);
}