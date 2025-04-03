#pragma once

#include <type_traits>

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
    static_assert((!IsIntegralComponent<Components> && ...));
    return _registry->remove<Components...>(*this);
}

template<typename... Components> auto EntityHandle::erase() const -> void
{
    static_assert((!IsIntegralComponent<Components> && ...));
    _registry->erase<Components...>(*this);
}

template<typename... Components> auto EntityHandle::get() const -> decltype(auto)
{
    return _registry->get<Components...>(*this);
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

template<typename... Components> auto Registry::clear() -> void
{
    static_assert((!IsIntegralComponent<Components> && ...));
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
    static_assert((!IsIntegralComponent<Components> && ...));
    return _registry.remove<Components...>(id);
}

template<typename... Components> auto Registry::erase(EntityId id) -> void
{
    static_assert((!IsIntegralComponent<Components> && ...));
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

template<typename Component>
auto Registry::get_or_emplace(this auto&& self, EntityId id, auto&&... args) -> decltype(auto)
{
    return self._registry.template get_or_emplace<Component>(id, std::forward<decltype(args)>(args)...);
}

template<typename... Components> auto Registry::view(this auto&& self) -> decltype(auto)
{
    return self._registry.template view<Components...>();
}

template<typename... Components> auto Registry::group(this auto&& self) -> decltype(auto)
{
    return self._registry.template group<Components...>();
}

template<typename... Components> auto Registry::sort() -> void
{
    _registry.sort<Components...>();
}

} // namespace zth
