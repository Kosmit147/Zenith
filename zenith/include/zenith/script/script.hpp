#pragma once

#include "zenith/ecs/ecs.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

class Script
{
public:
    Script() = default;
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Script)
    virtual ~Script() = default;

    // display_name is a string that gets displayed in the ui.
    [[nodiscard]] virtual auto display_name() const -> const char* { return "Script"; }
    virtual auto debug_edit() -> void {}

    friend class Scene;

private:
    auto dispatch_event(EntityHandle actor, const Event& event) -> void;
    auto update(EntityHandle actor) -> void;

    virtual auto on_event([[maybe_unused]] EntityHandle actor, [[maybe_unused]] const Event& event) -> void {}
    virtual auto on_update([[maybe_unused]] EntityHandle actor) -> void {}
};

} // namespace zth
