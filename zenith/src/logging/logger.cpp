#include "zenith/logging/logger.hpp"

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "zenith/core/assert.hpp"

namespace zth {

auto Logger::init(const LoggerSpec& logger_spec) -> void
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logger_spec.log_file_path, true);

    spdlog::sinks_init_list sink_list = { console_sink, file_sink };
    _core_logger = std::make_shared<spdlog::logger>(logger_spec.core_logger_label, sink_list);
    _client_logger = std::make_shared<spdlog::logger>(logger_spec.client_logger_label, sink_list);

#ifndef ZTH_DIST_BUILD
    _core_logger->set_level(spdlog::level::trace);
    _client_logger->set_level(spdlog::level::trace);
#else
    _core_logger->set_level(spdlog::level::info);
    _client_logger->set_level(spdlog::level::info);
#endif

    ZTH_CORE_INFO("Logger initialized.");
}

auto Logger::shut_down() -> void
{
    ZTH_CORE_INFO("Logger shut down.");

    _client_logger.reset();
    _core_logger.reset();
}

auto Logger::core_logger() -> spdlog::logger&
{
    ZTH_ASSERT(_core_logger != nullptr);
    return *_core_logger;
}

auto Logger::client_logger() -> spdlog::logger&
{
    ZTH_ASSERT(_client_logger != nullptr);
    return *_client_logger;
}

} // namespace zth
