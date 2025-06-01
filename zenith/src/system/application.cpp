#include "zenith/system/application.hpp"

#include "zenith/core/profiler.hpp"
#include "zenith/core/scene.hpp"
#include "zenith/layer/layers.hpp"
#include "zenith/system/event_queue.hpp"
#include "zenith/system/window.hpp"
#include "zenith/util/defer.hpp"

namespace zth {

LayerStack Application::_layers;
LayerStack Application::_overlays;

auto Application::init(const ApplicationSpec& spec) -> Result<void, String>
{
    delta_time_limit = spec.delta_time_limit;
    fixed_time_step = spec.fixed_time_step;

    Defer cleanup{ [&] {
        pop_all_overlays();
        pop_all_layers();
    } };

    auto result = push_layer(make_unique<SystemLayer>(spec.logger_spec, spec.window_spec));
    if (!result)
        return Error{ result.error() };

    result = push_layer(make_unique<RuntimeLayer>());
    if (!result)
        return Error{ result.error() };

    result = push_overlay(make_unique<DebugLayer>());
    if (!result)
        return Error{ result.error() };

    ZTH_INTERNAL_TRACE("Application initialized.");
    cleanup.dismiss();

    return {};
}

auto Application::push_layer(UniquePtr<Layer>&& layer) -> Result<Reference<Layer>, String>
{
    return _layers.push(std::move(layer));
}

auto Application::pop_layer() -> bool
{
    return _layers.pop();
}

auto Application::push_overlay(UniquePtr<Layer>&& overlay) -> Result<Reference<Layer>, String>
{
    return _overlays.push(std::move(overlay));
}

auto Application::pop_overlay() -> bool
{
    return _overlays.pop();
}

auto Application::run() -> void
{
    Window::make_context_current();

#if defined(ZTH_PROFILER)
    Profiler::init();
#endif

    while (!Window::should_close())
    {
#if defined(ZTH_PROFILER)
        Profiler::start_frame();
#endif

        start_frame();

        while (auto event = EventQueue::pop())
            dispatch_event(*event);

        fixed_update();
        update();
        render();

        Window::swap_buffers();
        Window::poll_events();
    }

    shut_down();
}

auto Application::time() -> double
{
    return Window::time();
}

auto Application::delta_time() -> double
{
    return _delta_time;
}

auto Application::frame_time() -> double
{
    return _frame_time;
}

auto Application::fixed_update_time() -> double
{
    return _fixed_update_time;
}

auto Application::update_time() -> double
{
    return _update_time;
}

auto Application::render_time() -> double
{
    return _render_time;
}

auto Application::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down application...");
    pop_all_overlays();
    pop_all_layers();
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

auto Application::start_frame() -> void
{
    ZTH_PROFILE_FUNCTION();

    auto current_time = time();
    _frame_time = current_time - _prev_start_frame_time_point;
    _delta_time = std::min(_frame_time, delta_time_limit);
    _prev_start_frame_time_point = current_time;

    _layers.start_frame();
    _overlays.start_frame();
}

auto Application::dispatch_event(const Event& event) -> void
{
    _layers.dispatch_event(event);
    _overlays.dispatch_event(event);
}

auto Application::fixed_update() -> void
{
    ZTH_PROFILE_FUNCTION();

    auto start_fixed_update_time_point = time();

    auto accumulated_fixed_update_time = static_cast<double>(_fixed_updates_performed) * fixed_time_step;
    auto fixed_updates_to_perform = static_cast<usize>((time() - accumulated_fixed_update_time) / fixed_time_step);

    for (usize i = 0; i < fixed_updates_to_perform; i++)
    {
        _layers.fixed_update();
        _overlays.fixed_update();
    }

    _fixed_updates_performed += fixed_updates_to_perform;

    _fixed_update_time = time() - start_fixed_update_time_point;
}

auto Application::update() -> void
{
    ZTH_PROFILE_FUNCTION();

    auto start_update_time_point = time();

    _layers.update();
    _overlays.update();

    _update_time = time() - start_update_time_point;
}

auto Application::render() -> void
{
    ZTH_PROFILE_FUNCTION();

    auto start_render_time_point = time();

    _layers.render();
    _overlays.render();

    _render_time = time() - start_render_time_point;
}

} // namespace zth
