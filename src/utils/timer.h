#pragma once

#include <chrono>

namespace utils
{
    /// @brief Keeps track of time passed between calls
    class Timer
    {
        /// @brief The time at last call
        std::chrono::steady_clock::time_point old_time;
        /// @brief The time at current call
        std::chrono::steady_clock::time_point new_time;

    public:
        Timer();
        /// @return The time that passed since last call in seconds
        float get_delta_seconds();
    };
}