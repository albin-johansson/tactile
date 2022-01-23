#include "property_system.hpp"

#include <utility>  // move
#include <vector>   // erase

#include <tactile_stdlib.hpp>

#include "assert.hpp"
#include "throw.hpp"

namespace tactile::Sys {
namespace {

/* Identifier used to distinguish property contexts, generated on a
   session-by-session basis and not stored anywhere in save files. */
inline ContextID gNextContextId{1};

}  // namespace

auto AddPropertyContext(entt::registry& registry, const entt::entity entity)
    -> PropertyContext&
{
  TACTILE_ASSERT(entity != entt::null);
  auto& context = registry.emplace<PropertyContext>(entity);

  context.id = gNextContextId;
  ++gNextContextId;

  return context;
}

auto CopyPropertyContext(const entt::registry& registry, const entt::entity source)
    -> PropertyContextSnapshot
{
  TACTILE_ASSERT(source != entt::null);
  TACTILE_ASSERT(registry.all_of<PropertyContext>(source));
  const auto& context = registry.get<PropertyContext>(source);

  PropertyContextSnapshot snapshot;
  snapshot.id = context.id;
  snapshot.name = context.name;

  for (const auto propertyEntity : context.properties) {
    const auto& property = registry.get<Property>(propertyEntity);
    snapshot.properties.try_emplace(property.name, property.value);
  }

  return snapshot;
}

void RestorePropertyContext(entt::registry& registry,
                            const entt::entity entity,
                            PropertyContextSnapshot snapshot)
{
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get_or_emplace<PropertyContext>(entity);
  context.id = snapshot.id;
  context.name = std::move(snapshot.name);

  for (auto&& [propertyName, propertyValue] : snapshot.properties) {
    AddProperty(registry, context, propertyName, propertyValue);
  }
}

void AddProperty(entt::registry& registry,
                 PropertyContext& context,
                 std::string name,
                 const PropertyType type)
{
  TACTILE_ASSERT(!HasPropertyWithName(registry, context, name));

  const auto entity = registry.create();
  auto& property = registry.emplace<Property>(entity);
  property.name = std::move(name);
  property.value.reset_to_default(type);

  context.properties.push_back(entity);
}

void AddProperty(entt::registry& registry,
                 PropertyContext& context,
                 std::string name,
                 attribute_value value)
{
  TACTILE_ASSERT(!HasPropertyWithName(registry, context, name));

  const auto entity = registry.create();
  auto& property = registry.emplace<Property>(entity);
  property.name = std::move(name);
  property.value = std::move(value);

  context.properties.push_back(entity);
}

void RemoveProperty(entt::registry& registry,
                    PropertyContext& context,
                    const std::string_view name)
{
  const auto entity = FindProperty(registry, context, name);
  TACTILE_ASSERT(entity != entt::null);

  std::erase(context.properties, entity);
  registry.destroy(entity);
}

void RenameProperty(entt::registry& registry,
                    PropertyContext& context,
                    const std::string_view oldName,
                    std::string newName)
{
  TACTILE_ASSERT(HasPropertyWithName(registry, context, oldName));
  TACTILE_ASSERT(!HasPropertyWithName(registry, context, newName));

  const auto entity = FindProperty(registry, context, oldName);
  auto& property = registry.get<Property>(entity);
  property.name = std::move(newName);
}

void UpdateProperty(entt::registry& registry,
                    PropertyContext& context,
                    const std::string_view name,
                    attribute_value value)
{
  const auto entity = FindProperty(registry, context, name);
  TACTILE_ASSERT(entity != entt::null);

  auto& property = registry.get<Property>(entity);
  property.value = std::move(value);
}

void ChangePropertyType(entt::registry& registry,
                        PropertyContext& context,
                        const std::string_view name,
                        const PropertyType type)
{
  const auto entity = FindProperty(registry, context, name);
  TACTILE_ASSERT(entity != entt::null);

  auto& property = registry.get<Property>(entity);
  property.value.reset_to_default(type);
}

auto GetCurrentContext(entt::registry& registry) -> PropertyContext&
{
  const auto& current = registry.ctx<ActivePropertyContext>();
  return (current.entity != entt::null) ? registry.get<PropertyContext>(current.entity)
                                        : registry.ctx<PropertyContext>();
}

auto GetCurrentContext(const entt::registry& registry) -> const PropertyContext&
{
  const auto& current = registry.ctx<ActivePropertyContext>();
  return (current.entity != entt::null) ? registry.get<PropertyContext>(current.entity)
                                        : registry.ctx<PropertyContext>();
}

auto HasContext(const entt::registry& registry, const ContextID id) -> bool
{
  if (registry.ctx<PropertyContext>().id == id) {
    return true;
  }

  for (auto&& [entity, context] : registry.view<PropertyContext>().each()) {
    if (context.id == id) {
      return true;
    }
  }

  return false;
}

auto GetContext(entt::registry& registry, const ContextID id) -> PropertyContext&
{
  if (auto& context = registry.ctx<PropertyContext>(); context.id == id) {
    return context;
  }

  for (auto&& [entity, context] : registry.view<PropertyContext>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  ThrowTraced(TactileError{"No matching property context!"});
}

auto GetContext(const entt::registry& registry, const ContextID id)
    -> const PropertyContext&
{
  if (const auto& context = registry.ctx<PropertyContext>(); context.id == id) {
    return context;
  }

  for (auto&& [entity, context] : registry.view<PropertyContext>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  ThrowTraced(TactileError{"No matching property context!"});
}

auto GetCurrentContextId(const entt::registry& registry) -> ContextID
{
  const auto& context = GetCurrentContext(registry);
  return context.id;
}

auto FindProperty(const entt::registry& registry,
                  const PropertyContext& context,
                  const std::string_view name) -> entt::entity
{
  for (const auto entity : context.properties) {
    const auto& property = registry.get<Property>(entity);
    if (property.name == name) {
      return entity;
    }
  }

  return entt::null;
}

auto GetProperty(const entt::registry& registry,
                 const PropertyContext& context,
                 const std::string_view name) -> const Property&
{
  const auto entity = FindProperty(registry, context, name);
  if (entity != entt::null) {
    return registry.get<Property>(entity);
  }
  else {
    ThrowTraced(TactileError{"Found no property with the specified name!"});
  }
}

auto HasPropertyWithName(const entt::registry& registry,
                         const PropertyContext& context,
                         const std::string_view name) -> bool
{
  return FindProperty(registry, context, name) != entt::null;
}

void ResetNextContextId() noexcept
{
  gNextContextId = 1;
}

auto GetAndUpdateNextContextId() noexcept -> ContextID
{
  return gNextContextId++;
}

auto GetNextContextId() noexcept -> ContextID
{
  return gNextContextId;
}

}  // namespace tactile::Sys
