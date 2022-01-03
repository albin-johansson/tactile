#include "component_system.hpp"

#include <utility>  // move

#include <fmt/format.h>
#include <tactile_stdlib.hpp>

#include "assert.hpp"
#include "logging.hpp"
#include "throw.hpp"

#include "core/components/property_context.hpp"

namespace Tactile::Sys {
namespace {

[[nodiscard]] auto GetComponentAttribute(const entt::registry& registry,
                                         const ComponentID id,
                                         const std::string_view attribute)
    -> decltype(ComponentDef::attributes)::const_iterator
{
  const auto [defEntity, def] = GetComponentDef(registry, id);
  if (const auto it = def.attributes.find(attribute); it != def.attributes.end()) {
    return it;
  }
  else {
    ThrowTraced(TactileError{"Invalid component attribute name!"});
  }
}

[[nodiscard]] auto GetComponentAttribute(entt::registry& registry,
                                         const ComponentID id,
                                         const std::string_view attribute)
    -> decltype(ComponentDef::attributes)::iterator
{
  const auto [defEntity, def] = GetComponentDef(registry, id);
  if (const auto it = def.attributes.find(attribute); it != def.attributes.end()) {
    return it;
  }
  else {
    ThrowTraced(TactileError{"Invalid component attribute name!"});
  }
}

}  // namespace

void CreateComponentDef(entt::registry& registry, const ComponentID id, std::string name)
{
  TACTILE_ASSERT(!IsComponentValid(registry, id));
  TACTILE_ASSERT(!IsComponentNameTaken(registry, name));

  LogDebug("Creating component definition with explicit ID '{}'", id);

  const auto entity = registry.create();

  auto& def = registry.emplace<ComponentDef>(entity);
  def.id = id;
  def.name = std::move(name);
}

auto CreateComponentDef(entt::registry& registry, std::string name) -> ComponentID
{
  static ComponentID next_id = 1;

  TACTILE_ASSERT(!IsComponentNameTaken(registry, name));
  TACTILE_ASSERT_MSG(!IsComponentValid(registry, next_id),
                     "Verify use of use explicit component identifiers!");

  LogDebug("Creating component definition '{}' (ID '{}')", name, next_id);

  const auto entity = registry.create();

  auto& def = registry.emplace<ComponentDef>(entity);
  def.id = next_id;
  def.name = std::move(name);

  ++next_id;

  return def.id;
}

void RemoveComponentDef(entt::registry& registry, const ComponentID id)
{
  LogDebug("Deleting component definition '{}'", id);

  const auto [defEntity, def] = GetComponentDef(registry, id);

  /* Removes component entities of the specified type from all component bundles */
  auto removeComponentsFrom = [&, id](std::vector<entt::entity>& components) {
    std::erase_if(components, [&](const entt::entity componentEntity) {
      const auto& component = registry.get<Component>(componentEntity);
      return component.type == id;
    });
  };

  auto& root = registry.ctx<PropertyContext>();
  removeComponentsFrom(root.components);

  for (auto&& [entity, context] : registry.view<PropertyContext>().each()) {
    removeComponentsFrom(context.components);
  }

  /* Removes the affected components from the pool */
  for (auto&& [entity, component] : registry.view<Component>().each()) {
    if (component.type == def.id) {
      registry.remove<Component>(entity);
    }
  }

  registry.destroy(defEntity);
}

void RenameComponentDef(entt::registry& registry, const ComponentID id, std::string name)
{
  TACTILE_ASSERT(!Sys::IsComponentNameTaken(registry, name));

  LogDebug("Renaming component definition '{}' to '{}'", id, name);

  auto [entity, def] = GetComponentDef(registry, id);
  def.name = std::move(name);
}

auto FindComponentDef(const entt::registry& registry, const ComponentID id)
    -> entt::entity
{
  for (auto&& [entity, component] : registry.view<ComponentDef>().each()) {
    if (component.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto GetComponentDef(entt::registry& registry, const ComponentID id)
    -> std::pair<entt::entity, ComponentDef&>
{
  const auto entity = FindComponentDef(registry, id);
  if (entity != entt::null) {
    return {entity, registry.get<ComponentDef>(entity)};
  }
  else {
    ThrowTraced(TactileError{"Failed to find component definition with specified ID!"});
  }
}

auto GetComponentDef(const entt::registry& registry, const ComponentID id)
    -> std::pair<entt::entity, const ComponentDef&>
{
  const auto entity = FindComponentDef(registry, id);
  if (entity != entt::null) {
    return {entity, registry.get<ComponentDef>(entity)};
  }
  else {
    ThrowTraced(TactileError{"Failed to find component definition with specified ID!"});
  }
}

auto GetComponentDefName(const entt::registry& registry, const ComponentID id)
    -> const std::string&
{
  const auto& [entity, def] = GetComponentDef(registry, id);
  return def.name;
}

auto GetFirstAvailableComponentDef(const entt::registry& registry) -> Maybe<ComponentID>
{
  if (!registry.storage<ComponentDef>().empty()) {
    const auto entity = registry.view<ComponentDef>().front();
    const auto& def = registry.get<ComponentDef>(entity);
    return def.id;
  }
  else {
    return nothing;
  }
}

auto IsComponentValid(const entt::registry& registry, const ComponentID id) -> bool
{
  for (auto [entity, def] : registry.view<ComponentDef>().each()) {
    if (def.id == id) {
      return true;
    }
  }

  return false;
}

void CreateComponentAttribute(entt::registry& registry,
                              const ComponentID id,
                              const std::string& name)
{
  CreateComponentAttribute(registry, id, name, std::string{});
}

void CreateComponentAttribute(entt::registry& registry,
                              const ComponentID id,
                              const std::string& name,
                              const PropertyValue& value)
{
  LogDebug("Adding attribute '{}' to component '{}'", name, id);

  auto [defEntity, def] = GetComponentDef(registry, id);
  def.attributes[name] = value;

  /* Updates existing components of the affected type to feature the new attribute */
  for (auto&& [entity, component] : registry.view<Component>().each()) {
    if (component.type == id) {
      component.values[name] = value;
    }
  }
}

void RemoveComponentAttribute(entt::registry& registry,
                              const ComponentID id,
                              const std::string_view name)
{
  LogDebug("Removing attribute '{}' from component '{}'", name, id);

  TACTILE_ASSERT(FindComponentDef(registry, id) != entt::null);
  TACTILE_ASSERT(IsComponentAttributeNameTaken(registry, id, name));

  const auto removeAttributeFrom = [name](auto& map) {
    std::erase_if(map, [name](const auto& pair) { return pair.first == name; });
  };

  auto [defEntity, def] = GetComponentDef(registry, id);
  removeAttributeFrom(def.attributes);

  for (auto&& [entity, component] : registry.view<Component>().each()) {
    if (component.type == id) {
      removeAttributeFrom(component.values);
    }
  }
}

void RenameComponentAttribute(entt::registry& registry,
                              const ComponentID id,
                              const std::string& current,
                              std::string updated)
{
  TACTILE_ASSERT(!IsComponentAttributeNameTaken(registry, id, updated));

  LogDebug("Renaming attribute '{}' in component '{}' to '{}'", current, id, updated);

  auto [defEntity, def] = GetComponentDef(registry, id);

  TACTILE_ASSERT(def.attributes.contains(current));
  auto value = def.attributes.at(current);

  def.attributes.erase(current);
  def.attributes[std::move(updated)] = std::move(value);
}

void DuplicateComponentAttribute(entt::registry& registry,
                                 const ComponentID id,
                                 const std::string_view attribute)
{
  LogDebug("Duplicating attribute '{}' in component '{}'", attribute, id);

  auto [defEntity, def] = GetComponentDef(registry, id);
  auto iter = GetComponentAttribute(registry, id, attribute);

  int suffix = 1;
  std::string candidateName;
  do {
    candidateName = fmt::format("{} ({})", attribute, suffix);
    ++suffix;
  } while (IsComponentAttributeNameTaken(registry, id, candidateName));

  def.attributes[std::move(candidateName)] = iter->second;
}

void SetComponentAttributeType(entt::registry& registry,
                               const ComponentID id,
                               const std::string_view attribute,
                               const PropertyType type)
{
  LogVerbose("Setting type of attribute '{}' in component '{}' to '{}'",
             attribute,
             id,
             type);

  auto iter = GetComponentAttribute(registry, id, attribute);
  iter->second.ResetToDefault(type);
}

void SetComponentAttributeValue(entt::registry& registry,
                                const ComponentID id,
                                const std::string_view attribute,
                                PropertyValue value)
{
  auto iter = GetComponentAttribute(registry, id, attribute);

  TACTILE_ASSERT_MSG(iter->second.GetType() == value.GetType(),
                     "Requested default value had wrong type!");
  iter->second = std::move(value);
}

auto GetComponentAttributeType(const entt::registry& registry,
                               const ComponentID id,
                               const std::string_view attribute) -> PropertyType
{
  const auto iter = GetComponentAttribute(registry, id, attribute);
  return iter->second.GetType().value();
}

auto GetComponentAttributeValue(const entt::registry& registry,
                                const ComponentID id,
                                const std::string_view attribute) -> const PropertyValue&
{
  const auto iter = GetComponentAttribute(registry, id, attribute);
  return iter->second;
}

auto IsComponentAttributeNameTaken(const entt::registry& registry,
                                   const ComponentID id,
                                   const std::string_view name) -> bool
{
  const auto& [entity, def] = GetComponentDef(registry, id);

  for (const auto& [attributeName, attribute] : def.attributes) {
    if (attributeName == name) {
      return true;
    }
  }

  return false;
}

auto GetComponentAttributeCount(const entt::registry& registry, const ComponentID id)
    -> usize
{
  const auto [entity, def] = GetComponentDef(registry, id);
  return def.attributes.size();
}

auto AddComponent(entt::registry& registry,
                  const entt::entity entity,
                  const ComponentID id) -> Component&
{
  TACTILE_ASSERT(FindComponentDef(registry, id) != entt::null);
  TACTILE_ASSERT(!HasComponent(registry, entity, id));

  LogDebug("Adding component '{}' to entity '{}'", id, entity);

  auto& context = entity != entt::null ? registry.get<PropertyContext>(entity)
                                       : registry.ctx<PropertyContext>();

  const auto componentEntity = registry.create();
  context.components.push_back(componentEntity);

  const auto [defEntity, def] = GetComponentDef(registry, id);
  auto& component = registry.emplace<Component>(componentEntity);
  component.type = id;
  component.values = def.attributes;

  return component;
}

void RemoveComponent(entt::registry& registry,
                     const entt::entity entity,
                     const ComponentID id)
{
  TACTILE_ASSERT(FindComponentDef(registry, id) != entt::null);
  TACTILE_ASSERT(HasComponent(registry, entity, id));

  LogDebug("Removing component '{}' from entity '{}'", id, entity);

  auto& context = entity != entt::null ? registry.get<PropertyContext>(entity)
                                       : registry.ctx<PropertyContext>();
  entt::entity match = entt::null;

  for (const auto componentEntity : context.components) {
    const auto& component = registry.get<Component>(componentEntity);
    if (component.type == id) {
      match = componentEntity;
      registry.destroy(componentEntity);
    }
  }

  TACTILE_ASSERT(match != entt::null);
  std::erase_if(context.components, [match](const entt::entity componentEntity) {
    return componentEntity == match;
  });
}

auto HasComponent(entt::registry& registry,
                  const entt::entity entity,
                  const ComponentID id) -> bool
{
  const auto& context = entity != entt::null ? registry.get<PropertyContext>(entity)
                                             : registry.ctx<PropertyContext>();
  for (const auto componentEntity : context.components) {
    const auto& component = registry.get<Component>(componentEntity);
    if (component.type == id) {
      return true;
    }
  }

  return false;
}

auto GetComponent(const entt::registry& registry,
                  const entt::entity entity,
                  const ComponentID id) -> const Component&
{
  const auto& context = entity != entt::null ? registry.get<PropertyContext>(entity)
                                             : registry.ctx<PropertyContext>();
  for (const auto componentEntity : context.components) {
    const auto& component = registry.get<Component>(componentEntity);
    if (component.type == id) {
      return component;
    }
  }

  ThrowTraced(TactileError{"Entity did not feature requested component!"});
}

auto GetComponentAttribute(const entt::registry& registry,
                           const entt::entity entity,
                           const ComponentID id,
                           const std::string_view attribute) -> const PropertyValue&
{
  const auto& context = entity != entt::null ? registry.get<PropertyContext>(entity)
                                             : registry.ctx<PropertyContext>();

  for (const auto componentEntity : context.components) {
    const auto& component = registry.get<Component>(componentEntity);
    if (component.type == id) {
      if (const auto it = component.values.find(attribute);
          it != component.values.end()) {
        return it->second;
      }
    }
  }

  ThrowTraced(TactileError{"Did not find component attribute!"});
}

auto GetComponentCount(const entt::registry& registry, const entt::entity entity) -> usize
{
  const auto& context = entity != entt::null ? registry.get<PropertyContext>(entity)
                                             : registry.ctx<PropertyContext>();
  return context.components.size();
}

auto IsComponentNameTaken(const entt::registry& registry, const std::string_view name)
    -> bool
{
  for (auto&& [entity, component] : registry.view<ComponentDef>().each()) {
    if (component.name == name) {
      return true;
    }
  }

  return false;
}

}  // namespace Tactile::Sys
