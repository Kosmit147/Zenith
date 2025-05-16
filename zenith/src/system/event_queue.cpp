#include "zenith/system/event_queue.hpp"

namespace zth {

// @speed: Could allocate events using temporary storage or some kind of ring buffer.
Queue<Event> EventQueue::_event_queue;

auto EventQueue::push(const Event& event) -> void
{
    _event_queue.push(event);
}

auto EventQueue::pop() -> Optional<Event>
{
    if (_event_queue.empty())
        return nil;

    auto res = _event_queue.front();
    _event_queue.pop();
    return res;
}

} // namespace zth
