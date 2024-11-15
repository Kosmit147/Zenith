#include "Zenith/Core/Application.hpp"

namespace zth {

auto Application::create(const ApplicationSpec& spec) -> std::optional<Application>
{
    auto maybe_window = Window::create(spec.window_spec);

    if (!maybe_window)
        return {};

    auto& window = maybe_window.value();

    return std::optional{ Application{ std::move(window) } };
}

Application::Application(Application&& other) noexcept : _window(std::move(other._window)) {}

auto Application::operator=(Application&& other) noexcept -> Application&
{
    _window = std::move(other._window);
    return *this;
}

auto Application::run() const -> void
{
    _window.set_active();

    while (!_window.should_close())
    {
        _window.swap_buffers();
        _window.poll_events();
    }
}

Application::Application(Window&& window) : _window(std::move(window)) {}

} // namespace zth
