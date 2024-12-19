#pragma once

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace zth {

struct LoggerSpec
{
    std::string core_logger_label = "ZENITH";
    std::string client_logger_label = "APP";
    std::string log_file_path = "log/log.txt";
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
    static inline std::shared_ptr<spdlog::logger> _core_logger;
    static inline std::shared_ptr<spdlog::logger> _client_logger;
};

} // namespace zth

// macros for Zenith
#ifdef ZTH_DIST_BUILD

#define ZTH_CORE_TRACE(...)
#define ZTH_CORE_DEBUG(...)

#else

#define ZTH_CORE_TRACE(...) ::zth::Logger::core_logger().trace(__VA_ARGS__)
#define ZTH_CORE_DEBUG(...) ::zth::Logger::core_logger().debug(__VA_ARGS__)

#endif

#define ZTH_CORE_INFO(...) ::zth::Logger::core_logger().info(__VA_ARGS__)
#define ZTH_CORE_WARN(...) ::zth::Logger::core_logger().warn(__VA_ARGS__)
#define ZTH_CORE_ERROR(...) ::zth::Logger::core_logger().error(__VA_ARGS__)
#define ZTH_CORE_CRITICAL(...) ::zth::Logger::core_logger().critical(__VA_ARGS__)

// macros for clients
#ifdef ZTH_DIST_BUILD

#define ZTH_TRACE(...)
#define ZTH_DEBUG(...)

#else

#define ZTH_TRACE(...) ::zth::Logger::client_logger().trace(__VA_ARGS__)
#define ZTH_DEBUG(...) ::zth::Logger::client_logger().debug(__VA_ARGS__)

#endif

#define ZTH_INFO(...) ::zth::Logger::client_logger().info(__VA_ARGS__)
#define ZTH_WARN(...) ::zth::Logger::client_logger().warn(__VA_ARGS__)
#define ZTH_ERROR(...) ::zth::Logger::client_logger().error(__VA_ARGS__)
#define ZTH_CRITICAL(...) ::zth::Logger::client_logger().critical(__VA_ARGS__)
