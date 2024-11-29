#pragma once

#include <optional>

#include "Zenith/Platform/Event.hpp"

namespace zth {

class EventQueue
{
public:
    static auto push(const Event& event) -> void;
    static auto pop() -> std::optional<Event>;
};

} // namespace zth
