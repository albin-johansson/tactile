/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "component_system.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "context_system.hpp"
#include "core/components/attributes.hpp"
#include "misc/assert.hpp"
#include "misc/logging.hpp"
#include "misc/throw.hpp"
#include "registry_system.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _get_component_def_attr(const entt::registry& registry,
                                           const component_id id,
                                           const std::string_view attribute)
    -> decltype(comp::ComponentDef::attributes)::const_iterator
{
  const auto [defEntity, def] = get_component_def(registry, id);
  if (auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    return iter;
  }
  else {
    throw_traced(TactileError{"Invalid component attribute name!"});
  }
}

[[nodiscard]] auto _get_component_def_attr(entt::registry& registry,
                                           const component_id id,
                                           const std::string_view attribute)
    -> decltype(comp::ComponentDef::attributes)::iterator
{
  const auto [defEntity, def] = get_component_def(registry, id);
  if (auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    return iter;
  }
  else {
    throw_traced(TactileError{"Invalid component attribute name!"});
  }
}

[[nodiscard]] auto _get_component_value(comp::Component& component,
                                        const std::string_view attribute)
    -> decltype(comp::Component::values)::iterator
{
  if (auto iter = component.values.find(attribute); iter != component.values.end()) {
    return iter;
  }
  else {
    throw_traced(TactileError{"Invalid component attribute name!"});
  }
}

[[nodiscard]] auto _get_component(entt::registry& registry,
                                  const context_id contextId,
                                  const component_id componentId) -> comp::Component&
{
  auto& context = get_context(registry, contextId);

  for (const auto componentEntity : context.components) {
    auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == componentId) {
      return component;
    }
  }

  throw_traced(TactileError{"Did not find component!"});
}

}  // namespace

auto make_component_def(entt::registry& registry, std::string name) -> component_id
{
  static component_id next_id = 1;

  TACTILE_ASSERT(!is_component_name_taken(registry, name));
  TACTILE_ASSERT_MSG(!is_valid_component(registry, next_id),
                     "Verify use of use explicit component identifiers!");

  log_debug("Creating component definition '{}' (ID '{}')", name, next_id);

  const auto entity = registry.create();

  auto& def = registry.emplace<comp::ComponentDef>(entity);
  def.id = next_id;
  def.name = std::move(name);

  ++next_id;

  return def.id;
}

void make_component_def(entt::registry& registry, const component_id id, std::string name)
{
  TACTILE_ASSERT(!is_valid_component(registry, id));
  TACTILE_ASSERT(!is_component_name_taken(registry, name));

  log_debug("Creating component definition with explicit ID '{}'", id);

  const auto entity = registry.create();

  auto& def = registry.emplace<comp::ComponentDef>(entity);
  def.id = id;
  def.name = std::move(name);
}

auto remove_component_def(entt::registry& registry, const component_id id)
    -> RemoveComponentDefResult
{
  log_debug("Deleting component definition '{}'", id);

  const auto [defEntity, def] = get_component_def(registry, id);

  RemoveComponentDefResult result;
  result.id = id;
  result.name = def.name;
  result.attributes = def.attributes;

  /* Cache attributes from components that will be removed */
  auto cache = [&](const component_id componentId,
                   const comp::AttributeContext& context) {
    for (const auto componentEntity : context.components) {
      const auto& component = checked_get<comp::Component>(registry, componentEntity);
      if (component.type == componentId) {
        result.values[context.id] = component.values;
      }
    }
  };

  /* Removes component entities of the specified type */
  auto remove = [&, id](std::vector<entt::entity>& components) {
    std::erase_if(components, [&](const entt::entity componentEntity) {
      const auto& component = checked_get<comp::Component>(registry, componentEntity);
      return component.type == id;
    });
  };

  auto& root = registry.ctx<comp::AttributeContext>();
  cache(def.id, root);
  remove(root.components);

  for (auto&& [entity, context] : registry.view<comp::AttributeContext>().each()) {
    cache(def.id, context);
    remove(context.components);
  }

  registry.destroy(defEntity);

  return result;
}

void restore_component_def(entt::registry& registry, RemoveComponentDefResult snapshot)
{
  log_debug("Restoring component definition '{}'", snapshot.id);

  make_component_def(registry, snapshot.id, snapshot.name);
  for (auto&& [name, value] : snapshot.attributes) {
    make_component_attribute(registry, snapshot.id, name, value);
  }

  for (auto&& [contextId, values] : snapshot.values) {
    log_verbose("Restoring component '{}' for context '{}'", snapshot.id, contextId);

    auto& context = sys::get_context(registry, contextId);

    const auto componentEntity = registry.create();
    auto& component = registry.emplace<comp::Component>(componentEntity);
    component.type = snapshot.id;
    component.values = std::move(values);

    context.components.push_back(componentEntity);
  }
}

void rename_component_def(entt::registry& registry,
                          const component_id id,
                          std::string name)
{
  TACTILE_ASSERT(!sys::is_component_name_taken(registry, name));

  log_debug("Renaming component definition '{}' to '{}'", id, name);

  auto [entity, def] = get_component_def(registry, id);
  def.name = std::move(name);
}

auto is_valid_component(const entt::registry& registry, const component_id id) -> bool
{
  for (auto [entity, def] : registry.view<comp::ComponentDef>().each()) {
    if (def.id == id) {
      return true;
    }
  }

  return false;
}

auto is_component_name_taken(const entt::registry& registry, const std::string_view name)
    -> bool
{
  for (auto&& [entity, component] : registry.view<comp::ComponentDef>().each()) {
    if (component.name == name) {
      return true;
    }
  }

  return false;
}

auto get_first_available_component_def(const entt::registry& registry)
    -> Maybe<component_id>
{
  if (!registry.storage<comp::ComponentDef>().empty()) {
    const auto entity = registry.view<comp::ComponentDef>().front();
    const auto& def = checked_get<comp::ComponentDef>(registry, entity);
    return def.id;
  }
  else {
    return nothing;
  }
}

auto find_component_def(const entt::registry& registry, const component_id id)
    -> entt::entity
{
  for (auto&& [entity, component] : registry.view<comp::ComponentDef>().each()) {
    if (component.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto find_component_def(const entt::registry& registry, const std::string_view name)
    -> entt::entity
{
  for (auto&& [entity, def] : registry.view<comp::ComponentDef>().each()) {
    if (def.name == name) {
      return entity;
    }
  }

  return entt::null;
}

auto get_component_def(entt::registry& registry, const component_id id)
    -> std::pair<entt::entity, comp::ComponentDef&>
{
  const auto entity = find_component_def(registry, id);
  if (entity != entt::null) {
    return {entity, checked_get<comp::ComponentDef>(registry, entity)};
  }
  else {
    throw_traced(TactileError{"Failed to find component definition with specified ID!"});
  }
}

auto get_component_def(const entt::registry& registry, const component_id id)
    -> std::pair<entt::entity, const comp::ComponentDef&>
{
  const auto entity = find_component_def(registry, id);
  if (entity != entt::null) {
    return {entity, checked_get<comp::ComponentDef>(registry, entity)};
  }
  else {
    throw_traced(TactileError{"Failed to find component definition with specified ID!"});
  }
}

auto get_component_def_name(const entt::registry& registry, const component_id id)
    -> const std::string&
{
  const auto& [entity, def] = get_component_def(registry, id);
  return def.name;
}

void make_component_attribute(entt::registry& registry,
                              const component_id id,
                              const std::string& name)
{
  make_component_attribute(registry, id, name, std::string{});
}

void make_component_attribute(entt::registry& registry,
                              const component_id id,
                              const std::string& name,
                              const Attribute& value)
{
  log_debug("Adding attribute '{}' to component '{}'", name, id);

  auto [defEntity, def] = get_component_def(registry, id);
  def.attributes[name] = value;

  /* Updates existing components of the affected type to feature the new attribute */
  for (auto&& [entity, component] : registry.view<comp::Component>().each()) {
    if (component.type == id) {
      component.values[name] = value;
    }
  }
}

void remove_component_attribute(entt::registry& registry,
                                const component_id id,
                                const std::string_view name)
{
  log_debug("Removing attribute '{}' from component '{}'", name, id);

  TACTILE_ASSERT(find_component_def(registry, id) != entt::null);
  TACTILE_ASSERT(is_component_attribute_name_taken(registry, id, name));

  const auto removeAttributeFrom = [name](auto& map) {
    std::erase_if(map, [name](const auto& pair) { return pair.first == name; });
  };

  auto [defEntity, def] = get_component_def(registry, id);
  removeAttributeFrom(def.attributes);

  for (auto&& [entity, component] : registry.view<comp::Component>().each()) {
    if (component.type == id) {
      removeAttributeFrom(component.values);
    }
  }
}

void rename_component_attribute(entt::registry& registry,
                                const component_id id,
                                const std::string& current,
                                std::string updated)
{
  TACTILE_ASSERT(!is_component_attribute_name_taken(registry, id, updated));

  log_debug("Renaming attribute '{}' in component '{}' to '{}'", current, id, updated);

  auto [defEntity, def] = get_component_def(registry, id);

  TACTILE_ASSERT(def.attributes.contains(current));
  auto value = def.attributes.at(current);

  def.attributes.erase(current);
  def.attributes[std::move(updated)] = std::move(value);
}

auto duplicate_component_attribute(entt::registry& registry,
                                   const component_id id,
                                   const std::string_view attribute) -> std::string
{
  log_debug("Duplicating attribute '{}' in component '{}'", attribute, id);

  auto [defEntity, def] = get_component_def(registry, id);
  auto iter = _get_component_def_attr(registry, id, attribute);

  int suffix = 1;
  std::string candidateName;
  do {
    candidateName = fmt::format("{} ({})", attribute, suffix);
    ++suffix;
  } while (is_component_attribute_name_taken(registry, id, candidateName));

  def.attributes[candidateName] = iter->second;

  return candidateName;
}

void set_component_attribute_type(entt::registry& registry,
                                  const component_id id,
                                  const std::string_view attribute,
                                  const AttributeType type)
{
  log_verbose("Setting type of attribute '{}' in component '{}' to '{}'",
              attribute,
              id,
              type);

  auto iter = _get_component_attr(registry, id, attribute);
  iter->second.reset_to_default(type);
}

void set_component_attribute_value(entt::registry& registry,
                                   const component_id id,
                                   const std::string_view attribute,
                                   Attribute value)
{
  auto iter = _get_component_def_attr(registry, id, attribute);

  TACTILE_ASSERT_MSG(iter->second.type() == value.type(),
                     "Requested default value had wrong type!");
  iter->second = std::move(value);
}

auto get_component_attribute_type(const entt::registry& registry,
                                  const component_id id,
                                  const std::string_view attribute) -> AttributeType
{
  const auto iter = _get_component_def_attr(registry, id, attribute);
  return iter->second.type();
}

auto get_component_attribute_value(const entt::registry& registry,
                                   const component_id id,
                                   const std::string_view attribute) -> const Attribute&
{
  const auto iter = _get_component_def_attr(registry, id, attribute);
  return iter->second;
}

auto is_component_attribute_name_taken(const entt::registry& registry,
                                       const component_id id,
                                       const std::string_view name) -> bool
{
  const auto& [entity, def] = get_component_def(registry, id);

  for (const auto& [attributeName, attribute] : def.attributes) {
    if (attributeName == name) {
      return true;
    }
  }

  return false;
}

auto get_component_attribute_count(const entt::registry& registry, const component_id id)
    -> usize
{
  const auto [entity, def] = get_component_def(registry, id);
  return def.attributes.size();
}

auto add_component(entt::registry& registry,
                   const context_id context,
                   const component_id component) -> comp::Component&
{
  auto& ctx = get_context(registry, context);

  const auto componentEntity = registry.create();
  ctx.components.push_back(componentEntity);

  const auto [defEntity, def] = get_component_def(registry, component);
  auto& comp = registry.emplace<comp::Component>(componentEntity);
  comp.type = component;
  comp.values = def.attributes;

  return comp;
}

auto remove_component(entt::registry& registry,
                      const context_id contextId,
                      const component_id componentId) -> RemoveComponentResult
{
  TACTILE_ASSERT(find_component_def(registry, componentId) != entt::null);
  TACTILE_ASSERT(has_component(registry, contextId, componentId));

  log_debug("Removing component '{}' from context '{}'", componentId, contextId);

  auto& context = get_context(registry, contextId);
  entt::entity match = entt::null;

  RemoveComponentResult snapshot;
  snapshot.context = contextId;
  snapshot.component = componentId;

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == componentId) {
      snapshot.values = component.values;
      match = componentEntity;

      registry.destroy(componentEntity);
    }
  }

  TACTILE_ASSERT(match != entt::null);
  std::erase_if(context.components, [match](const entt::entity componentEntity) {
    return componentEntity == match;
  });

  return snapshot;
}

void restore_component(entt::registry& registry, RemoveComponentResult snapshot)
{
  TACTILE_ASSERT(find_component_def(registry, snapshot.component) != entt::null);
  TACTILE_ASSERT(!has_component(registry, snapshot.context, snapshot.component));

  log_debug("Restoring component '{}' for context '{}'",
            snapshot.component,
            snapshot.context);

  auto& context = get_context(registry, snapshot.context);

  const auto componentEntity = registry.create();
  context.components.push_back(componentEntity);

  auto& comp = registry.emplace<comp::Component>(componentEntity);
  comp.type = snapshot.component;
  comp.values = std::move(snapshot.values);
}

void update_component(entt::registry& registry,
                      const context_id contextId,
                      const component_id componentId,
                      const std::string_view attribute,
                      Attribute value)
{
  auto& component = _get_component(registry, contextId, componentId);

  auto iter = _get_component_value(component, attribute);
  iter->second = std::move(value);
}

auto reset_component(entt::registry& registry,
                     const context_id contextId,
                     const component_id componentId) -> ResetComponentResult
{
  TACTILE_ASSERT(has_component(registry, contextId, componentId));

  log_debug("Resetting component '{}' in context '{}'", componentId, contextId);

  const auto& [defEntity, def] = get_component_def(registry, componentId);
  auto& component = _get_component(registry, contextId, componentId);

  ResetComponentResult result;
  for (auto& [name, value] : component.values) {
    result.values[name] = value;
    value = def.attributes.at(name);
  }

  return result;
}

auto has_component(const entt::registry& registry,
                   const context_id contextId,
                   const component_id componentId) -> bool
{
  const auto& context = get_context(registry, contextId);

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == componentId) {
      return true;
    }
  }

  return false;
}

auto get_component(const entt::registry& registry,
                   const context_id contextId,
                   const component_id componentId) -> const comp::Component&
{
  const auto& context = get_context(registry, contextId);

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == componentId) {
      return component;
    }
  }

  throw_traced(TactileError{"Context did not feature requested component!"});
}

auto get_component_attribute(const entt::registry& registry,
                             const context_id contextId,
                             const component_id componentId,
                             const std::string_view attribute) -> const Attribute&
{
  const auto& context = get_context(registry, contextId);

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == componentId) {
      if (const auto it = component.values.find(attribute);
          it != component.values.end()) {
        return it->second;
      }
    }
  }

  throw_traced(TactileError{"Did not find component attribute!"});
}

auto get_component_count(const entt::registry& registry, const context_id contextId)
    -> usize
{
  return get_context(registry, contextId).components.size();
}

}  // namespace tactile::sys
