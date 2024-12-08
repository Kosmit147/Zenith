#pragma once

namespace zth {

class Event;

class EventListener
{
public:
    virtual ~EventListener() = default;

    virtual auto on_event(const Event& event) -> void = 0;
};

} // namespace zth
