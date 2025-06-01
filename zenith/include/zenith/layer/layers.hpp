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

class DebugLayer : public Layer
{
public:
    Key toggle_debug_mode_key = Key::F1;

    Key switch_to_gizmo_translate_mode_key = Key::Q;
    Key switch_to_gizmo_rotate_mode_key = Key::E;
    Key switch_to_gizmo_scale_mode_key = Key::R;

    Key toggle_profiler_key = Key::F10;

public:
    explicit DebugLayer() = default;
    ZTH_NO_COPY_NO_MOVE(DebugLayer)
    ~DebugLayer() override = default;

    auto render() -> void override;

    auto on_frame_start() -> void override;
    auto on_event(const Event& event) -> void override;
    auto on_update() -> void override;

private:
    debug::SceneHierarchyPanel _scene_hierarchy_panel;
    debug::DebugPanel _debug_panel;

    bool _debug_mode_on = false;

#if defined(ZTH_PROFILER)
    bool _profiler_on = false;
#endif

    // This gets set if we enter debug mode and the cursor is not enabled. In that case we need to enable the cursor and
    // revert the change when we leave debug mode.
    bool _had_to_enable_cursor = false;

private:
    [[nodiscard]] auto on_attach() -> Result<void, String> override;
    auto on_detach() -> void override;

    auto on_key_pressed_event(const KeyPressedEvent& event) -> void;
};

} // namespace zth
