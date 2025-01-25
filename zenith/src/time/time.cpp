#include "zenith/time/time.hpp"

#include "zenith/platform/window.hpp"
#include "zenith/logging/logger.hpp"

namespace zth {

auto Time::init() -> void
{
    ZTH_CORE_INFO("Time system initialized.");
}

auto Time::on_update() -> void
{
    auto new_time = Window::time();
    _delta_time = std::min(new_time - _time, delta_time_limit);
    _time = new_time;
}

auto Time::shut_down() -> void
{
    ZTH_CORE_INFO("Time system shut down.");
}

} // namespace zth
