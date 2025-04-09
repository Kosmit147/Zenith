#include "zenith/system/input.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/system/event.hpp"
#include "zenith/system/window.hpp"

namespace zth {

std::unordered_map<Key, bool> Input::_key_states;
std::unordered_map<MouseButton, bool> Input::_mouse_button_states;

auto Input::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing input system...");

    for (auto key : key_enumerations)
        _key_states.insert({ key, false });

    for (auto button : mouse_button_enumerations)
        _mouse_button_states.insert({ button, false });

    // A solution for sudden movements caused by the first read of mouse position.
    _mouse_pos = Window::mouse_pos();
    _prev_mouse_pos = Window::mouse_pos();

    ZTH_INTERNAL_TRACE("Input system initialized.");
    return {};
}

auto Input::start_frame() -> void
{
    _prev_mouse_pos = _mouse_pos;
    _mouse_pos_delta = { 0.0f, 0.0f };
    _mouse_scroll_delta = { 0.0f, 0.0f };
}

auto Input::on_input_event(const Event& event) -> void
{
    ZTH_ASSERT(event.category() == EventCategory::InputEvent);

    switch (event.type())
    {
        using enum EventType;
    case KeyPressed:
    {
        auto [key] = event.key_pressed_event();
        set_key_pressed(key, true);
    }
    break;
    case KeyReleased:
    {
        auto [key] = event.key_released_event();
        set_key_pressed(key, false);
    }
    break;
    case MouseButtonPressed:
    {
        auto [button] = event.mouse_button_pressed_event();
        set_mouse_button_pressed(button, true);
    }
    break;
    case MouseButtonReleased:
    {
        auto [button] = event.mouse_button_released_event();
        set_mouse_button_pressed(button, false);
    }
    break;
    case MouseMoved:
    {
        auto [new_pos] = event.mouse_moved_event();
        set_mouse_pos(new_pos);
    }
    break;
    case MouseWheelScrolled:
    {
        auto [delta] = event.mouse_wheel_scrolled_event();
        set_mouse_scroll_delta(_mouse_scroll_delta + delta);
    }
    break;
    default:
    {
        ZTH_ASSERT(false);
        std::unreachable();
    }
    }
}

auto Input::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Input system shut down.");
}

auto Input::is_key_pressed(Key key) -> bool
{
    return _key_states.at(key);
}

auto Input::set_key_pressed(Key key, bool value) -> void
{
    _key_states.insert_or_assign(key, value);
}

auto Input::is_mouse_button_pressed(MouseButton button) -> bool
{
    return _mouse_button_states.at(button);
}

auto Input::set_mouse_button_pressed(MouseButton button, bool value) -> void
{
    _mouse_button_states.insert_or_assign(button, value);
}

auto Input::mouse_pos() -> glm::vec2
{
    return _mouse_pos;
}

auto Input::mouse_pos_delta() -> glm::vec2
{
    return _mouse_pos_delta;
}

auto Input::mouse_scroll_delta() -> glm::vec2
{
    return _mouse_scroll_delta;
}

auto Input::set_mouse_pos(glm::vec2 value) -> void
{
    _mouse_pos = value;
    _mouse_pos_delta = _mouse_pos - _prev_mouse_pos;
}

auto Input::set_mouse_scroll_delta(glm::vec2 value) -> void
{
    _mouse_scroll_delta = value;
}

auto glfw_key_to_key(int key) -> Optional<Key>
{
    switch (key)
    {
        using enum Key;
    case GLFW_KEY_SPACE:
        return Space;
    case GLFW_KEY_APOSTROPHE:
        return Apostrophe;
    case GLFW_KEY_COMMA:
        return Comma;
    case GLFW_KEY_MINUS:
        return Minus;
    case GLFW_KEY_PERIOD:
        return Period;
    case GLFW_KEY_SLASH:
        return Slash;
    case GLFW_KEY_0:
        return Num0;
    case GLFW_KEY_1:
        return Num1;
    case GLFW_KEY_2:
        return Num2;
    case GLFW_KEY_3:
        return Num3;
    case GLFW_KEY_4:
        return Num4;
    case GLFW_KEY_5:
        return Num5;
    case GLFW_KEY_6:
        return Num6;
    case GLFW_KEY_7:
        return Num7;
    case GLFW_KEY_8:
        return Num8;
    case GLFW_KEY_9:
        return Num9;
    case GLFW_KEY_SEMICOLON:
        return Semicolon;
    case GLFW_KEY_EQUAL:
        return Equal;
    case GLFW_KEY_A:
        return A;
    case GLFW_KEY_B:
        return B;
    case GLFW_KEY_C:
        return C;
    case GLFW_KEY_D:
        return D;
    case GLFW_KEY_E:
        return E;
    case GLFW_KEY_F:
        return F;
    case GLFW_KEY_G:
        return G;
    case GLFW_KEY_H:
        return H;
    case GLFW_KEY_I:
        return I;
    case GLFW_KEY_J:
        return J;
    case GLFW_KEY_K:
        return K;
    case GLFW_KEY_L:
        return L;
    case GLFW_KEY_M:
        return M;
    case GLFW_KEY_N:
        return N;
    case GLFW_KEY_O:
        return O;
    case GLFW_KEY_P:
        return P;
    case GLFW_KEY_Q:
        return Q;
    case GLFW_KEY_R:
        return R;
    case GLFW_KEY_S:
        return S;
    case GLFW_KEY_T:
        return T;
    case GLFW_KEY_U:
        return U;
    case GLFW_KEY_V:
        return V;
    case GLFW_KEY_W:
        return W;
    case GLFW_KEY_X:
        return X;
    case GLFW_KEY_Y:
        return Y;
    case GLFW_KEY_Z:
        return Z;
    case GLFW_KEY_LEFT_BRACKET:
        return LeftBracket;
    case GLFW_KEY_BACKSLASH:
        return BackSlash;
    case GLFW_KEY_RIGHT_BRACKET:
        return RightBracket;
    case GLFW_KEY_GRAVE_ACCENT:
        return GraveAccent;
    case GLFW_KEY_WORLD_1:
        return World1;
    case GLFW_KEY_WORLD_2:
        return World2;
    case GLFW_KEY_ESCAPE:
        return Escape;
    case GLFW_KEY_ENTER:
        return Enter;
    case GLFW_KEY_TAB:
        return Tab;
    case GLFW_KEY_BACKSPACE:
        return Backspace;
    case GLFW_KEY_INSERT:
        return Insert;
    case GLFW_KEY_DELETE:
        return Delete;
    case GLFW_KEY_RIGHT:
        return Right;
    case GLFW_KEY_LEFT:
        return Left;
    case GLFW_KEY_DOWN:
        return Down;
    case GLFW_KEY_UP:
        return Up;
    case GLFW_KEY_PAGE_UP:
        return PageUp;
    case GLFW_KEY_PAGE_DOWN:
        return PageDown;
    case GLFW_KEY_HOME:
        return Home;
    case GLFW_KEY_END:
        return End;
    case GLFW_KEY_CAPS_LOCK:
        return CapsLock;
    case GLFW_KEY_SCROLL_LOCK:
        return ScrollLock;
    case GLFW_KEY_NUM_LOCK:
        return NumLock;
    case GLFW_KEY_PRINT_SCREEN:
        return PrintScreen;
    case GLFW_KEY_PAUSE:
        return Pause;
    case GLFW_KEY_F1:
        return F1;
    case GLFW_KEY_F2:
        return F2;
    case GLFW_KEY_F3:
        return F3;
    case GLFW_KEY_F4:
        return F4;
    case GLFW_KEY_F5:
        return F5;
    case GLFW_KEY_F6:
        return F6;
    case GLFW_KEY_F7:
        return F7;
    case GLFW_KEY_F8:
        return F8;
    case GLFW_KEY_F9:
        return F9;
    case GLFW_KEY_F10:
        return F10;
    case GLFW_KEY_F11:
        return F11;
    case GLFW_KEY_F12:
        return F12;
    case GLFW_KEY_F13:
        return F13;
    case GLFW_KEY_F14:
        return F14;
    case GLFW_KEY_F15:
        return F15;
    case GLFW_KEY_F16:
        return F16;
    case GLFW_KEY_F17:
        return F17;
    case GLFW_KEY_F18:
        return F18;
    case GLFW_KEY_F19:
        return F19;
    case GLFW_KEY_F20:
        return F20;
    case GLFW_KEY_F21:
        return F21;
    case GLFW_KEY_F22:
        return F22;
    case GLFW_KEY_F23:
        return F23;
    case GLFW_KEY_F24:
        return F24;
    case GLFW_KEY_F25:
        return F25;
    case GLFW_KEY_KP_0:
        return Keypad0;
    case GLFW_KEY_KP_1:
        return Keypad1;
    case GLFW_KEY_KP_2:
        return Keypad2;
    case GLFW_KEY_KP_3:
        return Keypad3;
    case GLFW_KEY_KP_4:
        return Keypad4;
    case GLFW_KEY_KP_5:
        return Keypad5;
    case GLFW_KEY_KP_6:
        return Keypad6;
    case GLFW_KEY_KP_7:
        return Keypad7;
    case GLFW_KEY_KP_8:
        return Keypad8;
    case GLFW_KEY_KP_9:
        return Keypad9;
    case GLFW_KEY_KP_DECIMAL:
        return KeypadDecimal;
    case GLFW_KEY_KP_DIVIDE:
        return KeypadDivide;
    case GLFW_KEY_KP_MULTIPLY:
        return KeypadMultiply;
    case GLFW_KEY_KP_SUBTRACT:
        return KeypadSubtract;
    case GLFW_KEY_KP_ADD:
        return KeypadAdd;
    case GLFW_KEY_KP_ENTER:
        return KeypadEnter;
    case GLFW_KEY_KP_EQUAL:
        return KeypadEqual;
    case GLFW_KEY_LEFT_SHIFT:
        return LeftShift;
    case GLFW_KEY_LEFT_CONTROL:
        return LeftControl;
    case GLFW_KEY_LEFT_ALT:
        return LeftAlt;
    case GLFW_KEY_LEFT_SUPER:
        return LeftSuper;
    case GLFW_KEY_RIGHT_SHIFT:
        return RightShift;
    case GLFW_KEY_RIGHT_CONTROL:
        return RightControl;
    case GLFW_KEY_RIGHT_ALT:
        return RightAlt;
    case GLFW_KEY_RIGHT_SUPER:
        return RightSuper;
    case GLFW_KEY_MENU:
        return Menu;
    default:
        break;
    }

    return nil;
}

auto glfw_mouse_button_to_mouse_button(int button) -> Optional<MouseButton>
{
    switch (button)
    {
        using enum MouseButton;
    case GLFW_MOUSE_BUTTON_LEFT:
        return Left;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return Middle;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return Right;
    default:
        break;
    }

    return nil;
}

auto to_string(Key key) -> const char*
{
    switch (key)
    {
        using enum Key;
    case Space:
        return "Space";
    case Apostrophe:
        return "Apostrophe";
    case Comma:
        return "Comma";
    case Minus:
        return "Minus";
    case Period:
        return "Period";
    case Slash:
        return "Slash";
    case Num0:
        return "Num0";
    case Num1:
        return "Num1";
    case Num2:
        return "Num2";
    case Num3:
        return "Num3";
    case Num4:
        return "Num4";
    case Num5:
        return "Num5";
    case Num6:
        return "Num6";
    case Num7:
        return "Num7";
    case Num8:
        return "Num8";
    case Num9:
        return "Num9";
    case Semicolon:
        return "Semicolon";
    case Equal:
        return "Equal";
    case A:
        return "A";
    case B:
        return "B";
    case C:
        return "C";
    case D:
        return "D";
    case E:
        return "E";
    case F:
        return "F";
    case G:
        return "G";
    case H:
        return "H";
    case I:
        return "I";
    case J:
        return "J";
    case K:
        return "K";
    case L:
        return "L";
    case M:
        return "M";
    case N:
        return "N";
    case O:
        return "O";
    case P:
        return "P";
    case Q:
        return "Q";
    case R:
        return "R";
    case S:
        return "S";
    case T:
        return "T";
    case U:
        return "U";
    case V:
        return "V";
    case W:
        return "W";
    case X:
        return "X";
    case Y:
        return "Y";
    case Z:
        return "Z";
    case LeftBracket:
        return "LeftBracket";
    case BackSlash:
        return "BackSlash";
    case RightBracket:
        return "RightBracket";
    case GraveAccent:
        return "GraveAccent";
    case World1:
        return "World1";
    case World2:
        return "World2";
    case Escape:
        return "Escape";
    case Enter:
        return "Enter";
    case Tab:
        return "Tab";
    case Backspace:
        return "Backspace";
    case Insert:
        return "Insert";
    case Delete:
        return "Delete";
    case Right:
        return "Right";
    case Left:
        return "Left";
    case Down:
        return "Down";
    case Up:
        return "Up";
    case PageUp:
        return "PageUp";
    case PageDown:
        return "PageDown";
    case Home:
        return "Home";
    case End:
        return "End";
    case CapsLock:
        return "CapsLock";
    case ScrollLock:
        return "ScrollLock";
    case NumLock:
        return "NumLock";
    case PrintScreen:
        return "PrintScreen";
    case Pause:
        return "Pause";
    case F1:
        return "F1";
    case F2:
        return "F2";
    case F3:
        return "F3";
    case F4:
        return "F4";
    case F5:
        return "F5";
    case F6:
        return "F6";
    case F7:
        return "F7";
    case F8:
        return "F8";
    case F9:
        return "F9";
    case F10:
        return "F10";
    case F11:
        return "F11";
    case F12:
        return "F12";
    case F13:
        return "F13";
    case F14:
        return "F14";
    case F15:
        return "F15";
    case F16:
        return "F16";
    case F17:
        return "F17";
    case F18:
        return "F18";
    case F19:
        return "F19";
    case F20:
        return "F20";
    case F21:
        return "F21";
    case F22:
        return "F22";
    case F23:
        return "F23";
    case F24:
        return "F24";
    case F25:
        return "F25";
    case Keypad0:
        return "Keypad0";
    case Keypad1:
        return "Keypad1";
    case Keypad2:
        return "Keypad2";
    case Keypad3:
        return "Keypad3";
    case Keypad4:
        return "Keypad4";
    case Keypad5:
        return "Keypad5";
    case Keypad6:
        return "Keypad6";
    case Keypad7:
        return "Keypad7";
    case Keypad8:
        return "Keypad8";
    case Keypad9:
        return "Keypad9";
    case KeypadDecimal:
        return "KeypadDecimal";
    case KeypadDivide:
        return "KeypadDivide";
    case KeypadMultiply:
        return "KeypadMultiply";
    case KeypadSubtract:
        return "KeypadSubtract";
    case KeypadAdd:
        return "KeypadAdd";
    case KeypadEnter:
        return "KeypadEnter";
    case KeypadEqual:
        return "KeypadEqual";
    case LeftShift:
        return "LeftShift";
    case LeftControl:
        return "LeftControl";
    case LeftAlt:
        return "LeftAlt";
    case LeftSuper:
        return "LeftSuper";
    case RightShift:
        return "RightShift";
    case RightControl:
        return "RightControl";
    case RightAlt:
        return "RightAlt";
    case RightSuper:
        return "RightSuper";
    case Menu:
        return "Menu";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

auto to_string(MouseButton button) -> const char*
{
    switch (button)
    {
        using enum MouseButton;
    case Left:
        return "Left";
    case Middle:
        return "Middle";
    case Right:
        return "Right";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::Key, key)
{
    return ZTH_FORMAT_OUT("{}", zth::to_string(key));
}

ZTH_DEFINE_FORMATTER(zth::MouseButton, button)
{
    return ZTH_FORMAT_OUT("{}", zth::to_string(button));
}
