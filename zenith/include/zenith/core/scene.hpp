#pragma once

#include "zenith/ecs/ecs.hpp"
#include "zenith/memory/managed.hpp"
#include "zenith/memory/temporary_storage.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"
#include "zenith/util/result.hpp"

namespace zth {

class Scene
{
public:
    explicit Scene();
    ZTH_NO_COPY_NO_MOVE(Scene)
    virtual ~Scene() = default;

    auto start_frame() -> void;
    auto dispatch_event(const Event& event) -> void;
    auto fixed_update() -> void;
    auto update() -> void;
    auto render() -> void;

    auto create_entity(const String& tag) -> EntityHandle;
    auto create_entity(String&& tag = "Entity") -> EntityHandle;

    [[nodiscard]] auto find_entity_by_tag(StringView tag) -> Optional<EntityHandle>;
    [[nodiscard]] auto find_entities_by_tag(StringView tag) -> TemporaryVector<EntityHandle>;

    [[nodiscard]] auto registry(this auto&& self) -> auto& { return self._registry; }

    friend class SceneManager;

private:
    Registry _registry;

private:
    virtual auto on_load() -> void {}
    virtual auto on_frame_start() -> void {}
    virtual auto on_event([[maybe_unused]] const Event& event) -> void {}
    virtual auto on_fixed_update() -> void {}
    virtual auto on_update() -> void {}
    virtual auto on_render() -> void {}
    virtual auto on_unload() -> void {}
};

class SceneManager
{
public:
    SceneManager() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto start_frame() -> void;
    static auto dispatch_event(const Event& event) -> void;
    static auto fixed_update() -> void;
    static auto update() -> void;
    static auto render() -> void;
    static auto on_render() -> void;
    static auto shut_down() -> void;

    static auto queue_scene(UniquePtr<Scene>&& scene) -> void;

    [[nodiscard]] static auto scene() -> Optional<Reference<Scene>>;
    [[nodiscard]] static auto scene_unchecked() -> Scene&;

private:
    static inline UniquePtr<Scene> _scene = nullptr;
    static inline UniquePtr<Scene> _queued_scene = nullptr;
};

} // namespace zth
