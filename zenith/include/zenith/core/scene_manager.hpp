#pragma once

#include <memory>

#include "zenith/core/scene.hpp"
#include "zenith/system/fwd.hpp"

namespace zth {

class SceneManager
{
public:
    SceneManager() = delete;

    static auto init() -> void;
    static auto dispatch_event(const Event& event) -> void;
    static auto update() -> void;
    static auto render_scene() -> void;
    static auto on_render() -> void;
    static auto shut_down() -> void;

    static auto load_scene(std::unique_ptr<Scene>&& scene) -> void;

private:
    static inline std::unique_ptr<Scene> _scene = nullptr;
    static inline std::unique_ptr<Scene> _queued_scene = nullptr;
};

} // namespace zth
