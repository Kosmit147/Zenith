#pragma once

#include <tuple>

namespace zth {

template<typename... Components> auto ConstEntityHandle::all_of() const -> bool
{
    return _registry->all_of<Components...>(*this);
}

template<typename... Components> auto ConstEntityHandle::any_of() const -> bool
{
    return _registry->any_of<Components...>(*this);
}

template<typename... Components> auto ConstEntityHandle::get() const -> decltype(auto)
{
    return static_cast<const Registry*>(_registry)->get<Components...>(*this);
}

template<typename... Components> auto ConstEntityHandle::try_get() const -> decltype(auto)
{
    return static_cast<const Registry*>(_registry)->try_get<Components...>(*this);
}

template<typename Component> auto ConstEntityHandle::get_or_emplace(auto&&... args) const -> decltype(auto)
{
    return static_cast<const Registry*>(_registry)->get_or_emplace<Component>(*this,
                                                                              std::forward<decltype(args)>(args)...);
}

template<typename Component> auto EntityHandle::emplace(auto&&... args) const -> decltype(auto)
{
    return _registry->emplace<Component>(*this, std::forward<decltype(args)>(args)...);
}

template<typename Component> auto EntityHandle::emplace_or_replace(auto&&... args) const -> decltype(auto)
{
    return _registry->emplace_or_replace<Component>(*this, std::forward<decltype(args)>(args)...);
}

template<typename Component> auto EntityHandle::try_emplace(auto&&... args) const -> decltype(auto)
{
    return _registry->try_emplace<Component>(*this, std::forward<decltype(args)>(args)...);
}

template<typename Component, std::invocable<Component&>... F>
auto EntityHandle::patch(F&&... funcs) const -> decltype(auto)
{
    return _registry->patch<Component>(*this, std::forward<decltype(funcs)>(funcs)...);
}

template<typename Component> auto EntityHandle::replace(auto&&... args) const -> decltype(auto)
{
    return _registry->replace<Component>(*this, std::forward<decltype(args)>(args)...);
}

template<typename... Components> auto EntityHandle::remove() const -> usize
{
    static_assert((!IntegralComponent<Components> && ...));
    return _registry->remove<Components...>(*this);
}

template<typename... Components> auto EntityHandle::erase() const -> void
{
    static_assert((!IntegralComponent<Components> && ...));
    _registry->erase<Components...>(*this);
}

template<typename... Components> auto EntityHandle::get() const -> decltype(auto)
{
    return _registry->get<Components...>(*this);
}

template<typename... Components> auto EntityHandle::try_get() const -> decltype(auto)
{
    return _registry->try_get<Components...>(*this);
}

template<typename Component> auto EntityHandle::get_or_emplace(auto&&... args) const -> decltype(auto)
{
    return _registry->get_or_emplace<Component>(*this, std::forward<decltype(args)>(args)...);
}

template<typename Component> auto Registry::emplace(EntityId id, auto&&... args) -> decltype(auto)
{
    return _registry.emplace<Component>(id, std::forward<decltype(args)>(args)...);
}

template<typename Component> auto Registry::emplace_or_replace(EntityId id, auto&&... args) -> decltype(auto)
{
    return _registry.emplace_or_replace<Component>(id, std::forward<decltype(args)>(args)...);
}

template<typename Component> auto Registry::try_emplace(EntityId id, auto&&... args) -> decltype(auto)
{
    if (any_of<Component>(id))
        return get<Component>(id);

    return emplace<Component>(id, std::forward<decltype(args)>(args)...);
}

template<typename... Components> auto Registry::clear() -> void
{
    static_assert((!IntegralComponent<Components> && ...));
    _registry.clear<Components...>();
}

template<typename Component, std::invocable<Component&>... F>
auto Registry::patch(EntityId id, F&&... funcs) -> decltype(auto)
{
    return _registry.patch<Component>(id, std::forward<decltype(funcs)>(funcs)...);
}

template<typename Component> auto Registry::replace(EntityId id, auto&&... args) -> decltype(auto)
{
    return _registry.replace<Component>(id, std::forward<decltype(args)>(args)...);
}

template<typename... Components> auto Registry::remove(EntityId id) -> usize
{
    static_assert((!IntegralComponent<Components> && ...));
    return _registry.remove<Components...>(id);
}

template<typename... Components> auto Registry::erase(EntityId id) -> void
{
    static_assert((!IntegralComponent<Components> && ...));
    _registry.erase<Components...>(id);
}

template<typename... Components> auto Registry::all_of(EntityId id) const -> bool
{
    return _registry.all_of<Components...>(id);
}

template<typename... Components> auto Registry::any_of(EntityId id) const -> bool
{
    return _registry.any_of<Components...>(id);
}

template<typename... Components> auto Registry::get(this auto&& self, EntityId id) -> decltype(auto)
{
    return self._registry.template get<Components...>(id);
}

template<typename... Components> auto Registry::try_get(this auto&& self, EntityId id) -> auto
{
    if constexpr (sizeof...(Components) == 1)
    {
        return make_optional_reference_from_pointer(self._registry.template try_get<Components...>(id));
    }
    else
    {
        return std::apply(
            [](auto&&... pointers) { return std::make_tuple(make_optional_reference_from_pointer(pointers)...); },
            self._registry.template try_get<Components...>(id));
    }
}

template<typename Component>
auto Registry::get_or_emplace(this auto&& self, EntityId id, auto&&... args) -> decltype(auto)
{
    return self._registry.template get_or_emplace<Component>(id, std::forward<decltype(args)>(args)...);
}

template<typename Component, auto Listener>
    requires(std::invocable<decltype(Listener), Registry&, EntityId>)
auto Registry::add_on_attach_listener() -> void
{
    _registry.on_construct<Component>().template connect<&Registry::listener_adapter<Listener>>(*this);
}

template<typename Component, auto Listener>
    requires(std::invocable<decltype(Listener), Registry&, EntityId>)
auto Registry::remove_on_attach_listener() -> void
{
    _registry.on_construct<Component>().template disconnect<&Registry::listener_adapter<Listener>>(*this);
}

template<typename Component, auto Listener>
    requires(std::invocable<decltype(Listener), Registry&, EntityId>)
auto Registry::add_on_detach_listener() -> void
{
    _registry.on_destroy<Component>().template connect<&Registry::listener_adapter<Listener>>(*this);
}

template<typename Component, auto Listener>
    requires(std::invocable<decltype(Listener), Registry&, EntityId>)
auto Registry::remove_on_detach_listener() -> void
{
    _registry.on_destroy<Component>().template disconnect<&Registry::listener_adapter<Listener>>(*this);
}

template<typename... Components, typename... Exclude>
auto Registry::view(this auto&& self, ExcludeComponents<Exclude...> exclude) -> decltype(auto)
{
    return self._registry.template view<Components...>(std::forward<decltype(exclude)>(exclude));
}

template<typename... Components, typename... Get, typename... Exclude>
auto Registry::group(this auto&& self, GetComponents<Get...> get, ExcludeComponents<Exclude...> exclude)
    -> decltype(auto)
{
    return self._registry.template group<Components...>(std::forward<decltype(get)>(get),
                                                        std::forward<decltype(exclude)>(exclude));
}

template<typename... Components> auto Registry::sort() -> void
{
    _registry.sort<Components...>();
}

template<auto Listener>
    requires(std::invocable<decltype(Listener), Registry&, EntityId>)
auto Registry::listener_adapter([[maybe_unused]] entt::registry& registry, entt::entity entity) -> void
{
    Listener(*this, static_cast<EntityId>(entity));
}

} // namespace zth
