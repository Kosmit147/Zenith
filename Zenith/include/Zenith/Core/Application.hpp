#pragma once

#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Window.hpp"
#include "Zenith/Platform/fwd.hpp"
#include "Zenith/Utility/Macros.hpp"

#define ZTH_IMPLEMENT_APP(user_app)                                                                                    \
    namespace zth {                                                                                                    \
                                                                                                                       \
    [[nodiscard]] auto create_application() -> Application*                                                            \
    {                                                                                                                  \
        return new user_app;                                                                                           \
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

    auto handle_event(const Event& event) -> void;
    auto handle_update() -> void;
    auto handle_render() -> void;
};

} // namespace zth
