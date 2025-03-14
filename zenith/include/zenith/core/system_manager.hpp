#pragma once

#include "zenith/stl/vector.hpp"
#include "zenith/system/fwd.hpp"

namespace zth {

class SystemManager
{
public:
    SystemManager() = delete;

    static auto init_systems(const ApplicationSpec& spec) -> void;
    static auto on_event(const Event& event) -> void;
    static auto on_update() -> void;
    static auto on_render() -> void;
    static auto shut_down_systems() -> void;

private:
    using SystemShutdownFunc = void (*)();

    static Vector<SystemShutdownFunc> _system_shutdown_funcs;
};

} // namespace zth
