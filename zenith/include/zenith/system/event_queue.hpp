#pragma once

#include "zenith/stl/queue.hpp"
#include "zenith/system/event.hpp"
#include "zenith/util/optional.hpp"

namespace zth {

class EventQueue
{
public:
    static auto push(const Event& event) -> void;
    [[nodiscard]] static auto pop() -> Optional<Event>;

private:
    static Queue<Event> _event_queue;
};

} // namespace zth
