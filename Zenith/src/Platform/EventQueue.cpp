#include "Zenith/Platform/EventQueue.hpp"

#include <queue>

namespace zth {

namespace {

std::queue<Event> event_queue;

}

auto EventQueue::push(const Event& event) -> void
{
    event_queue.push(event);
}

auto EventQueue::pop() -> std::optional<Event>
{
    std::optional<Event> res = {};

    if (event_queue.empty())
        return res;

    res = event_queue.front();
    event_queue.pop();
    return res;
}

}
