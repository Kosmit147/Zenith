#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

#include <concepts>
#include <type_traits>
#include <utility>

#include "zenith/core/typedefs.hpp"
#include "zenith/ecs/fwd.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/system/temporary_storage.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"

// Zenith uses ENTT as a backend powering its entity component system.

// An entity component system allows game objects (entities) to be dynamically composed of any set of components which
// provides more freedom and flexibility than an object-oriented model. A component can be anything. Example components
// that Zenith uses are TagComponent, which stores the entity's tag (a name) and TransformComponent, which stores the
// entity's translation, rotation and scale. A Zenith entity is always required to have these two components.

// An entity is in essence just an id that is associated with a set of components through the Registry. The Registry
// provides ways to iterate through all instances of a component that it contains, which enables a more efficient way of
// dealing with updating entities: Instead of considering every entity separately and determining what needs to be done
// with it based on the state of the entity, you can just iterate over every instance of a component and update it.
// Components are closely packed in memory which highly increases cache efficiency.

// There are certain limitations to keep in mind when working with the entity component system:
// - Creating entities and components is allowed during iterations, except during reverse iterations, and it never
// invalidates already existing references.
// - Deleting the current entity or removing its components is allowed during iterations, but it could invalidate
// references. For all the other entities, destroying them or removing their iterated components is not allowed and
// results in undefined behavior.
// - In case of reverse iterations, adding or removing elements is not allowed under any circumstances.

// The most common way to interact with an entity is through an EntityHandle which is a thin wrapper over an entity id
// and a pointer to the registry associated with that entity. It exposes methods to operate on the entity's components
// and to destroy the entity.

// A component can be any user-defined type. It is not needed to announce the existence of a component before using it.

// The registry is not thread safe as it is. However:
// - As long as a thread iterates over entities that have component X or assigns / removes that component from a set of
// entities, another thread can safely do the same with components Y and Z.
// - Similarly, a single set of components can be iterated by multiple threads as long as the components are neither
// assigned nor removed in the meantime.

namespace zth {

// clang-format off

// An integral component is a component that cannot be removed.
template<typename T> struct is_integral_component : std::false_type {};
template<typename T> constexpr auto is_integral_component_v = is_integral_component<T>::value;

// We always require every entity to have a TagComponent and a TransformComponent.
template<> struct is_integral_component<TagComponent> : std::true_type {};
template<> struct is_integral_component<const TagComponent> : std::true_type {};
template<> struct is_integral_component<TransformComponent> : std::true_type {};
template<> struct is_integral_component<const TransformComponent> : std::true_type {};
template<> struct is_integral_component<DeletionMarkerComponent> : std::true_type {};
template<> struct is_integral_component<const DeletionMarkerComponent> : std::true_type {};

// clang-format on

template<typename T>
concept IntegralComponent = is_integral_component_v<T>;

template<typename... Components> using GetComponents = entt::get_t<Components...>;
template<typename... Components> using ExcludeComponents = entt::exclude_t<Components...>;

using EntityId = entt::entity;
constexpr inline auto null_entity = entt::null;

// A non-owning handle to an entity.
class ConstEntityHandle
{
public:
    // clang-format off
    struct InvalidHandle {};
    // clang-format on
    static constexpr InvalidHandle invalid;

public:
    ConstEntityHandle() = default;
    ConstEntityHandle(InvalidHandle) {}
    ConstEntityHandle(EntityId id, Registry& registry);

    [[nodiscard]] auto tag() const -> const TagComponent&;
    [[nodiscard]] auto transform() const -> const TransformComponent&;

    template<typename... Components> [[nodiscard]] auto all_of() const -> bool;
    template<typename... Components> [[nodiscard]] auto any_of() const -> bool;
    template<typename... Components> [[nodiscard]] auto get() const -> decltype(auto);
    template<typename... Components> [[nodiscard]] auto try_get() const -> decltype(auto);
    template<typename Component> [[nodiscard]] auto get_or_emplace(auto&&... args) const -> decltype(auto);

    [[nodiscard]] auto id() const { return _id; }
    [[nodiscard]] auto registry() const -> Optional<Reference<const Registry>>;

    [[nodiscard]] operator EntityId() const { return _id; }

    [[nodiscard]] auto valid() const -> bool;
    [[nodiscard]] explicit operator bool() const { return valid(); }

protected:
    EntityId _id = null_entity;
    Registry* _registry = nullptr;
};

// A non-owning handle to an entity.
class EntityHandle : public ConstEntityHandle
{
public:
    using ConstEntityHandle::ConstEntityHandle;

    [[nodiscard]] auto tag() const -> TagComponent&;
    [[nodiscard]] auto transform() const -> TransformComponent&;

    template<typename Component> auto emplace(auto&&... args) const -> decltype(auto);
    template<typename Component> auto emplace_or_replace(auto&&... args) const -> decltype(auto);
    template<typename Component> auto try_emplace(auto&&... args) const -> decltype(auto);
    template<typename Component, std::invocable<Component&>... F> auto patch(F&&... funcs) const -> decltype(auto);
    template<typename Component> auto replace(auto&&... args) const -> decltype(auto);
    template<typename... Components> auto remove() const -> usize;
    template<typename... Components> auto erase() const -> void;
    template<typename... Components> [[nodiscard]] auto get() const -> decltype(auto);
    template<typename... Components> [[nodiscard]] auto try_get() const -> decltype(auto);
    template<typename Component> [[nodiscard]] auto get_or_emplace(auto&&... args) const -> decltype(auto);

    auto destroy() -> bool;
    auto destroy_unchecked() -> void;

    auto destroy_now() -> bool;
    auto destroy_now_unchecked() -> void;

    [[nodiscard]] auto registry() const -> Optional<Reference<Registry>>;
};

// The Registry stores all the entities and their components and provides methods to create and remove entities, modify
// their components, and to iterate through all existing entities and components.
class Registry
{
public:
    explicit Registry() = default;
    ZTH_NO_COPY_NO_MOVE(Registry) // Moving the registry would invalidate the references that listener adapters hold.
    ~Registry();

    [[nodiscard]] auto valid(EntityId id) const -> bool;

    auto create(const String& tag) -> EntityHandle;
    auto create(String&& tag = "Entity") -> EntityHandle;

    auto find_entity_by_tag(StringView tag) -> Optional<EntityHandle>;
    auto find_entities_by_tag(StringView tag) -> TemporaryVector<EntityHandle>;

    template<typename Component> auto emplace(EntityId id, auto&&... args) -> decltype(auto);
    template<typename Component> auto emplace_or_replace(EntityId id, auto&&... args) -> decltype(auto);
    template<typename Component> auto try_emplace(EntityId id, auto&&... args) -> decltype(auto);
    template<typename... Components> auto clear() -> void;
    template<typename Component, std::invocable<Component&>... F>
    auto patch(EntityId id, F&&... funcs) -> decltype(auto);
    template<typename Component> auto replace(EntityId id, auto&&... args) -> decltype(auto);
    template<typename... Components> auto remove(EntityId id) -> usize;
    template<typename... Components> auto erase(EntityId id) -> void;
    template<typename... Components> [[nodiscard]] auto all_of(EntityId id) const -> bool;
    template<typename... Components> [[nodiscard]] auto any_of(EntityId id) const -> bool;
    template<typename... Components> [[nodiscard]] auto get(this auto&& self, EntityId id) -> decltype(auto);
    template<typename... Components> [[nodiscard]] auto try_get(this auto&& self, EntityId id) -> auto;
    template<typename Component>
    [[nodiscard]] auto get_or_emplace(this auto&& self, EntityId id, auto&&... args) -> decltype(auto);

    auto clear() -> void;

    template<typename Component, auto Listener>
        requires(std::invocable<decltype(Listener), Registry&, EntityId>)
    auto add_on_attach_listener() -> void;

    template<typename Component, auto Listener>
        requires(std::invocable<decltype(Listener), Registry&, EntityId>)
    auto remove_on_attach_listener() -> void;

    template<typename Component, auto Listener>
        requires(std::invocable<decltype(Listener), Registry&, EntityId>)
    auto add_on_detach_listener() -> void;

    template<typename Component, auto Listener>
        requires(std::invocable<decltype(Listener), Registry&, EntityId>)
    auto remove_on_detach_listener() -> void;

    auto destroy(EntityId id) -> bool;
    auto destroy(EntityHandle& entity) -> bool;
    auto destroy_unchecked(EntityId id) -> void;
    auto destroy_unchecked(EntityHandle& entity) -> void;

    auto destroy_now(EntityId id) -> bool;
    auto destroy_now(EntityHandle& entity) -> bool;
    auto destroy_now_unchecked(EntityId id) -> void;
    auto destroy_now_unchecked(EntityHandle& entity) -> void;

    template<typename... Components, typename... Exclude>
    [[nodiscard]] auto view(this auto&& self, ExcludeComponents<Exclude...> exclude = ExcludeComponents{})
        -> decltype(auto);

    template<typename... Components, typename... Get, typename... Exclude>
    [[nodiscard]] auto group(this auto&& self, GetComponents<Get...> get = GetComponents{},
                             ExcludeComponents<Exclude...> exclude = ExcludeComponents{}) -> decltype(auto);

    template<typename... Components> auto sort() -> void;

private:
    entt::registry _registry;

private:
    template<auto Listener>
        requires(std::invocable<decltype(Listener), Registry&, EntityId>)
    auto listener_adapter(entt::registry& registry, entt::entity entity) -> void;
};

} // namespace zth

#include "ecs.inl"
