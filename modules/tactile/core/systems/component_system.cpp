#include "component_system.hpp"

#include <utility>  // move

#include <tactile_stdlib.hpp>

#include "assert.hpp"
#include "logging.hpp"
#include "throw.hpp"

namespace Tactile::Sys {

auto CreateComponentDef(entt::registry& registry, std::string name) -> ComponentID
{
  static ComponentID next_id = 1;
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
  TACTILE_ASSERT(defEntity != entt::null);

  /* Removes component entities of the specified type from all component bundles */
  for (auto&& [entity, bundle] : registry.view<ComponentBundle>().each()) {
    std::erase_if(bundle.components, [&](const entt::entity componentEntity) {
      const auto& component = registry.get<Component>(componentEntity);
      return component.type == id;
    });
  }

  /* Removes the affected components from the pool */
  for (auto&& [entity, component] : registry.view<Component>().each()) {
    if (component.type == def.id) {
      registry.remove<Component>(entity);
    }
  }

  registry.destroy(defEntity);
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

void CreateComponentAttribute(entt::registry& registry,
                              const ComponentID id,
                              const std::string& name)
{
  LogDebug("Adding attribute '{}' to component '{}'", name, id);

  auto [defEntity, def] = GetComponentDef(registry, id);
  def.attributes[name] = PropertyType::String;
  def.defaults[name] = std::string{};

  /* Updates existing components of the affected type to feature the new attribute */
  for (auto&& [entity, component] : registry.view<Component>().each()) {
    if (component.type == id) {
      component.values[name] = std::string{};
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
  removeAttributeFrom(def.defaults);

  for (auto&& [entity, component] : registry.view<Component>().each()) {
    if (component.type == id) {
      removeAttributeFrom(component.values);
    }
  }
}

void SetComponentAttributeDefault(entt::registry& registry,
                                  const ComponentID id,
                                  const std::string& attribute,
                                  PropertyValue value)
{
  const auto [defEntity, def] = GetComponentDef(registry, id);
  def.defaults.at(attribute) = std::move(value);
}

auto GetComponentAttribute(const entt::registry& registry,
                           const entt::entity entity,
                           const ComponentID id,
                           const std::string_view attribute) -> const PropertyValue&
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<ComponentBundle>(entity));

  const auto& bundle = registry.get<ComponentBundle>(entity);
  for (const auto componentEntity : bundle.components) {
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

auto AddComponent(entt::registry& registry,
                  const entt::entity entity,
                  const ComponentID id) -> Component&
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<ComponentBundle>(entity));  // TODO auto add bundle?
  TACTILE_ASSERT(FindComponentDef(registry, id) != entt::null);
  TACTILE_ASSERT(!HasComponent(registry, entity, id));

  LogDebug("Adding component '{}' to entity '{}'", id, entity);

  auto& bundle = registry.get<ComponentBundle>(entity);

  const auto componentEntity = registry.create();
  bundle.components.push_back(componentEntity);

  const auto [defEntity, def] = GetComponentDef(registry, id);
  auto& component = registry.emplace<Component>(componentEntity);
  component.type = id;
  component.values = def.defaults;

  return component;
}

void RemoveComponent(entt::registry& registry,
                     const entt::entity entity,
                     const ComponentID id)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<ComponentBundle>(entity));
  TACTILE_ASSERT(FindComponentDef(registry, id) != entt::null);
  TACTILE_ASSERT(HasComponent(registry, entity, id));

  LogDebug("Removing component '{}' from entity '{}'", id, entity);

  auto& bundle = registry.get<ComponentBundle>(entity);
  entt::entity match = entt::null;

  for (const auto componentEntity : bundle.components) {
    const auto& component = registry.get<Component>(componentEntity);
    if (component.type == id) {
      match = componentEntity;
      registry.destroy(componentEntity);
    }
  }

  TACTILE_ASSERT(match != entt::null);
  std::erase_if(bundle.components, [match](const entt::entity componentEntity) {
    return componentEntity == match;
  });
}

auto HasComponent(entt::registry& registry,
                  const entt::entity entity,
                  const ComponentID id) -> bool
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<ComponentBundle>(entity));

  const auto& bundle = registry.get<ComponentBundle>(entity);
  for (const auto componentEntity : bundle.components) {
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
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<ComponentBundle>(entity));

  const auto& bundle = registry.get<ComponentBundle>(entity);
  for (const auto componentEntity : bundle.components) {
    const auto& component = registry.get<Component>(componentEntity);
    if (component.type == id) {
      return component;
    }
  }

  ThrowTraced(TactileError{"Entity did not feature requested component!"});
}

auto GetComponentCount(const entt::registry& registry, const entt::entity entity) -> usize
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<ComponentBundle>(entity));

  const auto& bundle = registry.get<ComponentBundle>(entity);
  return bundle.components.size();
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
