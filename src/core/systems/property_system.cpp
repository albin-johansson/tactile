#include "property_system.hpp"

#include <algorithm>  // any_of
#include <cassert>    // assert
#include <utility>    // move
#include <vector>     // erase

namespace Tactile::Sys {
namespace {

/* Identifier used to distinguish property contexts, generated on a
   session-by-session basis and not stored anywhere in save files. */
inline ContextID next_context_id{1};

}  // namespace

auto AddPropertyContext(entt::registry& registry, const entt::entity entity)
    -> PropertyContext&
{
  auto& context = registry.emplace<PropertyContext>(entity);

  context.id = next_context_id;
  ++next_context_id;

  return context;
}

void RestorePropertyContext(entt::registry& registry,
                            const entt::entity source,
                            PropertyContextSnapshot snapshot)
{
  auto& context = registry.get_or_emplace<PropertyContext>(source);
  context.id = snapshot.id;
  context.name = std::move(snapshot.name);

  for (auto [propertyName, propertyValue] : snapshot.properties) {
    const auto propertyEntity = registry.create();
    auto& property = registry.emplace<Property>(propertyEntity);
    property.name = propertyName;
    property.value = std::move(propertyValue);
  }
}

auto CopyPropertyContext(const entt::registry& registry, const entt::entity source)
    -> PropertyContextSnapshot
{
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

void AddProperty(entt::registry& registry,
                 const ContextID id,
                 std::string name,
                 const PropertyType type)
{
  auto& context = GetContext(registry, id);

  const auto entity = registry.create();
  auto& property = registry.emplace<Property>(entity);
  property.name = std::move(name);
  property.value.ResetToDefault(type);

  context.properties.push_back(entity);
}

void AddProperty(entt::registry& registry,
                 const ContextID id,
                 std::string name,
                 PropertyValue value)
{
  auto& context = GetContext(registry, id);

  const auto entity = registry.create();
  auto& property = registry.emplace<Property>(entity);
  property.name = std::move(name);
  property.value = std::move(value);

  context.properties.push_back(entity);
}

void RemoveProperty(entt::registry& registry,
                    const ContextID id,
                    const std::string_view name)
{
  auto& context = GetContext(registry, id);

  const auto entity = FindProperty(registry, context, name);
  assert(entity != entt::null);

  std::erase(context.properties, entity);
  registry.destroy(entity);
}

void RenameProperty(entt::registry& registry,
                    std::string_view oldName,
                    std::string newName)
{
  assert(HasPropertyWithName(registry, oldName));

  const auto entity = FindProperty(registry, oldName);
  if (entity != entt::null) {
    auto& property = registry.get<Property>(entity);
    property.name = std::move(newName);
  }
}

void RenameProperty(entt::registry& registry,
                    const ContextID id,
                    const std::string_view oldName,
                    std::string newName)
{
  auto& context = GetContext(registry, id);
  assert(FindProperty(registry, context, newName) == entt::null);

  const auto entity = FindProperty(registry, context, oldName);
  assert(entity != entt::null);

  auto& property = registry.get<Property>(entity);
  property.name = std::move(newName);
}

void UpdateProperty(entt::registry& registry,
                    const ContextID id,
                    const std::string_view name,
                    PropertyValue value)
{
  auto& context = GetContext(registry, id);

  const auto entity = FindProperty(registry, context, name);
  assert(entity != entt::null);

  auto& property = registry.get<Property>(entity);
  property.value = std::move(value);
}

void ChangePropertyType(entt::registry& registry,
                        const ContextID id,
                        const std::string_view name,
                        const PropertyType type)
{
  auto& context = GetContext(registry, id);

  const auto entity = FindProperty(registry, context, name);
  assert(entity != entt::null);

  auto& property = registry.get<Property>(entity);
  property.value.ResetToDefault(type);
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

auto GetContext(entt::registry& registry, const ContextID id) -> PropertyContext&
{
  for (auto&& [entity, context] : registry.view<PropertyContext>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  assert(registry.ctx<PropertyContext>().id == id);
  return registry.ctx<PropertyContext>();
}

auto GetContext(const entt::registry& registry, const ContextID id)
    -> const PropertyContext&
{
  for (auto&& [entity, context] : registry.view<PropertyContext>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  assert(registry.ctx<PropertyContext>().id == id);
  return registry.ctx<PropertyContext>();
}

auto GetPropertyValue(const entt::registry& registry,
                      const ContextID id,
                      const std::string_view name) -> const PropertyValue&
{
  const auto& context = GetContext(registry, id);

  const auto entity = FindProperty(registry, context, name);
  assert(entity != entt::null);

  const auto& property = registry.get<Property>(entity);
  return property.value;
}

auto FindProperty(const entt::registry& registry, const std::string_view name)
    -> entt::entity
{
  const auto& context = GetCurrentContext(registry);
  for (const auto propertyEntity : context.properties) {
    const auto& property = registry.get<Property>(propertyEntity);
    if (name == property.name) {
      return propertyEntity;
    }
  }

  return entt::null;
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

auto HasPropertyWithName(const entt::registry& registry, const std::string_view name)
    -> bool
{
  const auto& context = GetCurrentContext(registry);
  return std::ranges::any_of(context.properties, [&, name](entt::entity entity) {
    return registry.get<Property>(entity).name == name;
  });
}

}  // namespace Tactile::Sys
