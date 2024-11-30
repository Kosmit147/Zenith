#pragma once

#include <optional>
#include <queue>

#include "Zenith/Platform/Event.hpp"

namespace zth {

class EventQueue
{
public:
    static auto push(const Event& event) -> void;
    static auto pop() -> std::optional<Event>;

private:
    static inline std::queue<Event> _event_queue;
};

} // namespace zth
