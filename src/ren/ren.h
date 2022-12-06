#pragma once

#include <glm/glm.hpp>
#include <string>

#include "glheaders.h"
#include "mesh.h"

typedef struct GLFWwindow GLFWwindow;

namespace ren {
class Window;

/// @brief An idiomatic wrapper for GLFWwindow
struct Window {
    /// @param window A pointer to a GLFWwindow
    Window(GLFWwindow* window);
    /// @param title The window title
    Window(const std::string& title);
    Window(Window&& window);
    ~Window();

    /// @brief Makes this window the current context
    void make_current();
    /// @return If the window should close
    bool sould_close();
    /// @brief Sets the title in the title bar of this window
    /// @param title The title to set
    void set_title(const std::string& title);
    /// @return The window's aspect ratio as a float = width / height
    float get_aspect_ratio();
    /// @brief Swaps frame buffers
    void swap_buffers();
    void add_listener(void* pointer, GLFWkeyfun callback);
    /// @return The GLFWwindow pointer
    GLFWwindow* get_pointer();
    /// @return glm::vec2 the context's size in pixels
    glm::vec2 get_size();

private:
    /// @brief A pointer to a GLFWwindow
    GLFWwindow* window;
};

struct GLFWData;

/// @brief Initializes GLFW
/// @return If the initialization was successful
bool init();
/// @brief Attempts to create an instance of Window
/// @param title The title of the window
/// @return If successful the created Window else nothing
Window create_window(const std::string& title);
/// @brief Sets up OpenGL window hints
void set_window_hints();
/// @brief Initializes default shaders
/// @return If the initialization was successful
bool init_shaders();
/// @brief Sets the colour used to clear the screen
/// @param r Red
/// @param g Green
/// @param b Blue
/// @param a Alpha
void set_clear_color(float r, float g, float b, float a);
/// @brief Terminates GLFW
void terminate();
/// @brief Sets the swap interval
/// @param interval The minimum number of screen updates between buffer swaps
void set_swap_interval(int interval);
template <typename T>
void set_uniform(const std::string& id, const T& value);
/// @brief Instanciates the given mesh with the given transforms.
/// @param mesh
/// @param transforms
unsigned int instanciate(const Mesh* mesh, const std::vector<glm::mat4>* transforms);
/// @brief Draw n quads with different transforms
/// @param mesh
void draw_instances(const Mesh* mesh, unsigned int transform_buffer);
/// @brief Clears the screen with the colour set ren::with set_clear_color
void clear();
/// @brief Processes all pending GLFW events
void poll_events();
}
