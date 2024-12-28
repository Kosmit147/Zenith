#include <spdlog/spdlog.h>

#include <exception>
#include <iostream>
#include <memory>
#include <print>

#include "Zenith/Core/Application.hpp"
#include "Zenith/Core/Exception.hpp"

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
        std::println(std::cerr, "Failed to initialize logger: {}", e.what());
    }
    catch (const Exception& e)
    {
        std::println(std::cerr, "{}\n{}", e.what(), e.stacktrace());
    }
    catch (const std::exception& e)
    {
        std::println(std::cerr, "{}", e.what());
    }

    // this must be called before main finishes if using asynchronous loggers
    spdlog::drop_all();
}
