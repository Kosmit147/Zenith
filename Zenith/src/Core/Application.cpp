#include "Zenith/Core/Application.hpp"

#include "Zenith/Core/Engine.hpp"

namespace zth {

Application::Application(const ApplicationSpec& spec)
{
    Logger::init(spec.logger_spec);
    Engine::init(spec.window_spec);
}

Application::~Application()
{
    Engine::shut_down();
    Logger::shut_down();
}

auto Application::run() -> void
{
    auto& window = Engine::get().window;
    window.set_active();

    while (!window.should_close())
    {
        on_update();

        window.swap_buffers();
        window.poll_events();
    }
}

} // namespace zth
