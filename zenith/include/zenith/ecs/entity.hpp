#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

#include <concepts>
#include <utility>

#include "zenith/core/typedefs.hpp"
#include "zenith/ecs/fwd.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"

namespace zth {

using EntityId = entt::entity;
constexpr auto null_entity = entt::null;

// A non-owning handle to an entity.
class ConstEntityHandle
{
public:
    struct InvalidHandle {};
    static constexpr InvalidHandle invalid;

public:
    ConstEntityHandle(InvalidHandle) {}
    ConstEntityHandle(EntityId id, Registry& registry);

    template<typename... Components> [[nodiscard]] auto all_of() const -> bool;
    template<typename... Components> [[nodiscard]] auto any_of() const -> bool;
    template<typename... Components> [[nodiscard]] auto get() const -> decltype(auto);
    template<typename Component> [[nodiscard]] auto get_or_emplace(auto&&... args) const -> Component&;

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

    template<typename Component> auto emplace(auto&&... args) const -> Component&;
    template<typename Component> auto emplace_or_replace(auto&&... args) const -> Component&;
    template<typename Component, std::invocable<Component&>... F> auto patch(F&&... funcs) const -> Component&;
    template<typename Component> auto replace(auto&&... args) const -> Component&;
    template<typename... Components> auto remove() const -> usize;
    template<typename... Components> auto erase() const -> void;
    template<typename... Components> [[nodiscard]] auto get() const -> decltype(auto);
    template<typename Component> [[nodiscard]] auto get_or_emplace(auto&&... args) const -> Component&;

    auto destroy() -> void;
    auto destroy_unchecked() -> void;

    [[nodiscard]] auto registry() const -> Optional<Reference<Registry>>;
};

class Registry
{
public:
    explicit Registry() = default;

    ZTH_NO_COPY(Registry)
    ZTH_DEFAULT_MOVE(Registry)

    ~Registry() = default;

    [[nodiscard]] auto valid(EntityId id) const -> bool;

    [[nodiscard]] auto create(const String& tag) -> EntityHandle;
    [[nodiscard]] auto create(String&& tag = "Entity") -> EntityHandle;

    template<typename Component> auto emplace(EntityId id, auto&&... args) -> Component&;
    template<typename Component> auto emplace_or_replace(EntityId id, auto&&... args) -> Component&;
    template<typename... Components> auto clear() -> void;
    template<typename Component, std::invocable<Component&>... F> auto patch(EntityId id, F&&... funcs) -> Component&;
    template<typename Component> auto replace(EntityId id, auto&&... args) -> Component&;
    template<typename... Components> auto remove(EntityId id) -> usize;
    template<typename... Components> auto erase(EntityId id) -> void;
    template<typename... Components> [[nodiscard]] auto all_of(EntityId id) const -> bool;
    template<typename... Components> [[nodiscard]] auto any_of(EntityId id) const -> bool;
    template<typename... Components> [[nodiscard]] auto get(this auto&& self, EntityId id) -> decltype(auto);
    template<typename Component>
    [[nodiscard]] auto get_or_emplace(this auto&& self, EntityId id, auto&&... args) -> Component&;

    auto destroy(EntityId id) -> void;
    auto destroy(EntityHandle& entity) -> void;

    auto destroy_unchecked(EntityId id) -> void;
    auto destroy_unchecked(EntityHandle& entity) -> void;

    // @todo: Add get / exclude functionality.
    template<typename... Components> [[nodiscard]] auto view(this auto&& self) -> decltype(auto);
    template<typename... Components> [[nodiscard]] auto group(this auto&& self) -> decltype(auto);
    template<typename... Components> auto sort() -> void;

private:
    entt::registry _registry;
};

} // namespace zth

#include "entity.inl"
