#pragma once

#include <memory>

#include "Zenith/Core/Scene.hpp"
#include "Zenith/Platform/fwd.hpp"

namespace zth {

class SceneManager
{
public:
    SceneManager() = delete;

    static auto init() -> void;
    static auto on_update() -> void;
    static auto on_event(const Event& event) -> void;
    static auto on_render() -> void;
    static auto shut_down() -> void;

    static auto load_scene(std::unique_ptr<Scene>&& scene) -> void;

    [[nodiscard]] static auto scene() -> const Scene* { return _scene.get(); }

private:
    static inline std::unique_ptr<Scene> _scene = nullptr;
    static inline std::unique_ptr<Scene> _queued_scene = nullptr;
};

} // namespace zth
