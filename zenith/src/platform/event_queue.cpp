#include "zenith/platform/event_queue.hpp"

namespace zth {

std::queue<Event> EventQueue::_event_queue;

auto EventQueue::push(const Event& event) -> void
{
    _event_queue.push(event);
}

auto EventQueue::pop() -> std::optional<Event>
{
    if (_event_queue.empty())
        return {};

    auto res = _event_queue.front();
    _event_queue.pop();
    return std::make_optional(res);
}

} // namespace zth
