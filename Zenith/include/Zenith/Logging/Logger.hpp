#pragma once

#include <spdlog/spdlog.h>

#include <string>

namespace zth {

struct LoggerSpec
{
    std::string logger_name = "LOGGER";
    std::string log_file_path = "log.txt";
};

class Logger
{
public:
    static auto init(const LoggerSpec& core_logger_spec, const LoggerSpec& client_logger_spec) -> void;
    static auto shut_down() -> void;

    [[nodiscard]] static auto get_core_logger() -> spdlog::logger&;
    [[nodiscard]] static auto get_client_logger() -> spdlog::logger&;
};

} // namespace zth

// macros for Zenith
#ifdef _DEBUG

#define ZTH_CORE_TRACE(...) ::zth::Logger::get_core_logger().trace(__VA_ARGS__)
#define ZTH_CORE_DEBUG(...) ::zth::Logger::get_core_logger().debug(__VA_ARGS__)

#else

#define ZTH_CORE_TRACE(...)
#define ZTH_CORE_DEBUG(...)

#endif

#define ZTH_CORE_INFO(...) ::zth::Logger::get_core_logger().info(__VA_ARGS__)
#define ZTH_CORE_WARN(...) ::zth::Logger::get_core_logger().warn(__VA_ARGS__)
#define ZTH_CORE_ERROR(...) ::zth::Logger::get_core_logger().error(__VA_ARGS__)
#define ZTH_CORE_CRITICAL(...) ::zth::Logger::get_core_logger().critical(__VA_ARGS__)

// macros for clients
#ifdef _DEBUG

#define ZTH_TRACE(...) ::zth::Logger::get_client_logger().trace(__VA_ARGS__)
#define ZTH_DEBUG(...) ::zth::Logger::get_client_logger().debug(__VA_ARGS__)

#else

#define ZTH_TRACE(...)
#define ZTH_DEBUG(...)

#endif

#define ZTH_INFO(...) ::zth::Logger::get_client_logger().info(__VA_ARGS__)
#define ZTH_WARN(...) ::zth::Logger::get_client_logger().warn(__VA_ARGS__)
#define ZTH_ERROR(...) ::zth::Logger::get_client_logger().error(__VA_ARGS__)
#define ZTH_CRITICAL(...) ::zth::Logger::get_client_logger().critical(__VA_ARGS__)
