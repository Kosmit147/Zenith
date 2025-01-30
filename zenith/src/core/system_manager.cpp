#include "zenith/core/system_manager.hpp"

#include "zenith/core/asset_manager.hpp"
#include "zenith/core/scene_manager.hpp"
#include "zenith/core/typedefs.hpp"
#include "zenith/graphics/imgui_renderer.hpp"
#include "zenith/graphics/renderer.hpp"
#include "zenith/graphics/shader_preprocessor.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/system/application.hpp"
#include "zenith/system/event.hpp"
#include "zenith/system/input.hpp"
#include "zenith/system/time.hpp"

// list of which systems depend on which other systems
// we need to make sure that they're initialized in the right order
//
// Logger -> {}
// Time -> { Logger }
// Window -> { Logger }
// Input -> { Logger, Window }
// ShaderPreprocessor -> { Logger }
// AssetManager -> { Logger, ShaderPreprocessor }
// Renderer -> { Logger, Window, ShaderPreprocessor }
// ImGuiRenderer -> { Logger, Renderer, Window }
// SceneManager -> { Logger }

namespace zth {

std::vector<SystemManager::SystemShutdownFunc> SystemManager::_system_shutdown_funcs;

auto SystemManager::init_systems(const ApplicationSpec& spec) -> void
{
    auto add_system = [&]<typename InitFunc>(InitFunc init_func, SystemShutdownFunc shutdown_func) {
        init_func();
        _system_shutdown_funcs.push_back(shutdown_func);
    };

    try
    {
        add_system([&] { Logger::init(spec.logger_spec); }, Logger::shut_down);
        add_system([&] { Time::init(); }, Time::shut_down);
        add_system([&] { Window::init(spec.window_spec); }, Window::shut_down);
        add_system([&] { Input::init(); }, Input::shut_down);
        add_system([&] { ShaderPreprocessor::init(); }, ShaderPreprocessor::shut_down);
        add_system([&] { AssetManager::init(); }, AssetManager::shut_down);
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
    // start ImGui's next frame before updating other systems
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
