#include "property_system.hpp"

#include <utility>  // move
#include <vector>   // erase

#include "assert.hpp"
#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile::sys {

void add_property(entt::registry& registry,
                  comp::attribute_context& context,
                  std::string name,
                  const attribute_type type)
{
  TACTILE_ASSERT(!has_property_with_name(registry, context, name));

  const auto entity = registry.create();

  auto& property = registry.emplace<comp::property>(entity);
  property.name = std::move(name);
  property.value.reset_to_default(type);

  context.properties.push_back(entity);
}

void add_property(entt::registry& registry,
                  comp::attribute_context& context,
                  std::string name,
                  attribute_value value)
{
  TACTILE_ASSERT(!has_property_with_name(registry, context, name));

  const auto entity = registry.create();

  auto& property = registry.emplace<comp::property>(entity);
  property.name = std::move(name);
  property.value = std::move(value);

  context.properties.push_back(entity);
}

void remove_property(entt::registry& registry,
                     comp::attribute_context& context,
                     const std::string_view name)
{
  const auto entity = find_property(registry, context, name);
  TACTILE_ASSERT(entity != entt::null);

  std::erase(context.properties, entity);
  registry.destroy(entity);
}

void rename_property(entt::registry& registry,
                     comp::attribute_context& context,
                     const std::string_view oldName,
                     std::string newName)
{
  TACTILE_ASSERT(has_property_with_name(registry, context, oldName));
  TACTILE_ASSERT(!has_property_with_name(registry, context, newName));

  const auto entity = find_property(registry, context, oldName);
  TACTILE_ASSERT(entity != entt::null);

  auto& property = registry.get<comp::property>(entity);
  property.name = std::move(newName);
}

void update_property(entt::registry& registry,
                     comp::attribute_context& context,
                     const std::string_view name,
                     attribute_value value)
{
  const auto entity = find_property(registry, context, name);
  TACTILE_ASSERT(entity != entt::null);

  auto& property = registry.get<comp::property>(entity);
  property.value = std::move(value);
}

void change_property_type(entt::registry& registry,
                          comp::attribute_context& context,
                          const std::string_view name,
                          const attribute_type type)
{
  const auto entity = find_property(registry, context, name);
  TACTILE_ASSERT(entity != entt::null);

  auto& property = registry.get<comp::property>(entity);
  property.value.reset_to_default(type);
}

auto find_property(const entt::registry& registry,
                   const comp::attribute_context& context,
                   const std::string_view name) -> entt::entity
{
  for (const auto entity : context.properties) {
    const auto& property = registry.get<comp::property>(entity);
    if (property.name == name) {
      return entity;
    }
  }

  return entt::null;
}

auto get_property(const entt::registry& registry,
                  const comp::attribute_context& context,
                  const std::string_view name) -> const comp::property&
{
  const auto entity = find_property(registry, context, name);
  if (entity != entt::null) {
    return registry.get<comp::property>(entity);
  }
  else {
    throw_traced(tactile_error{"Found no property with the specified name!"});
  }
}

auto has_property_with_name(const entt::registry& registry,
                            const comp::attribute_context& context,
                            const std::string_view name) -> bool
{
  return find_property(registry, context, name) != entt::null;
}

}  // namespace tactile::sys
