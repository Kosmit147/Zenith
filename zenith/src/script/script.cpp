#include "zenith/script/script.hpp"

namespace zth {

auto Script::dispatch_event(EntityHandle actor, const Event& event) -> void
{
    on_event(actor, event);
}

auto Script::update(EntityHandle actor) -> void
{
    on_update(actor);
}

} // namespace zth
