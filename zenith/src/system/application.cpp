#include "zenith/system/application.hpp"

#include "zenith/core/system_manager.hpp"
#include "zenith/graphics/renderer.hpp"
#include "zenith/system/event_queue.hpp"

namespace zth {

Application::Application(const ApplicationSpec& spec)
{
    SystemManager::init_systems(spec);
    ZTH_CORE_INFO("Application initialized.");
}

Application::~Application()
{
    ZTH_CORE_INFO("Shutting down application...");
    SystemManager::shut_down_systems();
}

auto Application::run() -> void
{
    Window::set_active();

    while (!Window::should_close())
    {
        while (auto event = EventQueue::pop())
            handle_event(*event);

        handle_update();

        Renderer::clear();
        Renderer::render();
        handle_render();

        Window::swap_buffers();
        Window::poll_events();
    }
}

auto Application::handle_event(const Event& event) -> void
{
    SystemManager::on_event(event);
    on_event(event);
}

auto Application::handle_update() -> void
{
    SystemManager::on_update();
    on_update();
}

auto Application::handle_render() -> void
{
    SystemManager::on_render();
    on_render();
}

} // namespace zth
