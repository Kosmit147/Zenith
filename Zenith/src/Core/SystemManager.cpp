#include "Zenith/Core/SystemManager.hpp"

#include "Zenith/Core/Application.hpp"
#include "Zenith/Core/SceneManager.hpp"
#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Graphics/Renderer.hpp"
#include "Zenith/Graphics/ImGuiRenderer.hpp"
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
// ImGuiRenderer -> { Logger, Renderer, Window } 
// SceneManager -> { Logger }

namespace zth {

auto SystemManager::init_systems(const ApplicationSpec& spec) -> void
{
    auto add_system = [&]<typename InitLambda>(InitLambda init_func, SystemShutdownFunc shutdown_func) {
        init_func();
        _system_shutdown_funcs.push_back(shutdown_func);
    };

    try
    {
        add_system([&] { Logger::init(spec.logger_spec); }, Logger::shut_down);
        add_system([&] { Time::init(); }, Time::shut_down);
        add_system([&] { Window::init(spec.window_spec); }, Window::shut_down);
        add_system([&] { Input::init(); }, Input::shut_down);
        add_system([&] { Renderer::init(); }, Renderer::shut_down);
        add_system([&] { ImGuiRenderer::init(); }, ImGuiRenderer::shut_down);
        add_system([&] { SceneManager::init(); }, SceneManager::shut_down);

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

    SceneManager::on_event(event);
}

auto SystemManager::on_update() -> void
{
    // Start ImGui's next frame before updating other systems
    ImGuiRenderer::on_update();

    Time::on_update();
    Input::on_update();
    SceneManager::on_update();
}

auto SystemManager::on_render() -> void
{
    ImGuiRenderer::on_render();
    SceneManager::on_render();
}

auto SystemManager::shut_down_systems() -> void
{
    ZTH_CORE_INFO("[SystemManager] Shutting down all systems.");

    for (auto shutdown_func : _system_shutdown_funcs | std::views::reverse)
        shutdown_func();
}

} // namespace zth
