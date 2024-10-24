#include "Input.hpp"

namespace zth {

static const char* key_names[] = {
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "Num1",
    "Num2",
    "Num3",
    "Num4",
    "Num5",
    "Num6",
    "Num7",
    "Num8",
    "Num9",
    "Num0",
    "Enter",
    "Escape",
    "Backspace",
    "Tab",
    "Space",
    "Hyphen",
    "Equal",
    "LBracket",
    "RBracket",
    "Backslash",
    "Semicolon",
    "Apostrophe",
    "Grave",
    "Comma",
    "Period",
    "Slash",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "CapsLock",
    "PrintScreen",
    "ScrollLock",
    "Pause",
    "Insert",
    "Home",
    "PageUp",
    "Delete",
    "End",
    "PageDown",
    "Right",
    "Left",
    "Down",
    "Up",
    "NumLock",
    "NumpadDivide",
    "NumpadMultiply",
    "NumpadMinus",
    "NumpadPlus",
    "NumpadEqual",
    "NumpadEnter",
    "NumpadDecimal",
    "Numpad1",
    "Numpad2",
    "Numpad3",
    "Numpad4",
    "Numpad5",
    "Numpad6",
    "Numpad7",
    "Numpad8",
    "Numpad9",
    "Numpad0",
    "NonUsBackslash",
    "Application",
    "Execute",
    "ModeChange",
    "Help",
    "Menu",
    "Select",
    "Redo",
    "Undo",
    "Cut",
    "Copy",
    "Paste",
    "VolumeMute",
    "VolumeUp",
    "VolumeDown",
    "MediaPlayPause",
    "MediaStop",
    "MediaNextTrack",
    "MediaPreviousTrack",
    "LControl",
    "LShift",
    "LAlt",
    "LSystem",
    "RControl",
    "RShift",
    "RAlt",
    "RSystem",
    "Back",
    "Forward",
    "Refresh",
    "Stop",
    "Search",
    "Favorites",
    "HomePage",
    "LaunchApplication1",
    "LaunchApplication2",
    "LaunchMail",
    "LaunchMediaSelect",
};

const char* to_string(Key key)
{
    return key_names[static_cast<usize>(key) + 1];
}

const char* to_string(MouseButton button)
{
    return button == MouseButton::Left ? "Left" : "Right";
}

} // namespace zth
