#pragma once

#include <glm/vec2.hpp>

#include <unordered_map>

#include "zenith/platform/key.hpp"
#include "zenith/platform/mouse.hpp"
#include "zenith/platform/fwd.hpp"

namespace zth {

class Input
{
public:
    Input() = delete;

    static auto init() -> void;
    static auto on_update() -> void;
    static auto on_input_event(const Event& event) -> void;
    static auto shut_down() -> void;

    [[nodiscard]] static auto is_key_pressed(Key key) -> bool;
    static auto set_key_pressed(Key key, bool value) -> void;

    [[nodiscard]] static auto is_mouse_button_pressed(MouseButton button) -> bool;
    static auto set_mouse_button_pressed(MouseButton button, bool value) -> void;

    [[nodiscard]] static auto mouse_pos() -> glm::vec2;
    [[nodiscard]] static auto mouse_pos_delta() -> glm::vec2;
    [[nodiscard]] static auto mouse_scroll_delta() -> glm::vec2;

    static auto set_mouse_pos(glm::vec2 value) -> void;
    static auto set_mouse_scroll_delta(glm::vec2 value) -> void;

private:
    static inline std::unordered_map<Key, bool> _key_states;
    static inline std::unordered_map<MouseButton, bool> _mouse_button_states;
    static inline glm::vec2 _mouse_pos = { 0.0f, 0.0f };
    static inline glm::vec2 _mouse_pos_delta = { 0.0f, 0.0f };
    static inline glm::vec2 _mouse_scroll_delta = { 0.0f, 0.0f };

    static inline glm::vec2 _prev_mouse_pos = { 0.0f, 0.0f };
    static inline bool _mouse_scroll_delta_updated = false;
};

} // namespace zth
