#pragma once

#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Window.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

struct ApplicationSpec
{
    WindowSpec window_spec{};
    LoggerSpec core_logger_spec{ .logger_name = "ZENITH", .log_file_path = "zenith_log.txt" };
    LoggerSpec client_logger_spec{ .logger_name = "APP", .log_file_path = "app_log.txt" };
};

class Application
{
public:
    explicit Application(const ApplicationSpec& spec = {});

    ZTH_NO_COPY_NO_MOVE(Application)

    virtual ~Application();

    auto run() const -> void;

private:
    // virtual auto on_update() -> void;
    // virtual auto on_event() -> void;
};

} // namespace zth
