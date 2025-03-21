#pragma once

#include <memory>

#include "zenith/log/logger.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/system/window.hpp"
#include "zenith/util/macros.hpp"

// This macro must be used in a single .cpp file (create_application() must be defined only once).
// user_application is the class derived from zth::Application defined by the user.
#define ZTH_IMPLEMENT_APP(user_application)                                                                            \
    namespace zth {                                                                                                    \
    [[nodiscard]] auto create_application() -> std::unique_ptr<Application>                                            \
    {                                                                                                                  \
        return std::make_unique<user_application>();                                                                   \
    }                                                                                                                  \
    }

namespace zth {

struct ApplicationSpec
{
    WindowSpec window_spec{};
    LoggerSpec logger_spec{};
};

class Application
{
public:
    explicit Application(const ApplicationSpec& spec = {});
    ZTH_NO_COPY_NO_MOVE(Application)
    virtual ~Application();

    auto run() -> void;

private:
    virtual auto on_event([[maybe_unused]] const Event& event) -> void {}
    virtual auto on_update() -> void {}
    virtual auto on_render() -> void {}

    auto dispatch_event(const Event& event) -> void;
    auto update() -> void;
    auto render() -> void;
};

} // namespace zth
