#pragma once

#include "zenith/core/typedefs.hpp"
#include "zenith/layer/layer.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/memory/managed.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/system/window.hpp"
#include "zenith/util/reference.hpp"
#include "zenith/util/result.hpp"

namespace zth {

struct ApplicationSpec
{
    WindowSpec window_spec{};
    LoggerSpec logger_spec{};
    double delta_time_limit = 1 / 30.0; // In seconds.
    double fixed_time_step = 1 / 60.0;  // In seconds.
};

class Application
{
public:
    static inline double delta_time_limit = 1 / 30.0;
    static inline double fixed_time_step = 1 / 60.0;

public:
    Application() = delete;

    [[nodiscard]] static auto init(const ApplicationSpec& spec = {}) -> Result<void, String>;

    static auto push_layer(UniquePtr<Layer>&& layer) -> Result<Reference<Layer>, String>;
    // Returns false if layer stack is empty and there are no layers left to remove.
    static auto pop_layer() -> bool;

    static auto push_overlay(UniquePtr<Layer>&& overlay) -> Result<Reference<Layer>, String>;
    // Returns false if overlay stack is empty and there are no overlays left to remove.
    static auto pop_overlay() -> bool;

    static auto run() -> void;

    [[nodiscard]] static auto time() -> double;
    [[nodiscard]] static auto delta_time() -> double;

private:
    static LayerStack _layers;
    static LayerStack _overlays;

    static inline usize _fixed_updates_performed = 0;

    static inline double _prev_frame_time = 0.0;
    static inline double _delta_time = 0.0;

private:
    static auto shut_down() -> void;

    static auto pop_all_layers() -> void;
    static auto pop_all_overlays() -> void;

    static auto start_frame() -> void;
    static auto dispatch_event(const Event& event) -> void;
    static auto fixed_update() -> void;
    static auto update() -> void;
    static auto render() -> void;
};

} // namespace zth
