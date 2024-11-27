#pragma once

#include <glm/vec2.hpp>

#include <unordered_map>

#include "Zenith/Platform/Key.hpp"

namespace zth {

class Input
{
public:
    static auto init() -> void;
    static auto shut_down() -> void;

    [[nodiscard]] static auto is_key_pressed(Key key) -> bool;
    static auto set_key_pressed(Key key, bool value) -> void;

    [[nodiscard]] static auto get_mouse_pos() -> glm::vec2;
    [[nodiscard]] static auto get_mouse_pos_delta() -> glm::vec2;

    // should be called every frame to update the difference between new and last position
    static auto set_mouse_pos(glm::vec2 value) -> void;

private:
    static inline std::unordered_map<Key, bool> _key_states;
    static inline glm::vec2 _mouse_pos = { 0.0f, 0.0f };
    static inline glm::vec2 _mouse_pos_delta = { 0.0f, 0.0f };
};

} // namespace zth
