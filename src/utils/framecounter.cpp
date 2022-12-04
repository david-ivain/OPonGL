
#include "framecounter.h"

namespace utils
{
    FrameCounter::FrameCounter(float interval_seconds) : timer{}, interval_seconds(interval_seconds), frame_count(0), sum_delta_seconds(0), frame_rate(0)
    {
    }

    FrameInfo FrameCounter::get_frame_info()
    {
        auto delta_seconds = timer.get_delta_seconds();
        bool has_changed = false;

        if (sum_delta_seconds < interval_seconds)
        {
            frame_count++;
            sum_delta_seconds += delta_seconds;
        }
        else
        {
            frame_rate = 1.0f / (sum_delta_seconds / frame_count);
            frame_count = 0;
            sum_delta_seconds = 0;
            has_changed = true;
        }
        return {frame_rate, has_changed, delta_seconds};
    }
}