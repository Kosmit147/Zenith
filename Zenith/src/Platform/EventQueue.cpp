#include "Zenith/Platform/EventQueue.hpp"

namespace zth {

auto EventQueue::push(const Event& event) -> void
{
    _event_queue.push(event);
}

auto EventQueue::pop() -> std::optional<Event>
{
    std::optional<Event> res = {};

    if (_event_queue.empty())
        return res;

    res = _event_queue.front();
    _event_queue.pop();
    return res;
}

} // namespace zth
