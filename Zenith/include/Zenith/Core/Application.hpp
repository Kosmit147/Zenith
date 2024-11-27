#pragma once

#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Window.hpp"
#include "Zenith/Time/Timer.hpp"
#include "Zenith/Utility/Utility.hpp"

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
    Timer _time_timer;
    Timer _delta_time_timer;

private:
    virtual auto on_update() -> void {}
};

} // namespace zth
