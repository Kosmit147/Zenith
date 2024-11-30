#include "Zenith/Core/Application.hpp"

#include "Zenith/Core/SystemManager.hpp"
#include "Zenith/Graphics/Renderer.hpp"
#include "Zenith/Platform/EventQueue.hpp"

namespace zth {

Application::Application(const ApplicationSpec& spec)
{
    SystemManager::init_systems(spec);
    ZTH_CORE_INFO("Application initialized.");
}

Application::~Application()
{
    ZTH_CORE_INFO("Shutting down application.");
    SystemManager::shut_down_systems();
}

auto Application::run() -> void
{
    Window::set_active();

    while (!Window::should_close())
    {
        Renderer::clear();

        while (auto event = EventQueue::pop())
            handle_event(event.value());

        handle_update();

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

} // namespace zth
