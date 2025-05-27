#include "zenith/system/time.hpp"

#include "zenith/log/logger.hpp"
#include "zenith/system/window.hpp"

namespace zth {

auto Time::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Time system initialized.");
    return {};
}

auto Time::start_frame() -> void
{
    auto time = get_time();
    _delta_time = std::min(time - _prev_time, delta_time_limit);
    _prev_time = time;
}

auto Time::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Time system shut down.");
}

auto Time::get_time() -> double
{
    return Window::time();
}

} // namespace zth
