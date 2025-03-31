#pragma once

#include <memory>

#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/reference.hpp"
#include "zenith/util/result.hpp"

namespace zth {

class Layer
{
public:
    explicit Layer() = default;
    ZTH_NO_COPY_NO_MOVE(Layer)
    virtual ~Layer() = default;

    virtual auto render() -> void {}

    virtual auto on_frame_start() -> void {}
    virtual auto on_event([[maybe_unused]] const Event& event) -> void {}
    virtual auto on_update() -> void {}
    virtual auto on_render() -> void {}

    friend class LayerStack;

private:
    [[nodiscard]] virtual auto on_attach() -> Result<void, String> { return {}; }
    virtual auto on_detach() -> void {}
};

class LayerStack
{
public:
    [[nodiscard]] auto push(std::unique_ptr<Layer>&& layer) -> Result<Reference<Layer>, String>;
    // Returns false if layer stack is empty and there are no layers left to remove.
    auto pop() -> bool;

    [[nodiscard]] auto top() -> Layer&;
    [[nodiscard]] auto top() const -> const Layer&;

    auto start_frame() -> void;
    auto update() -> void;
    auto dispatch_event(const Event& event) -> void;
    auto render() -> void;

private:
    Vector<std::unique_ptr<Layer>> _layers;
};

} // namespace zth
