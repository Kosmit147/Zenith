#pragma once

#include <concepts>
#include <functional>

#include "zenith/ecs/ecs.hpp"
#include "zenith/memory/managed.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/fwd.hpp"
#include "zenith/system/temporary_storage.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/result.hpp"

namespace zth {

class Scene
{
public:
    explicit Scene(const String& name);
    explicit Scene(String&& name = "Unnamed Scene");

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

    [[nodiscard]] auto name() const -> auto& { return _name; }
    [[nodiscard]] auto registry(this auto&& self) -> auto& { return self._registry; }

    friend class SceneManager;

private:
    String _name;
    Registry _registry;

private:
    auto load() -> void;
    auto unload() -> void;

    virtual auto on_load() -> void {}
    virtual auto on_frame_start() -> void {}
    virtual auto on_event([[maybe_unused]] const Event& event) -> void {}
    virtual auto on_fixed_update() -> void {}
    virtual auto on_update() -> void {}
    virtual auto on_render() -> void {}
    virtual auto on_unload() -> void {}

    auto set_up_registry_listeners() -> void;
};

// SceneManager ensures that there is always a scene loaded.
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

    template<std::derived_from<Scene> T> static auto queue_scene() -> void;
    static auto queue_scene(const std::function<UniquePtr<Scene>()>& factory) -> void;
    static auto queue_scene(std::function<UniquePtr<Scene>()>&& factory) -> void;

    [[nodiscard]] static auto scene() -> Scene&;

private:
    static UniquePtr<Scene> _scene;
    static std::function<UniquePtr<Scene>()> _queued_scene_factory;
};

template<std::derived_from<Scene> T> auto SceneManager::queue_scene() -> void
{
    _queued_scene_factory = make_unique<T>;
}

} // namespace zth
