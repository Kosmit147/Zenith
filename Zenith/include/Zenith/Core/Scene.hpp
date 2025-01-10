#pragma once

#include "Zenith/Platform/fwd.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth {

class Scene
{
public:
    explicit Scene() = default;
    ZTH_NO_COPY_NO_MOVE(Scene)
    virtual ~Scene() = default;

    friend class SceneManager;

private:
    virtual auto on_load() -> void {}
    virtual auto on_update() -> void {}
    virtual auto on_event([[maybe_unused]] const Event& event) -> void {}
    virtual auto on_render() -> void {}
};

} // namespace zth
