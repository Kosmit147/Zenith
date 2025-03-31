#pragma once

#include <memory>

#include "zenith/core/scene.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"
#include "zenith/util/result.hpp"

namespace zth {

class SceneManager
{
public:
    SceneManager() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto start_frame() -> void;
    static auto dispatch_event(const Event& event) -> void;
    static auto update() -> void;
    static auto render() -> void;
    static auto on_render() -> void;
    static auto shut_down() -> void;

    static auto queue_scene(std::unique_ptr<Scene>&& scene) -> void;

    [[nodiscard]] static auto scene() -> Optional<Reference<Scene>>;
    [[nodiscard]] static auto scene_unchecked() -> Scene&;

private:
    static inline std::unique_ptr<Scene> _scene = nullptr;
    static inline std::unique_ptr<Scene> _queued_scene = nullptr;
};

} // namespace zth
