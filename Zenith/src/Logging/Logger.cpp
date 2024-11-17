#include "Zenith/Logging/Logger.hpp"

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace zth {

namespace {

std::shared_ptr<spdlog::logger> core_logger;
std::shared_ptr<spdlog::logger> client_logger;

} // namespace

auto Logger::init(const LoggerSpec& logger_spec) -> void
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logger_spec.log_file_path, true);

    spdlog::sinks_init_list sink_list = { console_sink, file_sink };
    core_logger = std::make_shared<spdlog::logger>(logger_spec.core_logger_label, sink_list);
    client_logger = std::make_shared<spdlog::logger>(logger_spec.client_logger_label, sink_list);

#ifdef _DEBUG
    core_logger->set_level(spdlog::level::trace);
    client_logger->set_level(spdlog::level::trace);
#else
    core_logger->set_level(spdlog::level::info);
    client_logger->set_level(spdlog::level::info);
#endif
}

auto Logger::shut_down() -> void
{
    client_logger.reset();
    core_logger.reset();
}

auto Logger::get_core_logger() -> spdlog::logger&
{
    return *core_logger;
}

auto Logger::get_client_logger() -> spdlog::logger&
{
    return *client_logger;
}

} // namespace zth
