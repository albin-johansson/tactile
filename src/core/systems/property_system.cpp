#include "property_system.hpp"

#include <algorithm>  // any_of
#include <cassert>    // assert
#include <utility>    // move
#include <vector>     // erase

#include "core/components/property.hpp"

namespace Tactile::Sys {

void AddProperty(entt::registry& registry, std::string name, const PropertyType type)
{
  auto& context = GetCurrentContext(registry);

  const auto entity = registry.create();
  auto& property = registry.emplace<Property>(entity);
  property.name = std::move(name);
  property.value.ResetToDefault(type);

  context.properties.push_back(entity);
}

void RemoveProperty(entt::registry& registry, const std::string_view name)
{
  assert(HasPropertyWithName(registry, name));

  const auto entity = FindProperty(registry, name);
  if (entity != entt::null)
  {
    auto& context = GetCurrentContext(registry);
    std::erase(context.properties, entity);
    registry.destroy(entity);
  }
}

void RenameProperty(entt::registry& registry,
                    std::string_view oldName,
                    std::string newName)
{
  assert(HasPropertyWithName(registry, oldName));

  const auto entity = FindProperty(registry, oldName);
  if (entity != entt::null)
  {
    auto& property = registry.get<Property>(entity);
    property.name = std::move(newName);
  }
}

void UpdateProperty(entt::registry& registry,
                    const std::string_view name,
                    PropertyValue value)
{
  assert(HasPropertyWithName(registry, name));

  const auto entity = FindProperty(registry, name);
  if (entity != entt::null)
  {
    auto& property = registry.get<Property>(entity);
    property.value = std::move(value);
  }
}

void ChangePropertyType(entt::registry& registry,
                        const std::string_view name,
                        const PropertyType type)
{
  assert(HasPropertyWithName(registry, name));

  const auto entity = FindProperty(registry, name);
  if (entity != entt::null)
  {
    auto& property = registry.get<Property>(entity);
    property.value.ResetToDefault(type);
  }
}

auto GetCurrentContext(entt::registry& registry) -> PropertyContext&
{
  const auto& current = registry.ctx<ActivePropertyContext>();
  return (current.entity != entt::null)
             ? registry.get<PropertyContext>(current.entity)
             : registry.ctx<PropertyContext>();
}

auto GetCurrentContext(const entt::registry& registry) -> const PropertyContext&
{
  const auto& current = registry.ctx<ActivePropertyContext>();
  return (current.entity != entt::null)
             ? registry.get<PropertyContext>(current.entity)
             : registry.ctx<PropertyContext>();
}

auto FindProperty(const entt::registry& registry, std::string_view name)
    -> entt::entity
{
  const auto& context = GetCurrentContext(registry);
  for (const auto propertyEntity : context.properties)
  {
    const auto& property = registry.get<Property>(propertyEntity);
    if (name == property.name)
    {
      return propertyEntity;
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
