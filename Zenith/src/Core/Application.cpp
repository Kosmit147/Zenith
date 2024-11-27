#include "Zenith/Core/Application.hpp"

#include "Zenith/Core/Engine.hpp"
#include "Zenith/Platform/Input.hpp"

namespace zth {

Application::Application(const ApplicationSpec& spec)
{
    Logger::init(spec.logger_spec);
    Engine::init(spec.window_spec);
    Input::init();
}

Application::~Application()
{
    Input::shut_down();
    Engine::shut_down();
    Logger::shut_down();
}

auto Application::run() -> void
{
    auto& engine = Engine::get();
    auto& window = engine.window;
    auto& renderer = engine.renderer;

    window.set_active();

    while (!window.should_close())
    {
        renderer.clear();

        auto time = _time_timer.elapsed_s();
        auto delta_time = _delta_time_timer.elapsed_s();
        _delta_time_timer.reset();

        engine.set_time(time);
        engine.set_delta_time(delta_time);

        on_update();
        engine.on_update();

        window.swap_buffers();
        window.poll_events();
    }
}

} // namespace zth
