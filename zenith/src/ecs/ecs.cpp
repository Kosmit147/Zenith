#include "zenith/ecs/ecs.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/ecs/components.hpp"

namespace zth {

ConstEntityHandle::ConstEntityHandle(EntityId id, Registry& registry) : _id{ id }, _registry{ &registry } {}

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

auto ConstEntityHandle::registry_unchecked() const -> const Registry&
{
    ZTH_ASSERT(_registry != nullptr);
    return *_registry;
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

auto EntityHandle::destroy() -> bool
{
    if (valid())
    {
        destroy_unchecked();
        return true;
    }

    return false;
}

auto EntityHandle::destroy_unchecked() -> void
{
    ZTH_ASSERT(valid());
    registry()->get().destroy_unchecked(*this);
}

auto EntityHandle::destroy_now() -> bool
{
    if (valid())
    {
        destroy_now_unchecked();
        return true;
    }

    return false;
}

auto EntityHandle::destroy_now_unchecked() -> void
{
    ZTH_ASSERT(valid());
    registry()->get().destroy_now_unchecked(*this);
}

auto EntityHandle::registry() const -> Optional<Reference<Registry>>
{
    if (_registry)
        return *_registry;
    else
        return nil;
}

auto EntityHandle::registry_unchecked() const -> Registry&
{
    ZTH_ASSERT(_registry != nullptr);
    return *_registry;
}

Registry::~Registry()
{
    clear();
}

auto Registry::valid(EntityId id) const -> bool
{
    return _registry.valid(id);
}

static_assert(IntegralComponent<TagComponent>);
static_assert(IntegralComponent<TransformComponent>);

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

auto Registry::clear() -> void
{
    _registry.clear();
}

auto Registry::destroy(EntityId id) -> bool
{
    if (_registry.valid(id))
    {
        destroy_unchecked(id);
        return true;
    }

    return false;
}

auto Registry::destroy(EntityHandle& entity) -> bool
{
    if (entity.valid())
    {
        destroy_unchecked(entity);
        return true;
    }

    return false;
}

auto Registry::destroy_unchecked(EntityId id) -> void
{
    emplace_or_replace<DeletionMarkerComponent>(id);
}

auto Registry::destroy_unchecked(EntityHandle& entity) -> void
{
    destroy_unchecked(entity.id());
    entity = EntityHandle::invalid;
}

auto Registry::destroy_now(EntityId id) -> bool
{
    if (_registry.valid(id))
    {
        destroy_now_unchecked(id);
        return true;
    }

    return false;
}

auto Registry::destroy_now(EntityHandle& entity) -> bool
{
    if (entity.valid())
    {
        destroy_now_unchecked(entity);
        return true;
    }

    return false;
}

auto Registry::destroy_now_unchecked(EntityId id) -> void
{
    _registry.destroy(id);
}

auto Registry::destroy_now_unchecked(EntityHandle& entity) -> void
{
    destroy_now_unchecked(entity.id());
    entity = EntityHandle::invalid;
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::EntityId, id)
{
    if (id == zth::null_entity)
        return ZTH_FORMAT_OUT("Null");
    else
        return ZTH_FORMAT_OUT("{}", std::to_underlying(id));
}
