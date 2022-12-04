#include <iostream>
#include <cstring>
#include <vector>

#include "ren/ren.h"
#include "utils/framecounter.h"
#include "game/game.h"

constexpr auto WINDOW_TITLE_FORMAT_STRING = "Pong FPS: %.2f Score: %d - %d";
constexpr float CLEAR_COLOR[4]{0, 0, 0, 1};

enum class ExitCodes : int
{
    GLFWInitFailed = 1,
    WindowInitFailed,
    ShadersInitFailed
};

void handle_key(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Game *game = (Game *)glfwGetWindowUserPointer(window);
    switch (action)
    {
    case GLFW_PRESS:
        switch (key)
        {
        case GLFW_KEY_UP:
            game->bat_axis[0]++;
            break;
        case GLFW_KEY_DOWN:
            game->bat_axis[0]--;
            break;
        case GLFW_KEY_SPACE:
            game->launch();
        default:
            break;
        }
        break;
    case GLFW_RELEASE:
        switch (key)
        {
        case GLFW_KEY_UP:
            game->bat_axis[0]--;
            break;
        case GLFW_KEY_DOWN:
            game->bat_axis[0]++;
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
}

/// @brief Initializes GLFW and a Window
/// @return The created Window
ren::Window init_window()
{
    // init GLFW

    if (!ren::init())
    {
        std::cerr << "Error: Failed to initialize glfw." << std::endl;
        exit(static_cast<int>(ExitCodes::GLFWInitFailed));
    }
    std::atexit(ren::terminate);
    std::set_terminate(ren::terminate);
    ren::set_window_hints();

    // init window

    auto maybe_window = ren::create_window("Pong");
    if (!maybe_window.has_value())
    {
        std::cerr << "Error: Failed to create window." << std::endl;
        exit(static_cast<int>(ExitCodes::WindowInitFailed));
    }
    auto window = std::move(maybe_window.value());
    window.make_current();
    ren::set_clear_color(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);

    if (!ren::init_shaders())
    {
        std::cerr << "Error: Failed to initialize shaders." << std::endl;
        exit(static_cast<int>(ExitCodes::ShadersInitFailed));
    }
    return window;
}

int main(int argc, char const *argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    auto vsync_enabled = 1;
    for (size_t i = 0; i < argc; i++)
    {
        if (args[i] == "--novsync")
            vsync_enabled = false;
    }

    // init GLFW + window

    auto window = init_window();
    ren::set_swap_interval(vsync_enabled);

    char window_title[32];

    utils::FrameCounter frame_counter(1);

    std::string aspect_uniform = "aspect";

    // instanciate meshes

    Game game{};
    window.add_listener(&game, &handle_key);

    unsigned int bat_transform_buffer = ren::instanciate(&game.bat_mesh, &game.bat_transforms);
    unsigned int ball_transform_buffer = ren::instanciate(&game.ball_mesh, &game.ball_transforms);

    // main loop

    while (!window.sould_close())
    {
        // calculate framerate
        // update title every second

        auto frame_info = frame_counter.get_frame_info();
        if (frame_info.has_changed)
        {
            std::snprintf(window_title, sizeof(window_title), WINDOW_TITLE_FORMAT_STRING, frame_info.frame_rate, game.scores[0], game.scores[1]);
            window.set_title(window_title);
        }

        game.update(frame_info.delta_seconds);

        // clear > draw > events

        ren::clear();
        ren::set_uniform(aspect_uniform, window.get_aspect_ratio());
        ren::draw_instances(&game.bat_mesh, bat_transform_buffer);
        ren::draw_instances(&game.ball_mesh, ball_transform_buffer);
        window.swap_buffers();
        ren::poll_events();
    }
    return 0;
}
