#include "Zenith/Core/Application.hpp"

#include <spdlog/spdlog.h>

#include "Zenith/Core/Engine.hpp"

namespace zth {

Application::Application(const ApplicationSpec& spec)
{
    Logger::init(spec.core_logger_spec, spec.client_logger_spec);
    Engine::init(spec.window_spec);
}

Application::~Application()
{
    // this must be called before main finishes if using asynchronous loggers
    spdlog::drop_all();

    Engine::shut_down();
    Logger::shut_down();
}

auto Application::run() const -> void
{
    auto& window = Engine::get().window;
    window.set_active();

    while (!window.should_close())
    {
        window.swap_buffers();
        window.poll_events();
    }
}

} // namespace zth
