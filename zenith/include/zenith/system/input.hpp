#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <array>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"
#include "zenith/stl/map.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/result.hpp"

namespace zth {

class Input
{
public:
    Input() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto start_frame() -> void;
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
    // @speed: These shouldn't be hash maps.
    static UnorderedMap<Key, bool> _key_states;
    static UnorderedMap<MouseButton, bool> _mouse_button_states;

    static inline glm::vec2 _mouse_pos{ 0.0f, 0.0f };
    static inline glm::vec2 _mouse_pos_delta{ 0.0f, 0.0f };
    static inline glm::vec2 _mouse_scroll_delta{ 0.0f, 0.0f };

    static inline glm::vec2 _prev_mouse_pos{ 0.0f, 0.0f };
};

enum class Key : u16
{
    Space = GLFW_KEY_SPACE,
    Apostrophe = GLFW_KEY_APOSTROPHE,
    Comma = GLFW_KEY_COMMA,
    Minus = GLFW_KEY_MINUS,
    Period = GLFW_KEY_PERIOD,
    Slash = GLFW_KEY_SLASH,
    Num0 = GLFW_KEY_0,
    Num1 = GLFW_KEY_1,
    Num2 = GLFW_KEY_2,
    Num3 = GLFW_KEY_3,
    Num4 = GLFW_KEY_4,
    Num5 = GLFW_KEY_5,
    Num6 = GLFW_KEY_6,
    Num7 = GLFW_KEY_7,
    Num8 = GLFW_KEY_8,
    Num9 = GLFW_KEY_9,
    Semicolon = GLFW_KEY_SEMICOLON,
    Equal = GLFW_KEY_EQUAL,
    A = GLFW_KEY_A,
    B = GLFW_KEY_B,
    C = GLFW_KEY_C,
    D = GLFW_KEY_D,
    E = GLFW_KEY_E,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    I = GLFW_KEY_I,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_M,
    N = GLFW_KEY_N,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,
    Q = GLFW_KEY_Q,
    R = GLFW_KEY_R,
    S = GLFW_KEY_S,
    T = GLFW_KEY_T,
    U = GLFW_KEY_U,
    V = GLFW_KEY_V,
    W = GLFW_KEY_W,
    X = GLFW_KEY_X,
    Y = GLFW_KEY_Y,
    Z = GLFW_KEY_Z,
    LeftBracket = GLFW_KEY_LEFT_BRACKET,
    BackSlash = GLFW_KEY_BACKSLASH,
    RightBracket = GLFW_KEY_RIGHT_BRACKET,
    GraveAccent = GLFW_KEY_GRAVE_ACCENT,
    World1 = GLFW_KEY_WORLD_1,
    World2 = GLFW_KEY_WORLD_2,
    Escape = GLFW_KEY_ESCAPE,
    Enter = GLFW_KEY_ENTER,
    Tab = GLFW_KEY_TAB,
    Backspace = GLFW_KEY_BACKSPACE,
    Insert = GLFW_KEY_INSERT,
    Delete = GLFW_KEY_DELETE,
    Right = GLFW_KEY_RIGHT,
    Left = GLFW_KEY_LEFT,
    Down = GLFW_KEY_DOWN,
    Up = GLFW_KEY_UP,
    PageUp = GLFW_KEY_PAGE_UP,
    PageDown = GLFW_KEY_PAGE_DOWN,
    Home = GLFW_KEY_HOME,
    End = GLFW_KEY_END,
    CapsLock = GLFW_KEY_CAPS_LOCK,
    ScrollLock = GLFW_KEY_SCROLL_LOCK,
    NumLock = GLFW_KEY_NUM_LOCK,
    PrintScreen = GLFW_KEY_PRINT_SCREEN,
    Pause = GLFW_KEY_PAUSE,
    F1 = GLFW_KEY_F1,
    F2 = GLFW_KEY_F2,
    F3 = GLFW_KEY_F3,
    F4 = GLFW_KEY_F4,
    F5 = GLFW_KEY_F5,
    F6 = GLFW_KEY_F6,
    F7 = GLFW_KEY_F7,
    F8 = GLFW_KEY_F8,
    F9 = GLFW_KEY_F9,
    F10 = GLFW_KEY_F10,
    F11 = GLFW_KEY_F11,
    F12 = GLFW_KEY_F12,
    F13 = GLFW_KEY_F13,
    F14 = GLFW_KEY_F14,
    F15 = GLFW_KEY_F15,
    F16 = GLFW_KEY_F16,
    F17 = GLFW_KEY_F17,
    F18 = GLFW_KEY_F18,
    F19 = GLFW_KEY_F19,
    F20 = GLFW_KEY_F20,
    F21 = GLFW_KEY_F21,
    F22 = GLFW_KEY_F22,
    F23 = GLFW_KEY_F23,
    F24 = GLFW_KEY_F24,
    F25 = GLFW_KEY_F25,
    Keypad0 = GLFW_KEY_KP_0,
    Keypad1 = GLFW_KEY_KP_1,
    Keypad2 = GLFW_KEY_KP_2,
    Keypad3 = GLFW_KEY_KP_3,
    Keypad4 = GLFW_KEY_KP_4,
    Keypad5 = GLFW_KEY_KP_5,
    Keypad6 = GLFW_KEY_KP_6,
    Keypad7 = GLFW_KEY_KP_7,
    Keypad8 = GLFW_KEY_KP_8,
    Keypad9 = GLFW_KEY_KP_9,
    KeypadDecimal = GLFW_KEY_KP_DECIMAL,
    KeypadDivide = GLFW_KEY_KP_DIVIDE,
    KeypadMultiply = GLFW_KEY_KP_MULTIPLY,
    KeypadSubtract = GLFW_KEY_KP_SUBTRACT,
    KeypadAdd = GLFW_KEY_KP_ADD,
    KeypadEnter = GLFW_KEY_KP_ENTER,
    KeypadEqual = GLFW_KEY_KP_EQUAL,
    LeftShift = GLFW_KEY_LEFT_SHIFT,
    LeftControl = GLFW_KEY_LEFT_CONTROL,
    LeftAlt = GLFW_KEY_LEFT_ALT,
    LeftSuper = GLFW_KEY_LEFT_SUPER,
    RightShift = GLFW_KEY_RIGHT_SHIFT,
    RightControl = GLFW_KEY_RIGHT_CONTROL,
    RightAlt = GLFW_KEY_RIGHT_ALT,
    RightSuper = GLFW_KEY_RIGHT_SUPER,
    Menu = GLFW_KEY_MENU,
};

enum class MouseButton : u8
{
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Middle = GLFW_MOUSE_BUTTON_MIDDLE,
    Right = GLFW_MOUSE_BUTTON_RIGHT,
};

constexpr std::array key_enumerations = {
    Key::Space,
    Key::Apostrophe,
    Key::Comma,
    Key::Minus,
    Key::Period,
    Key::Slash,
    Key::Num0,
    Key::Num1,
    Key::Num2,
    Key::Num3,
    Key::Num4,
    Key::Num5,
    Key::Num6,
    Key::Num7,
    Key::Num8,
    Key::Num9,
    Key::Semicolon,
    Key::Equal,
    Key::A,
    Key::B,
    Key::C,
    Key::D,
    Key::E,
    Key::F,
    Key::G,
    Key::H,
    Key::I,
    Key::J,
    Key::K,
    Key::L,
    Key::M,
    Key::N,
    Key::O,
    Key::P,
    Key::Q,
    Key::R,
    Key::S,
    Key::T,
    Key::U,
    Key::V,
    Key::W,
    Key::X,
    Key::Y,
    Key::Z,
    Key::LeftBracket,
    Key::BackSlash,
    Key::RightBracket,
    Key::GraveAccent,
    Key::World1,
    Key::World2,
    Key::Escape,
    Key::Enter,
    Key::Tab,
    Key::Backspace,
    Key::Insert,
    Key::Delete,
    Key::Right,
    Key::Left,
    Key::Down,
    Key::Up,
    Key::PageUp,
    Key::PageDown,
    Key::Home,
    Key::End,
    Key::CapsLock,
    Key::ScrollLock,
    Key::NumLock,
    Key::PrintScreen,
    Key::Pause,
    Key::F1,
    Key::F2,
    Key::F3,
    Key::F4,
    Key::F5,
    Key::F6,
    Key::F7,
    Key::F8,
    Key::F9,
    Key::F10,
    Key::F11,
    Key::F12,
    Key::F13,
    Key::F14,
    Key::F15,
    Key::F16,
    Key::F17,
    Key::F18,
    Key::F19,
    Key::F20,
    Key::F21,
    Key::F22,
    Key::F23,
    Key::F24,
    Key::F25,
    Key::Keypad0,
    Key::Keypad1,
    Key::Keypad2,
    Key::Keypad3,
    Key::Keypad4,
    Key::Keypad5,
    Key::Keypad6,
    Key::Keypad7,
    Key::Keypad8,
    Key::Keypad9,
    Key::KeypadDecimal,
    Key::KeypadDivide,
    Key::KeypadMultiply,
    Key::KeypadSubtract,
    Key::KeypadAdd,
    Key::KeypadEnter,
    Key::KeypadEqual,
    Key::LeftShift,
    Key::LeftControl,
    Key::LeftAlt,
    Key::LeftSuper,
    Key::RightShift,
    Key::RightControl,
    Key::RightAlt,
    Key::RightSuper,
    Key::Menu,
};

constexpr std::array mouse_button_enumerations = {
    MouseButton::Left,
    MouseButton::Middle,
    MouseButton::Right,
};

[[nodiscard]] auto glfw_key_to_key(int key) -> Optional<Key>;
[[nodiscard]] auto glfw_mouse_button_to_mouse_button(int button) -> Optional<MouseButton>;

[[nodiscard]] auto to_string(Key key) -> const char*;
[[nodiscard]] auto to_string(MouseButton button) -> const char*;

} // namespace zth

ZTH_DECLARE_FORMATTER(zth::Key);
ZTH_DECLARE_FORMATTER(zth::MouseButton);
