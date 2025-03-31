#pragma once

#include "zenith/ecs/ecs.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

class Scene
{
public:
    explicit Scene() = default;
    ZTH_NO_COPY_NO_MOVE(Scene)
    virtual ~Scene() = default;

    auto start_frame() -> void;
    auto dispatch_event(const Event& event) -> void;
    auto update() -> void;
    auto render() -> void;

    auto create_entity(const String& tag) -> EntityHandle;
    auto create_entity(String&& tag) -> EntityHandle;

    [[nodiscard]] auto registry(this auto&& self) -> auto& { return self._registry; }

    friend class SceneManager;

private:
    Registry _registry;

private:
    virtual auto on_load() -> void {}
    virtual auto on_frame_start() -> void {}
    virtual auto on_event([[maybe_unused]] const Event& event) -> void {}
    virtual auto on_update() -> void {}
    virtual auto on_render() -> void {}
    virtual auto on_unload() -> void {}
};

} // namespace zth
