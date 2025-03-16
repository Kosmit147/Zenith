#pragma once

#include <spdlog/spdlog.h>

#include <memory>

#include "zenith/stl/string.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

struct LoggerSpec
{
    String core_logger_label = "ZENITH";
    String client_logger_label = "APP";
    String log_file_path = "log/log.txt";
};

class Logger
{
public:
    Logger() = delete;

    static auto init(const LoggerSpec& logger_spec) -> void;
    static auto shut_down() -> void;

    [[nodiscard]] static auto core_logger() -> spdlog::logger&;
    [[nodiscard]] static auto client_logger() -> spdlog::logger&;

private:
    static std::shared_ptr<spdlog::logger> _core_logger;
    static std::shared_ptr<spdlog::logger> _client_logger;
};

} // namespace zth

// macros for Zenith
#define ZTH_CORE_TRACE(...) ::zth::Logger::core_logger().trace(__VA_ARGS__)
#define ZTH_CORE_DEBUG(...) ::zth::Logger::core_logger().debug(__VA_ARGS__)
#define ZTH_CORE_INFO(...) ::zth::Logger::core_logger().info(__VA_ARGS__)
#define ZTH_CORE_WARN(...) ::zth::Logger::core_logger().warn(__VA_ARGS__)
#define ZTH_CORE_ERROR(...) ::zth::Logger::core_logger().error(__VA_ARGS__)
#define ZTH_CORE_CRITICAL(...) ::zth::Logger::core_logger().critical(__VA_ARGS__)

// macros for clients
#define ZTH_TRACE(...) ::zth::Logger::client_logger().trace(__VA_ARGS__)
#define ZTH_DEBUG(...) ::zth::Logger::client_logger().debug(__VA_ARGS__)
#define ZTH_INFO(...) ::zth::Logger::client_logger().info(__VA_ARGS__)
#define ZTH_WARN(...) ::zth::Logger::client_logger().warn(__VA_ARGS__)
#define ZTH_ERROR(...) ::zth::Logger::client_logger().error(__VA_ARGS__)
#define ZTH_CRITICAL(...) ::zth::Logger::client_logger().critical(__VA_ARGS__)

#if defined(ZTH_DIST_BUILD)

#undef ZTH_CORE_TRACE
#undef ZTH_CORE_DEBUG

#define ZTH_CORE_TRACE(...) ZTH_NOP
#define ZTH_CORE_DEBUG(...) ZTH_NOP

#endif
