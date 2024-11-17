#include "Zenith/Platform/Input.hpp"

namespace zth {

auto Input::init() -> void
{
    for (auto key : key_enumerations)
        _key_states[key] = false;
}

auto Input::shut_down() -> void {}

auto Input::is_key_pressed(Key key) -> bool
{
    return _key_states[key];
}

auto Input::set_key_pressed(Key key, bool value) -> void
{
    _key_states[key] = value;
}

} // namespace zth
