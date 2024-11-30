#include "Zenith/Core/SystemManager.hpp"

#include "Zenith/Core/Application.hpp"
#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Graphics/Renderer.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Event.hpp"
#include "Zenith/Platform/Input.hpp"
#include "Zenith/Time/Time.hpp"

// list of which systems depend on which other systems
// we need to make sure that they're initialized in the right order
//
// Logger -> {}
// Time -> { Logger }
// Window -> { Logger }
// Input -> { Logger, Window }
// Renderer -> { Logger, Window }

namespace zth {

namespace {

constexpr usize system_count = 5;

}

auto SystemManager::init_systems(const ApplicationSpec& spec) -> void
{
    _system_shutdown_funcs.reserve(system_count);

    auto add_system = [&]<typename InitLambda, typename ShutdownLambda>(InitLambda init_func,
                                                                        ShutdownLambda shutdown_func) {
        init_func();
        _system_shutdown_funcs.push_back(shutdown_func);
    };

    try
    {
        add_system([&] { Logger::init(spec.logger_spec); }, [] { Logger::shut_down(); });
        add_system([&] { Time::init(); }, [] { Time::shut_down(); });
        add_system([&] { Window::init(spec.window_spec); }, [] { Window::shut_down(); });
        add_system([&] { Input::init(); }, [] { Input::shut_down(); });
        add_system([&] { Renderer::init(); }, [] { Renderer::shut_down(); });

        ZTH_CORE_INFO("[System Manager] All systems initialized.");
    }
    catch (...)
    {
        shut_down_systems();
        throw;
    }
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
    ZTH_CORE_INFO("[SystemManager] Shutting down all systems.");

    for (auto shutdown_func : _system_shutdown_funcs | std::views::reverse)
        shutdown_func();
}

} // namespace zth
