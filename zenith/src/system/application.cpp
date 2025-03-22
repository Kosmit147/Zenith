#include "zenith/system/application.hpp"

#include "zenith/core/scene_manager.hpp"
#include "zenith/core/system_manager.hpp"
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
    Window::make_context_current();

    while (!Window::should_close())
    {
        while (auto event = EventQueue::pop())
            dispatch_event(*event);

        update();
        render();

        Window::swap_buffers();
        Window::poll_events();
    }
}

auto Application::dispatch_event(const Event& event) -> void
{
    SystemManager::dispatch_event(event);
    on_event(event);
}

auto Application::update() -> void
{
    SystemManager::update();
    on_update();
}

auto Application::render() -> void
{
    SceneManager::render_scene();
    SystemManager::on_render();
    on_render();
}

} // namespace zth
