#include "zenith/ecs/ecs.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/ecs/components.hpp"

namespace zth {

ConstEntityHandle::ConstEntityHandle(EntityId id, Registry& registry) : _id(id), _registry(&registry) {}

auto ConstEntityHandle::tag() const -> const TagComponent&
{
    return get<const TagComponent>();
}

auto ConstEntityHandle::transform() const -> const TransformComponent&
{
    return get<const TransformComponent>();
}

auto ConstEntityHandle::registry() const -> Optional<Reference<const Registry>>
{
    if (_registry)
        return *_registry;
    else
        return nil;
}

auto ConstEntityHandle::valid() const -> bool
{
    return _registry && _registry->valid(_id);
}

auto EntityHandle::tag() const -> TagComponent&
{
    return get<TagComponent>();
}

auto EntityHandle::transform() const -> TransformComponent&
{
    return get<TransformComponent>();
}

auto EntityHandle::destroy() -> void
{
    if (valid())
        destroy_unchecked();
}

auto EntityHandle::destroy_unchecked() -> void
{
    ZTH_ASSERT(valid());
    registry()->get().destroy_unchecked(*this);
}

auto EntityHandle::registry() const -> Optional<Reference<Registry>>
{
    if (_registry)
        return *_registry;
    else
        return nil;
}

auto Registry::valid(EntityId id) const -> bool
{
    return _registry.valid(id);
}

static_assert(IsIntegralComponent<TagComponent>);
static_assert(IsIntegralComponent<TransformComponent>);

auto Registry::create(const String& tag) -> EntityHandle
{
    EntityHandle entity{ _registry.create(), *this };

    entity.emplace<TagComponent>(tag);
    entity.emplace<TransformComponent>();

    return entity;
}

auto Registry::create(String&& tag) -> EntityHandle
{
    EntityHandle entity{ _registry.create(), *this };

    entity.emplace<TagComponent>(std::move(tag));
    entity.emplace<TransformComponent>();

    return entity;
}

auto Registry::find_entity_by_tag(StringView tag) -> Optional<EntityHandle>
{
    auto tags = view<const TagComponent>();

    for (auto&& [entity_id, tag_component] : tags.each())
    {
        if (tag_component.tag == tag)
            return EntityHandle{ entity_id, *this };
    }

    return nil;
}

auto Registry::find_entities_by_tag(StringView tag) -> TemporaryVector<EntityHandle>
{
    TemporaryVector<EntityHandle> entities;

    auto tags = view<const TagComponent>();

    for (auto&& [entity_id, tag_component] : tags.each())
    {
        if (tag_component.tag == tag)
            entities.emplace_back(entity_id, *this);
    }

    return entities;
}

auto Registry::destroy(EntityId id) -> void
{
    if (_registry.valid(id))
        destroy_unchecked(id);
}

auto Registry::destroy(EntityHandle& entity) -> void
{
    if (entity.valid())
        destroy_unchecked(entity);
}

auto Registry::destroy_unchecked(EntityId id) -> void
{
    _registry.destroy(id);
}

auto Registry::destroy_unchecked(EntityHandle& entity) -> void
{
    destroy_unchecked(entity.id());
    entity = EntityHandle::invalid;
}

} // namespace zth
