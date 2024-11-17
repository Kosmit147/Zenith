#include "Zenith/Core/Engine.hpp"

namespace zth {

namespace {

std::unique_ptr<Engine> engine;

}

auto Engine::init(const WindowSpec& window_spec) -> void
{
    engine = std::unique_ptr<Engine>(new Engine(window_spec));
}

auto Engine::get() -> Engine&
{
    return *engine;
}

auto Engine::shut_down() -> void
{
    engine.reset();
}

auto Engine::on_update() -> void {}

Engine::Engine(const WindowSpec& window_spec) : window(window_spec) {}

} // namespace zth
