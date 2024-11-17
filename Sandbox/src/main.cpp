#include <Zenith/Zenith.hpp>

#include <iostream>
#include <print>

namespace {

const zth::ApplicationSpec app_spec = {
    .window_spec = {
        .width = 1920,
        .height = 1080,
        .title = "Sandbox",
        .gl_version = { 4, 6 },
        .gl_profile = zth::GlProfile::Core,
        .vsync = true,
    },
};

}

auto main() -> int
{
    try
    {
        zth::Application app(app_spec);
        app.run();
    }
    catch (const spdlog::spdlog_ex& e)
    {
        std::println(std::cerr, "Failed to initialize logger: {}", e.what());
    }
    catch (const zth::Exception& e)
    {
        ZTH_CRITICAL("{}\n{}", e.what(), e.stacktrace());
    }
    catch (const std::exception& e)
    {
        ZTH_CRITICAL(e.what());
    }
}
