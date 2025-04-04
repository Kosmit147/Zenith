#include "zenith/layer/layer.hpp"

#include "zenith/core/assert.hpp"

namespace zth {

auto LayerStack::push(std::unique_ptr<Layer>&& layer) -> Result<Reference<Layer>, String>
{
    auto& attached_layer = *_layers.emplace_back(std::move(layer));
    auto attach_result = attached_layer.on_attach();

    if (!attach_result)
    {
        _layers.pop_back();
        return Error{ attach_result.error() };
    }

    return attached_layer;
}

auto LayerStack::pop() -> bool
{
    if (_layers.empty())
        return false;

    auto& detached_layer = top();
    detached_layer.on_detach();
    _layers.pop_back();

    return true;
}

auto LayerStack::top() -> Layer&
{
    ZTH_ASSERT(!_layers.empty());
    return *_layers.back();
}

auto LayerStack::top() const -> const Layer&
{
    ZTH_ASSERT(!_layers.empty());
    return *_layers.back();
}

auto LayerStack::start_frame() -> void
{
    for (auto& layer : _layers)
        layer->on_frame_start();
}

auto LayerStack::fixed_update() -> void
{
    for (auto& layer : _layers)
        layer->on_fixed_update();
}

auto LayerStack::dispatch_event(const Event& event) -> void
{
    for (auto& layer : _layers)
        layer->on_event(event);
}

auto LayerStack::update() -> void
{
    for (auto& layer : _layers)
        layer->on_update();
}

auto LayerStack::render() -> void
{
    for (auto& layer : _layers)
        layer->render();

    for (auto& layer : _layers)
        layer->on_render();
}

} // namespace zth
