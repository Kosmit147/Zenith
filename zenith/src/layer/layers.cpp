#include "zenith/layer/layers.hpp"

#include "zenith/asset/asset.hpp"
#include "zenith/core/profiler.hpp"
#include "zenith/core/random.hpp"
#include "zenith/core/scene.hpp"
#include "zenith/gl/context.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/renderer/imgui_renderer.hpp"
#include "zenith/renderer/renderer.hpp"
#include "zenith/renderer/shader_preprocessor.hpp"
#include "zenith/system/event.hpp"
#include "zenith/system/temporary_storage.hpp"
#include "zenith/util/defer.hpp"

namespace zth {

// --- System Layer
// 1. Logger
// 2. TemporaryStorage
// 3. Window
// 4. gl::Context
// 5. Input

SystemLayer::SystemLayer(const LoggerSpec& logger_spec, const WindowSpec& window_spec, usize temporary_storage_capacity)
    : _logger_spec{ logger_spec }, _window_spec{ window_spec },
      _temporary_storage_capacity{ temporary_storage_capacity }
{}

auto SystemLayer::on_frame_start() -> void
{
    ZTH_PROFILE_FUNCTION();

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

    result = TemporaryStorage::init(_temporary_storage_capacity);
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
    Logger::shut_down();
}

// --- Runtime Layer
// 1. Random
// 2. ShaderPreprocessor
// 3. AssetManager
// 4. Renderer
// 5. Renderer2D
// 6. SceneManager

auto RuntimeLayer::render() -> void
{
    ZTH_PROFILE_FUNCTION();

    SceneManager::render();
}

auto RuntimeLayer::on_frame_start() -> void
{
    ZTH_PROFILE_FUNCTION();

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

auto RuntimeLayer::on_fixed_update() -> void
{
    ZTH_PROFILE_FUNCTION();

    SceneManager::fixed_update();
}

auto RuntimeLayer::on_update() -> void
{
    ZTH_PROFILE_FUNCTION();

    SceneManager::update();
}

auto RuntimeLayer::on_render() -> void
{
    ZTH_PROFILE_FUNCTION();

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

    result = SceneManager::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_scene_manager{ [] { SceneManager::shut_down(); } };

    shut_down_random.dismiss();
    shut_down_shader_preprocessor.dismiss();
    shut_down_asset_manager.dismiss();
    shut_down_renderer.dismiss();
    shut_down_renderer_2d.dismiss();
    shut_down_scene_manager.dismiss();

    ZTH_INTERNAL_TRACE("Runtime layer initialized...");
    return {};
}

auto RuntimeLayer::on_detach() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down runtime layer...");

    // Shut down order should be the reverse of initialization order.
    SceneManager::shut_down();
    Renderer2D::shut_down();
    Renderer::shut_down();
    AssetManager::shut_down();
    ShaderPreprocessor::shut_down();
    Random::shut_down();

    ZTH_INTERNAL_TRACE("Runtime layer shut down.");
}

// --- ImGui Overlay
// 1. ImGuiRenderer

auto ImGuiOverlay::render() -> void
{
    ZTH_PROFILE_FUNCTION();

    ImGuiRenderer::render();
}

auto ImGuiOverlay::on_frame_start() -> void
{
    ZTH_PROFILE_FUNCTION();

    ImGuiRenderer::start_frame();
}

auto ImGuiOverlay::on_attach() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing ImGui overlay...");

    auto result = ImGuiRenderer::init();
    if (!result)
        return Error{ result.error() };
    Defer shut_down_imgui_renderer{ [] { ImGuiRenderer::shut_down(); } };

    shut_down_imgui_renderer.dismiss();

    ZTH_INTERNAL_TRACE("ImGui overlay initialized...");
    return {};
}

auto ImGuiOverlay::on_detach() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down ImGui overlay...");
    ImGuiRenderer::shut_down();
    ZTH_INTERNAL_TRACE("ImGui overlay shut down.");
}

// --- Debug Overlay

auto DebugOverlay::on_event(const Event& event) -> void
{
    if (event.type() == EventType::KeyPressed)
        on_key_pressed_event(event.key_pressed_event());
}

auto DebugOverlay::on_update() -> void
{
    ZTH_PROFILE_FUNCTION();

    if (_scene_hierarchy_panel_open)
        _scene_hierarchy_panel.display(SceneManager::scene().registry(), _scene_hierarchy_panel_open);

    if (_asset_browser_open)
        _asset_browser.display(_asset_browser_open);

    if (_debug_panel_open)
        _debug_panel.display(_debug_panel_open);

#if defined(ZTH_PROFILER)
    if (_profiler_open)
        Profiler::display(_profiler_open);
#endif
}

auto DebugOverlay::on_attach() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing debug overlay...");
    ZTH_INTERNAL_TRACE("Debug overlay initialized...");
    return {};
}

auto DebugOverlay::on_detach() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down debug overlay...");
    ZTH_INTERNAL_TRACE("Debug overlay shut down.");
}

auto DebugOverlay::on_key_pressed_event(const KeyPressedEvent& event) -> void
{
    auto [key] = event;

    if (key == toggle_cursor_key)
        Window::set_cursor_enabled(!Window::cursor_enabled());

    if (key == toggle_scene_hierarchy_panel_key)
        _scene_hierarchy_panel_open = !_scene_hierarchy_panel_open;

    if (_scene_hierarchy_panel_open)
    {
        using Operation = debug::GizmoOperation;
        using Mode = debug::GizmoMode;

        auto& inspector = _scene_hierarchy_panel.inspector;

        if (key == switch_to_gizmo_translate_mode_key)
        {
            inspector.gizmo.operation = Operation::Translate;
            inspector.gizmo.mode = Mode::World;
        }

        if (key == switch_to_gizmo_rotate_mode_key)
        {
            inspector.gizmo.operation = Operation::Rotate;
            inspector.gizmo.mode = Mode::Local;
        }

        if (key == switch_to_gizmo_scale_mode_key)
        {
            inspector.gizmo.operation = Operation::Scale;
            inspector.gizmo.mode = Mode::Local;
        }
    }

    if (key == toggle_asset_browser_key)
        _asset_browser_open = !_asset_browser_open;

    if (key == toggle_debug_panel_key)
        _debug_panel_open = !_debug_panel_open;

#if defined(ZTH_PROFILER)
    if (key == toggle_profiler_key)
        _profiler_open = !_profiler_open;
#endif
}

} // namespace zth
