#include "Zenith/Core/SystemManager.hpp"

#include "Zenith/Core/Application.hpp"
#include "Zenith/Graphics/Renderer.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Event.hpp"
#include "Zenith/Platform/Input.hpp"
#include "Zenith/Time/Time.hpp"

// list of which systems depend on which other systems
// we need to make sure that they're initialized in the right order
//
// Time -> {}
// Logger -> {}
// Window -> {}
// Input -> { Window }
// Renderer -> {}

namespace zth {

auto SystemManager::init_systems(const ApplicationSpec& spec) -> void
{
    Time::init();
    Logger::init(spec.logger_spec);
    Window::init(spec.window_spec);
    Input::init();
    Renderer::init();
}

auto SystemManager::on_event(const Event& event) -> void
{
    switch (event.category())
    {
        using enum EventCategory;
    case WindowEvent:
        Renderer::on_window_event(event);
        break;
    case InputEvent:
        Input::on_input_event(event);
        break;
    }
}

auto SystemManager::on_update() -> void
{
    Time::on_update();
    Input::on_update();
}

auto SystemManager::shut_down_systems() -> void
{
    Renderer::shut_down();
    Input::shut_down();
    Window::shut_down();
    Logger::shut_down();
    Time::shut_down();
}

} // namespace zth
