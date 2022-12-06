#include "timer.h"

namespace utils {
Timer::Timer()
    : old_time(std::chrono::high_resolution_clock::now())
    , new_time(old_time)
{
}
float Timer::get_delta_seconds()
{
    old_time = new_time;
    new_time = std::chrono::high_resolution_clock::now();
    return (float)(new_time - old_time).count() / 1000000000;
}
}
