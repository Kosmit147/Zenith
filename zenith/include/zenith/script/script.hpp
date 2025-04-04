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

    [[nodiscard]] virtual auto display_label() const -> const char* { return "Script"; }
    virtual auto debug_edit() -> void {}

    virtual auto on_event([[maybe_unused]] EntityHandle actor, [[maybe_unused]] const Event& event) -> void {}
    virtual auto on_fixed_update([[maybe_unused]] EntityHandle actor) -> void {}
    virtual auto on_update([[maybe_unused]] EntityHandle actor) -> void {}
};

} // namespace zth
