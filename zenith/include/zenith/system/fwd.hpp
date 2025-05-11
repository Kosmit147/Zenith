#pragma once

#include "zenith/core/typedefs.hpp"

namespace zth {

struct ApplicationSpec;
class Application;

enum class EventType : u8;
enum class EventCategory : u8;
struct WindowResizedEvent;
struct KeyPressedEvent;
struct KeyReleasedEvent;
struct MouseButtonPressedEvent;
struct MouseButtonReleasedEvent;
struct MouseMovedEvent;
struct MouseWheelScrolledEvent;
class Event;

class EventQueue;

class Input;
enum class Key : u16;
enum class MouseButton : u8;

class TemporaryStorage;

class Time;

class Timer;

struct LineInfo;
struct PreprocessShaderError;
class ShaderPreprocessor;

struct WindowAspectRatio;
struct WindowSpec;
class Window;

} // namespace zth
