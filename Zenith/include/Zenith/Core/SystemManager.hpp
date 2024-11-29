#pragma once

namespace zth {

struct ApplicationSpec;

class Event;

class SystemManager
{
public:
    SystemManager() = delete;

    static auto init_systems(const ApplicationSpec& spec) -> void;
    static auto on_event(const Event& event) -> void;
    static auto on_update() -> void;
    static auto shut_down_systems() -> void;
};

} // namespace zth
