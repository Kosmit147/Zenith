#include "zenith/layer/layers.hpp"

#include <imgui.h>
#include <ImGuizmo.h>

#include "zenith/asset/asset.hpp"
#include "zenith/core/random.hpp"
#include "zenith/core/scene.hpp"
#include "zenith/gl/context.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/physics/physics.hpp"
#include "zenith/renderer/imgui_renderer.hpp"
#include "zenith/renderer/renderer.hpp"
#include "zenith/renderer/shader_preprocessor.hpp"
#include "zenith/system/event.hpp"
#include "zenith/system/temporary_storage.hpp"
#include "zenith/system/time.hpp"
#include "zenith/util/defer.hpp"

namespace zth {

// --- System Layer
// 1. Logger
// 2. Time
// 3. TemporaryStorage
// 4. Window
// 5. gl::Context
// 6. Input

SystemLayer::SystemLayer(const LoggerSpec& logger_spec, const WindowSpec& window_spec, const TimeSpec& time_spec)
    : _logger_spec{ logger_spec }, _window_spec{ window_spec }, _time_spec{ time_spec }
{}

auto SystemLayer::on_frame_start() -> void
{
    Time::start_frame();
    TemporaryStorage::start_frame();
    Input::start_frame();
}

auto SystemLayer::on_event(const Event& event) -> void
{
    if (event.category() == EventCategory::InputEvent)
        Input::on_input_event(event);
}

auto SystemLayer::on_attach() -> Result<void, String>
{
    auto result = Logger::init(_logger_spec);
    if (!result)
        return Error{ result.error() };
    Defer shut_down_logger{ [] { Logger::shut_down(); } };

    ZTH_INTERNAL_TRACE("Initializing system layer...");

    result = Time::init(_time_spec);
    if (!result)
        return Error{ result.error() };
    Defer shut_down_time{ [] { Time::shut_down(); } };

    result = TemporaryStorage::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_temporary_storage{ [] { TemporaryStorage::shut_down(); } };

    result = Window::init(_window_spec);
    if (!result)
        return Error{ result.error() };
    Defer shut_down_window{ [] { Window::shut_down(); } };

    result = gl::Context::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_gl_context{ [] { gl::Context::shut_down(); } };

    result = Input::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_input{ [] { Input::shut_down(); } };

    shut_down_logger.dismiss();
    shut_down_time.dismiss();
    shut_down_temporary_storage.dismiss();
    shut_down_window.dismiss();
    shut_down_gl_context.dismiss();
    shut_down_input.dismiss();

    ZTH_INTERNAL_TRACE("System layer initialized...");
    return {};
}

auto SystemLayer::on_detach() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down system layer...");

    // Shut down order should be the reverse of initialization order.
    Input::shut_down();
    gl::Context::shut_down();
    Window::shut_down();
    TemporaryStorage::shut_down();
    Time::shut_down();
    Logger::shut_down();
}

// --- Runtime Layer
// 1. Random
// 2. ShaderPreprocessor
// 3. AssetManager
// 4. Renderer
// 5. Renderer2D
// 6. Physics
// 7. SceneManager

RuntimeLayer::RuntimeLayer(const PhysicsSpec& physics_spec) : _physics_spec{ physics_spec } {}

auto RuntimeLayer::render() -> void
{
    SceneManager::render();
}

auto RuntimeLayer::on_frame_start() -> void
{
    Renderer::start_frame();
    Renderer2D::start_frame();
    SceneManager::start_frame();
}

auto RuntimeLayer::on_event(const Event& event) -> void
{
    if (event.category() == EventCategory::WindowEvent)
        Renderer::on_window_event(event);

    SceneManager::dispatch_event(event);
}

void RuntimeLayer::on_fixed_update()
{
    Physics::fixed_update();
    SceneManager::fixed_update();
}

auto RuntimeLayer::on_update() -> void
{
    SceneManager::update();
}

auto RuntimeLayer::on_render() -> void
{
    SceneManager::on_render();
}

auto RuntimeLayer::on_attach() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing runtime layer...");

    auto result = Random::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_random{ [] { Random::shut_down(); } };

    result = ShaderPreprocessor::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_shader_preprocessor{ [] { ShaderPreprocessor::shut_down(); } };

    result = AssetManager::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_asset_manager{ [] { AssetManager::shut_down(); } };

    result = Renderer::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_renderer{ [] { Renderer::shut_down(); } };

    result = Renderer2D::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_renderer_2d{ [] { Renderer2D::shut_down(); } };

    result = Physics::init(_physics_spec);
    if (!result)
        return Error{ result.error() };
    Defer shut_down_physics{ [] { Physics::shut_down(); } };

    result = SceneManager::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_scene_manager{ [] { SceneManager::shut_down(); } };

    shut_down_random.dismiss();
    shut_down_shader_preprocessor.dismiss();
    shut_down_asset_manager.dismiss();
    shut_down_renderer.dismiss();
    shut_down_renderer_2d.dismiss();
    shut_down_physics.dismiss();
    shut_down_scene_manager.dismiss();

    ZTH_INTERNAL_TRACE("Runtime layer initialized...");
    return {};
}

auto RuntimeLayer::on_detach() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down runtime layer...");

    // Shut down order should be the reverse of initialization order.
    SceneManager::shut_down();
    Physics::shut_down();
    Renderer2D::shut_down();
    Renderer::shut_down();
    AssetManager::shut_down();
    ShaderPreprocessor::shut_down();
    Random::shut_down();

    ZTH_INTERNAL_TRACE("Runtime layer shut down.");
}

// --- Debug Layer
// 1. ImGuiRenderer

auto DebugLayer::render() -> void
{
    ImGuiRenderer::render();
}

auto DebugLayer::on_frame_start() -> void
{
    ImGuiRenderer::start_frame();
}

auto DebugLayer::on_event(const Event& event) -> void
{
    if (event.type() == EventType::KeyPressed)
        on_key_pressed_event(event.key_pressed_event());
}

auto DebugLayer::on_update() -> void
{
    if (_debug_mode_on)
    {
        auto& scene = SceneManager::scene();
        _scene_hierarchy_panel.display(scene.registry());
        _debug_panel.display();
    }
}

auto DebugLayer::on_attach() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing debug layer...");

    auto result = ImGuiRenderer::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_imgui_renderer{ [] { ImGuiRenderer::shut_down(); } };

    shut_down_imgui_renderer.dismiss();

    ZTH_INTERNAL_TRACE("Debug layer initialized...");
    return {};
}

auto DebugLayer::on_detach() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down debug layer...");
    ImGuiRenderer::shut_down();
    ZTH_INTERNAL_TRACE("Debug layer shut down.");
}

auto DebugLayer::on_key_pressed_event(const KeyPressedEvent& event) -> void
{
    auto [key] = event;

    if (key == toggle_debug_mode_key)
    {
        if (!_debug_mode_on)
        {
            if (!Window::cursor_enabled())
            {
                Window::set_cursor_enabled(true);
                _had_to_enable_cursor = true;
            }

            _debug_mode_on = true;
        }
        else
        {
            if (_had_to_enable_cursor)
                Window::set_cursor_enabled(false);

            _debug_mode_on = false;
            _had_to_enable_cursor = false;
        }
    }

    if (_debug_mode_on)
    {
        auto& inspector = _scene_hierarchy_panel.inspector;

        if (key == switch_to_gizmo_translate_mode_key)
        {
            inspector.gizmo.operation = ImGuizmo::TRANSLATE;
            inspector.gizmo.mode = ImGuizmo::WORLD;
        }

        if (key == switch_to_gizmo_rotate_mode_key)
        {
            inspector.gizmo.operation = ImGuizmo::ROTATE;
            inspector.gizmo.mode = ImGuizmo::LOCAL;
        }

        if (key == switch_to_gizmo_scale_mode_key)
        {
            inspector.gizmo.operation = ImGuizmo::SCALE;
            inspector.gizmo.mode = ImGuizmo::LOCAL;
        }
    }
}

} // namespace zth
