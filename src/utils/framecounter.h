#pragma once

#include "timer.h"

namespace utils {
/// @brief Represents the info of one frame retrieved from "FrameCounter::get_frame_info"
struct FrameInfo {
    /// @brief The last calculated frame rate
    float frame_rate;
    /// @brief Indicates if a new calculation has happened since the last call
    bool has_changed;
    /// @brief The delta in seconds since last calculation
    float delta_seconds;
};

/// @brief Manages framerate calculations based on delta between calls
class FrameCounter {
private:
    /// @brief The timer used to keep track of the time
    Timer timer;
    /// @brief The interval between new calculations
    const float interval_seconds;
    /// @brief The number of frames since last calculation
    unsigned int frame_count;
    /// @brief The delta in seconds since last calculation
    float sum_delta_seconds;
    /// @brief The last calculated frame rate
    float frame_rate;

public:
    /// @param interval_seconds The interval between new calculations
    FrameCounter(float interval_seconds);

    /// @brief If interval has passed since the last calculation updates the frame rate. Then gets frame info
    /// @return The info from the frame, including the frame rate and if it has been recalculated
    FrameInfo get_frame_info();
};
}
