#pragma once

#include <optional>
#include <queue>

#include "zenith/system/event.hpp"

namespace zth {

class EventQueue
{
public:
    static auto push(const Event& event) -> void;
    [[nodiscard]] static auto pop() -> std::optional<Event>;

private:
    static std::queue<Event> _event_queue;
};

} // namespace zth
