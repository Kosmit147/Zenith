#include "Zenith/Logging/Logger.hpp"

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace zth {

namespace {

std::shared_ptr<spdlog::logger> core_logger;
std::shared_ptr<spdlog::logger> client_logger;

[[nodiscard]] auto create_logger(const LoggerSpec& spec) -> std::shared_ptr<spdlog::logger>
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(spec.log_file_path, true);

    spdlog::sinks_init_list sink_list = { console_sink, file_sink };
    auto logger = std::make_shared<spdlog::logger>(spec.logger_name, sink_list);

#ifdef _DEBUG
    logger->set_level(spdlog::level::trace);
#else
    logger->set_level(spdlog::level::info);
#endif

    return logger;
}

} // namespace

auto Logger::init(const LoggerSpec& core_logger_spec, const LoggerSpec& client_logger_spec) -> void
{
    core_logger = create_logger(core_logger_spec);
    client_logger = create_logger(client_logger_spec);
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
