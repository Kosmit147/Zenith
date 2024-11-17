#pragma once

#include <unordered_map>

#include "Zenith/Platform/Key.hpp"

namespace zth {

class Input
{
public:
    static auto init() -> void;
    static auto shut_down() -> void;

    static auto is_key_pressed(Key key) -> bool;
    static auto set_key_pressed(Key key, bool value) -> void;

private:
    static inline std::unordered_map<Key, bool> _key_states;
};

} // namespace zth
