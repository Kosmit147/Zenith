#include "zenith/system/application.hpp"

#include "zenith/core/exception.hpp"
#include "zenith/core/scene.hpp"
#include "zenith/layer/layers.hpp"
#include "zenith/system/event_queue.hpp"
#include "zenith/util/defer.hpp"

namespace zth {

Application::Application(const ApplicationSpec& spec)
{
    Defer cleanup{ [&] {
        pop_all_overlays();
        pop_all_layers();
    } };

    auto result = push_layer(std::make_unique<SystemLayer>(spec.logger_spec, spec.window_spec));
    if (!result)
        throw Exception{ result.error() };

    result = push_layer(std::make_unique<RuntimeLayer>());
    if (!result)
        throw Exception{ result.error() };

    result = push_overlay(std::make_unique<DebugLayer>());
    if (!result)
        throw Exception{ result.error() };

    ZTH_CORE_INFO("Application initialized.");
    cleanup.dismiss();
}

Application::~Application()
{
    ZTH_CORE_INFO("Shutting down application...");

    pop_all_overlays();
    pop_all_layers();
}

auto Application::push_layer(std::unique_ptr<Layer>&& layer) -> Result<Reference<Layer>, String>
{
    return _layers.push(std::move(layer));
}

auto Application::pop_layer() -> bool
{
    return _layers.pop();
}

auto Application::push_overlay(std::unique_ptr<Layer>&& overlay) -> Result<Reference<Layer>, String>
{
    return _overlays.push(std::move(overlay));
}

auto Application::pop_overlay() -> bool
{
    return _overlays.pop();
}

auto Application::pop_all_layers() -> void
{
    while (pop_layer())
    {}
}

auto Application::pop_all_overlays() -> void
{
    while (pop_overlay())
    {}
}

auto Application::run() -> void
{
    Window::make_context_current();

    while (!Window::should_close())
    {
        start_frame();

        while (auto event = EventQueue::pop())
            dispatch_event(*event);

        update();
        render();

        Window::swap_buffers();
        Window::poll_events();
    }
}

auto Application::start_frame() -> void
{
    _layers.start_frame();
    _overlays.start_frame();
}

auto Application::dispatch_event(const Event& event) -> void
{
    _layers.dispatch_event(event);
    _overlays.dispatch_event(event);
}

auto Application::update() -> void
{
    _layers.update();
    _overlays.update();
}

auto Application::render() -> void
{
    _layers.render();
    _overlays.render();
}

} // namespace zth
