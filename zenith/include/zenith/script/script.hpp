#pragma once

#include "zenith/ecs/ecs.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/macros.hpp"

// @todo: These script.* files probably shouldn't be in this folder among all the example scripts.

namespace zth {

class Script
{
public:
    Script() = default;
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Script)
    virtual ~Script() = default;

    [[nodiscard]] virtual auto display_label() const -> const char* { return "Script"; }
    virtual auto debug_edit() -> void {}

    virtual auto on_event([[maybe_unused]] EntityHandle actor, [[maybe_unused]] const Event& event) -> void {}
    virtual auto on_fixed_update([[maybe_unused]] EntityHandle actor) -> void {}
    virtual auto on_update([[maybe_unused]] EntityHandle actor) -> void {}

    friend class Scene; // Scene needs to be able to add on_attach and on_detach listeners.

private:
    virtual auto on_attach([[maybe_unused]] EntityHandle actor) -> void {}
    virtual auto on_detach([[maybe_unused]] EntityHandle actor) -> void {}
};

} // namespace zth
