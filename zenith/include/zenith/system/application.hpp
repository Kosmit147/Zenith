#pragma once

#include "zenith/core/typedefs.hpp"
#include "zenith/layer/layer.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/memory/managed.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/system/window.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/reference.hpp"
#include "zenith/util/result.hpp"

// This macro must be used in a single .cpp file (create_application() must be defined only once).
// user_application is the class derived from zth::Application defined by the user.
#define ZTH_IMPLEMENT_APP(user_application)                                                                            \
    namespace zth {                                                                                                    \
    [[nodiscard]] auto create_application() -> ::zth::UniquePtr<::zth::Application>                                    \
    {                                                                                                                  \
        return ::zth::make_unique<::user_application>();                                                               \
    }                                                                                                                  \
    }

namespace zth {

struct ApplicationSpec
{
    WindowSpec window_spec{};
    LoggerSpec logger_spec{};
    double fixed_update_time = 1 / 60.0; // In seconds.
};

class Application
{
public:
    double fixed_update_time;

public:
    explicit Application(const ApplicationSpec& spec = {});
    ZTH_NO_COPY_NO_MOVE(Application)
    virtual ~Application();

    auto push_layer(UniquePtr<Layer>&& layer) -> Result<Reference<Layer>, String>;
    // Returns false if layer stack is empty and there are no layers left to remove.
    auto pop_layer() -> bool;

    auto push_overlay(UniquePtr<Layer>&& overlay) -> Result<Reference<Layer>, String>;
    // Returns false if overlay stack is empty and there are no overlays left to remove.
    auto pop_overlay() -> bool;

    auto pop_all_layers() -> void;
    auto pop_all_overlays() -> void;

    auto run() -> void;

private:
    LayerStack _layers;
    LayerStack _overlays;

    usize _fixed_updates_performed = 0;

private:
    auto start_frame() -> void;
    auto dispatch_event(const Event& event) -> void;
    auto fixed_update() -> void;
    auto update() -> void;
    auto render() -> void;
};

} // namespace zth
