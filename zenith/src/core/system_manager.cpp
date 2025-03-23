#include "zenith/core/system_manager.hpp"

#include "zenith/asset/asset_manager.hpp"
#include "zenith/core/random.hpp"
#include "zenith/core/scene_manager.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/memory/temporary_storage.hpp"
#include "zenith/renderer/imgui_renderer.hpp"
#include "zenith/renderer/renderer.hpp"
#include "zenith/renderer/shader_preprocessor.hpp"
#include "zenith/system/application.hpp"
#include "zenith/system/event.hpp"
#include "zenith/system/input.hpp"
#include "zenith/system/time.hpp"

// A list describing which systems depend on which other systems. We need to make sure that they're initialized in the
// right order. Nearly every system depends on Logger and on TemporaryStorage, so we don't list those.
//
// Logger -> {}
// TemporaryStorage -> {}
// Time -> {}
// Window -> {}
// Random -> {}
// Input -> { Window }
// ShaderPreprocessor -> {}
// AssetManager -> { ShaderPreprocessor }
// Renderer -> { Window, ShaderPreprocessor }
// ImGuiRenderer -> { Renderer, Window }
// SceneManager -> {}

namespace zth {

Vector<SystemManager::SystemShutdownFunc> SystemManager::_system_shutdown_funcs;

auto SystemManager::init_systems(const ApplicationSpec& spec) -> void
{
    auto add_system = [&]<typename InitFunc>(InitFunc init_func, SystemShutdownFunc shutdown_func) {
        init_func();
        _system_shutdown_funcs.push_back(shutdown_func);
    };

    try
    {
        add_system([&] { Logger::init(spec.logger_spec); }, Logger::shut_down);
        add_system([&] { TemporaryStorage::init(); }, TemporaryStorage::shut_down);
        add_system([&] { Time::init(); }, Time::shut_down);
        add_system([&] { Window::init(spec.window_spec); }, Window::shut_down);
        add_system([&] { Random::init(); }, Random::shut_down);
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

auto SystemManager::dispatch_event(const Event& event) -> void
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

    SceneManager::dispatch_event(event);
}

auto SystemManager::update() -> void
{
    // Start ImGui's next frame before updating other systems.
    ImGuiRenderer::update();

    // Clear temporary storage before updating other systems.
    TemporaryStorage::update();

    Time::update();
    Input::update();
    SceneManager::update();
}

auto SystemManager::on_render() -> void
{
    ImGuiRenderer::on_render();
    SceneManager::on_render();
}

auto SystemManager::shut_down_systems() -> void
{
    ZTH_CORE_INFO("[System Manager] Shutting down all systems...");

    for (auto shutdown_func : _system_shutdown_funcs | std::views::reverse)
        shutdown_func();
}

} // namespace zth
