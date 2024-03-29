#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>

#include "game/game.h"
#include "ren/ren.h"
#include "ren/shader.h"
#include "utils/framecounter.h"

constexpr auto WINDOW_TITLE_FORMAT_STRING = "Pong FPS: %.2f Score: %d - %d";
constexpr float CLEAR_COLOR[4] { 0, 0, 0, 1 };

enum class ExitCodes : int {
    GLFWInitFailed = 1,
    WindowInitFailed,
    ShadersInitFailed,
    UniformGetLocationFailed
};

void handle_key(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    Game* game = (Game*)glfwGetWindowUserPointer(window);
    switch (action) {
    case GLFW_PRESS:
        switch (key) {
        case GLFW_KEY_UP:
            game->bat_axis[0]++;
            break;
        case GLFW_KEY_DOWN:
            game->bat_axis[0]--;
            break;
        case GLFW_KEY_SPACE:
            game->launch();
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        default:
            break;
        }
        break;
    case GLFW_RELEASE:
        switch (key) {
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

int main(int argc, char const* argv[])
{
    // get console params

    std::vector<std::string> args(argv, argv + argc);
    auto vsync_enabled = 1;
    for (size_t i = 0; i < argc; i++) {
        if (args[i] == "--novsync")
            vsync_enabled = 0;
    }

    // init GLFW

    if (!ren::init()) {
        std::fprintf(stderr, "Error: Failed to initialize glfw.\n");
        exit(static_cast<int>(ExitCodes::GLFWInitFailed));
    }
    std::atexit(ren::terminate);
    std::set_terminate(ren::terminate);
    ren::set_window_hints();

    // init window

    auto window = ren::create_window("Pong");
    if (!window.get_pointer()) {
        std::fprintf(stderr, "Error: Failed to create window.\n");
        exit(static_cast<int>(ExitCodes::WindowInitFailed));
    }
    window.make_current();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    ren::set_clear_color(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2],
        CLEAR_COLOR[3]);
    ren::set_swap_interval(vsync_enabled);

    // init shader programs

    auto vertex_shader = []() -> decltype(auto) {
        try {
            return ren::Shader::from_file("./assets/shaders/white.vert",
                GL_VERTEX_SHADER);
        } catch (std::runtime_error e) {
            std::fprintf(stderr, "Error: Failed to initialize shaders.\n");
            exit(static_cast<int>(ExitCodes::ShadersInitFailed));
        }
    }();
    auto fragment_shader = []() -> decltype(auto) {
        try {
            return ren::Shader::from_file("./assets/shaders/white.frag",
                GL_FRAGMENT_SHADER);
        } catch (std::runtime_error e) {
            std::fprintf(stderr, "Error: Failed to initialize shaders.\n");
            exit(static_cast<int>(ExitCodes::ShadersInitFailed));
        }
    }();
    auto ball_shader = []() -> decltype(auto) {
        try {
            return ren::Shader::from_file("./assets/shaders/ball.frag",
                GL_FRAGMENT_SHADER);
        } catch (std::runtime_error e) {
            std::fprintf(stderr, "Error: Failed to initialize shaders.\n");
            exit(static_cast<int>(ExitCodes::ShadersInitFailed));
        }
    }();

    ren::ShaderProgram shader_program;
    shader_program.attach(vertex_shader);
    shader_program.attach(fragment_shader);
    try {
        shader_program.link();
    } catch (std::runtime_error e) {
        std::fprintf(stderr, "Error: Failed to initialize shaders.\n");
        exit(static_cast<int>(ExitCodes::ShadersInitFailed));
    }

    ren::ShaderProgram ball_program;
    ball_program.attach(vertex_shader);
    ball_program.attach(ball_shader);
    try {
        ball_program.link();
    } catch (std::runtime_error e) {
        std::fprintf(stderr, "Error: Failed to initialize shaders.\n");
        exit(static_cast<int>(ExitCodes::ShadersInitFailed));
    }

    auto aspect_location = shader_program.get_uniform_location("aspect");
    auto aspect_location_ball = ball_program.get_uniform_location("aspect");
    auto resolution_location = ball_program.get_uniform_location("u_resolution");
    auto center_location = ball_program.get_uniform_location("u_center");
    auto radius_location = ball_program.get_uniform_location("u_radius");
    if (aspect_location == -1 || aspect_location_ball == -1 || resolution_location == -1 || center_location == -1 || radius_location == -1) {
        std::fprintf(stderr, "Error: Failed to get uniform location.\n");
        exit(static_cast<int>(ExitCodes::UniformGetLocationFailed));
    }

    ball_program.set_uniform(radius_location, BALL_SIZE.x / 2);

    // instanciate meshes

    Game game {};
    window.add_listener(&game, &handle_key);

    unsigned int bat_transform_buffer = ren::instanciate(&game.bat_mesh, &game.bat_transforms);
    unsigned int ball_transform_buffer = ren::instanciate(&game.ball_mesh, &game.ball_transforms);

    // finalize init

    char window_title[32];
    utils::FrameCounter frame_counter(1);

    // main loop

    while (!window.sould_close()) {
        // calculate framerate and get delta

        auto frame_info = frame_counter.get_frame_info();

        // update game state

        game.update(frame_info.delta_seconds);

        // update title every second

        if (frame_info.has_changed) {
            std::snprintf(window_title, sizeof(window_title),
                WINDOW_TITLE_FORMAT_STRING, frame_info.frame_rate,
                game.scores[0], game.scores[1]);
            window.set_title(window_title);
        }

        // clear > draw

        ren::clear();
        shader_program.use();
        shader_program.set_uniform(aspect_location, window.get_aspect_ratio());
        ren::draw_instances(&game.bat_mesh, bat_transform_buffer);

        ball_program.use();
        ball_program.set_uniform(aspect_location_ball, window.get_aspect_ratio());
        ball_program.set_uniform(resolution_location, window.get_size());
        ball_program.set_uniform(center_location, glm::vec2(game.ball_transforms[0][3]));
        ren::draw_instances(&game.ball_mesh, ball_transform_buffer);

        // swap buffers

        window.swap_buffers();

        // poll events

        ren::poll_events();
    }
    return 0;
}
