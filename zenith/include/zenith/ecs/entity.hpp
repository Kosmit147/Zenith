#pragma once

#include <entt/entity/entity.hpp>

namespace zth {

// Every entity is always required to have tag component and a transform component.
// @todo: Add methods to add / remove / retrieve / modify components.
class Entity
{
public:
    explicit Entity() = default;
    Entity(entt::entity id);

    [[nodiscard]] auto id() const { return _id; }
    operator entt::entity() const { return _id; }

private:
    entt::entity _id = entt::null;
};

} // namespace zth
