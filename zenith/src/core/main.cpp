#include <spdlog/spdlog.h>

#include <print>

#include "zenith/core/exception.hpp"
#include "zenith/memory/managed.hpp"
#include "zenith/system/application.hpp"

namespace zth {

// This function must be defined in the user's project using the ZTH_IMPLEMENT_APP macro.
[[nodiscard]] extern auto create_application() -> UniquePtr<Application>;

} // namespace zth

auto main() -> int
{
    try
    {
        auto app = zth::create_application();
        app->run();
    }
    catch (const zth::Exception& e)
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

    // This should be called before main finishes if using asynchronous loggers (an MSVC workaround).
    spdlog::drop_all();
}
