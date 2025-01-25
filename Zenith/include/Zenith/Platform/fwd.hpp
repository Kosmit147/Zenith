#pragma once

#include "gl/fwd.hpp"

#include "zenith/core/typedefs.hpp"

namespace zth {

enum class EventType : u8;
enum class EventCategory : u8;
class Event;

class EventQueue;

struct WindowResizedEvent;
struct KeyPressedEvent;
struct KeyReleasedEvent;
struct MouseButtonPressedEvent;
struct MouseButtonReleasedEvent;
struct MouseMovedEvent;
struct MouseWheelScrolledEvent;

class Input;

enum class Key : u16;

enum class MouseButton : u8;

struct LineInfo;
struct PreprocessShaderError;
class ShaderPreprocessor;

struct WindowAspectRatio;
struct WindowSpec;
class Window;

} // namespace zth
