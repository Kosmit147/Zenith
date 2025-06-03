#pragma once

#include "zenith/debug/ui.hpp"
#include "zenith/layer/layer.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/system/input.hpp"
#include "zenith/system/window.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/result.hpp"

namespace zth {

class SystemLayer : public Layer
{
public:
    explicit SystemLayer(const LoggerSpec& logger_spec, const WindowSpec& window_spec,
                         usize temporary_storage_capacity);
    ZTH_NO_COPY_NO_MOVE(SystemLayer)
    ~SystemLayer() override = default;

    auto on_frame_start() -> void override;
    auto on_event(const Event& event) -> void override;

private:
    LoggerSpec _logger_spec;
    WindowSpec _window_spec;
    usize _temporary_storage_capacity;

private:
    [[nodiscard]] auto on_attach() -> Result<void, String> override;
    auto on_detach() -> void override;
};

class RuntimeLayer : public Layer
{
public:
    explicit RuntimeLayer() = default;
    ZTH_NO_COPY_NO_MOVE(RuntimeLayer)
    ~RuntimeLayer() override = default;

    auto render() -> void override;

    auto on_frame_start() -> void override;
    auto on_event([[maybe_unused]] const Event& event) -> void override;
    auto on_fixed_update() -> void override;
    auto on_update() -> void override;
    auto on_render() -> void override;

private:
    [[nodiscard]] auto on_attach() -> Result<void, String> override;
    auto on_detach() -> void override;
};

class ImGuiOverlay : public Layer
{
public:
    explicit ImGuiOverlay() = default;
    ZTH_NO_COPY_NO_MOVE(ImGuiOverlay)
    ~ImGuiOverlay() override = default;

    auto render() -> void override;
    auto on_frame_start() -> void override;

private:
    [[nodiscard]] auto on_attach() -> Result<void, String> override;
    auto on_detach() -> void override;
};

class DebugOverlay : public Layer
{
public:
    Key toggle_cursor_key = Key::LeftControl;
    Key toggle_scene_hierarchy_panel_key = Key::F1;
    Key toggle_asset_browser_key = Key::F2;
    Key toggle_debug_panel_key = Key::F9;

#if defined(ZTH_PROFILER)
    Key toggle_profiler_key = Key::F10;
#endif

    Key switch_to_gizmo_translate_mode_key = Key::Q;
    Key switch_to_gizmo_rotate_mode_key = Key::E;
    Key switch_to_gizmo_scale_mode_key = Key::R;

public:
    explicit DebugOverlay() = default;
    ZTH_NO_COPY_NO_MOVE(DebugOverlay)
    ~DebugOverlay() override = default;

    auto on_event(const Event& event) -> void override;
    auto on_update() -> void override;

private:
    debug::SceneHierarchyPanel _scene_hierarchy_panel;
    debug::AssetBrowser _asset_browser;
    debug::DebugPanel _debug_panel;

    bool _scene_hierarchy_panel_open = false;
    bool _asset_browser_open = false;
    bool _debug_panel_open = false;

#if defined(ZTH_PROFILER)
    bool _profiler_open = false;
#endif

private:
    [[nodiscard]] auto on_attach() -> Result<void, String> override;
    auto on_detach() -> void override;

    auto on_key_pressed_event(const KeyPressedEvent& event) -> void;
};

} // namespace zth
