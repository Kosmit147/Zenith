#include "Zenith/Time/Time.hpp"

#include "Zenith/Platform/Window.hpp"

namespace zth {

auto Time::init() -> void {}

auto Time::on_update() -> void
{
    auto new_time = Window::time();
    _delta_time = new_time - _time;
    _time = new_time;
}

auto Time::shut_down() -> void {}

} // namespace zth
