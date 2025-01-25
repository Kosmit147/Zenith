#include <spdlog/spdlog.h>

#include <exception>
#include <iostream>
#include <memory>
#include <print>

#include "zenith/core/application.hpp"
#include "zenith/core/exception.hpp"

namespace zth {

[[nodiscard]] auto create_application() -> Application*;

} // namespace zth

auto main() -> int
{
    using namespace zth;

    try
    {
        std::unique_ptr<Application> app{ create_application() };
        app->run();
    }
    catch (const spdlog::spdlog_ex& e)
    {
        std::println(std::cerr, "CRITICAL ERROR: Failed to initialize logger: {}", e.what());
    }
    catch (const Exception& e)
    {
        std::println(std::cerr, "CRITICAL ERROR: {}\n{}", e.what(), e.stacktrace());
    }
    catch (const std::exception& e)
    {
        std::println(std::cerr, "CRITICAL ERROR: {}", e.what());
    }
    catch (...)
    {
        std::println(std::cerr, "CRITICAL ERROR: Unrecognized exception thrown.");
    }

    // this should be called before main finishes if using asynchronous loggers
    spdlog::drop_all();
}
