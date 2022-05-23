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

#include <boost/uuid/uuid_io.hpp>
#include <entt/entity/registry.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/systems/context_system.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _get_component_def_attr(const entt::registry& registry,
                                           const ComponentID& compId,
                                           const std::string_view attribute)
    -> decltype(comp::ComponentDef::attributes)::const_iterator
{
  const auto [defEntity, def] = get_component_def(registry, compId);
  if (auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    return iter;
  }
  else {
    panic("Invalid component attribute name!");
  }
}

[[nodiscard]] auto _get_component_def_attr(entt::registry& registry,
                                           const ComponentID& compId,
                                           const std::string_view attribute)
    -> decltype(comp::ComponentDef::attributes)::iterator
{
  const auto [defEntity, def] = get_component_def(registry, compId);
  if (auto iter = def.attributes.find(attribute); iter != def.attributes.end()) {
    return iter;
  }
  else {
    panic("Invalid component attribute name!");
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
    panic("Invalid component attribute name!");
  }
}

[[nodiscard]] auto _get_component(entt::registry& registry,
                                  const ContextID contextId,
                                  const ComponentID& compId) -> comp::Component&
{
  auto& context = get_context(registry, contextId);

  for (const auto componentEntity : context.components) {
    auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == compId) {
      return component;
    }
  }

  panic("Did not find component!");
}

void _visit_components(entt::registry& registry, auto callable)
{
  const auto& root = ctx_get<comp::AttributeContext>(registry);
  for (const auto compEntity : root.components) {
    auto& comp = checked_get<comp::Component>(registry, compEntity);
    callable(root.id, comp);
  }

  for (auto&& [entity, context, comp] :
       registry.view<comp::AttributeContext, comp::Component>().each()) {
    callable(context.id, comp);
  }
}

void _visit_components(const entt::registry& registry, auto callable)
{
  const auto& root = ctx_get<comp::AttributeContext>(registry);
  for (const auto compEntity : root.components) {
    const auto& comp = checked_get<comp::Component>(registry, compEntity);
    callable(root.id, comp);
  }

  for (auto&& [entity, context, comp] :
       registry.view<comp::AttributeContext, comp::Component>().each()) {
    callable(context.id, comp);
  }
}

void _visit_attributes(entt::registry& registry,
                       const ComponentID& compId,
                       const std::string_view attrName,
                       auto callable)
{
  _visit_components(registry, [=, &callable](const ContextID ctx, comp::Component& comp) {
    if (comp.type == compId) {
      if (auto iter = comp.values.find(attrName); iter != comp.values.end()) {
        callable(ctx, iter->second);
      }
    }
  });
}

void _visit_attributes(const entt::registry& registry,
                       const ComponentID& compId,
                       const std::string_view attrName,
                       auto callable)
{
  _visit_components(registry,
                    [=, &callable](const ContextID ctx, const comp::Component& comp) {
                      if (comp.type == compId) {
                        if (auto iter = comp.values.find(attrName);
                            iter != comp.values.end()) {
                          callable(ctx, iter->second);
                        }
                      }
                    });
}

[[nodiscard]] auto _collect_attributes(const entt::registry& registry,
                                       const ComponentID& compId,
                                       std::string_view attrName)
    -> HashMap<ContextID, Attribute>
{
  HashMap<ContextID, Attribute> attributes;

  _visit_attributes(registry,
                    compId,
                    attrName,
                    [&](const ContextID contextId, Attribute attr) {
                      attributes[contextId] = std::move(attr);
                    });

  return attributes;
}

}  // namespace

auto make_component_def(entt::registry& registry, std::string name) -> ComponentID
{
  TACTILE_ASSERT(!is_component_name_taken(registry, name));

  spdlog::debug("Creating component definition '{}'", name);

  const auto entity = registry.create();

  auto& def = registry.emplace<comp::ComponentDef>(entity);
  def.id = make_uuid();
  def.name = std::move(name);

  return def.id;
}

void make_component_def(entt::registry& registry,
                        const ComponentID& compId,
                        std::string name)
{
  TACTILE_ASSERT(!is_valid_component(registry, compId));
  TACTILE_ASSERT(!is_component_name_taken(registry, name));

  spdlog::debug("Creating component definition with explicit ID '{}'", compId);

  const auto entity = registry.create();

  auto& def = registry.emplace<comp::ComponentDef>(entity);
  def.id = compId;
  def.name = std::move(name);
}

auto remove_component_def(entt::registry& registry, const ComponentID& compId)
    -> RemoveComponentDefResult
{
  spdlog::debug("Deleting component definition '{}'", compId);

  const auto [defEntity, def] = get_component_def(registry, compId);

  RemoveComponentDefResult result;
  result.id = compId;
  result.name = def.name;
  result.attributes = def.attributes;

  /* Cache attributes from components that will be removed */
  auto cache = [&](const ComponentID& compId, const comp::AttributeContext& context) {
    for (const auto componentEntity : context.components) {
      const auto& component = checked_get<comp::Component>(registry, componentEntity);
      if (component.type == compId) {
        result.values[context.id] = component.values;
      }
    }
  };

  /* Removes component entities of the specified type */
  auto remove = [&, compId](std::vector<entt::entity>& components) {
    std::erase_if(components, [&](const entt::entity componentEntity) {
      const auto& component = checked_get<comp::Component>(registry, componentEntity);
      return component.type == compId;
    });
  };

  auto& root = ctx_get<comp::AttributeContext>(registry);
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
  spdlog::debug("Restoring component definition '{}'", snapshot.id);

  make_component_def(registry, snapshot.id, snapshot.name);
  for (auto&& [name, value] : snapshot.attributes) {
    make_component_attribute(registry, snapshot.id, name, value);
  }

  for (auto&& [contextId, values] : snapshot.values) {
    spdlog::trace("Restoring component '{}' for context '{}'", snapshot.id, contextId);

    auto& context = get_context(registry, contextId);

    const auto componentEntity = registry.create();
    auto& component = registry.emplace<comp::Component>(componentEntity);
    component.type = snapshot.id;
    component.values = std::move(values);

    context.components.push_back(componentEntity);
  }
}

void rename_component_def(entt::registry& registry,
                          const ComponentID& compId,
                          std::string name)
{
  TACTILE_ASSERT(!is_component_name_taken(registry, name));

  spdlog::debug("Renaming component definition '{}' to '{}'", compId, name);

  auto [entity, def] = get_component_def(registry, compId);
  def.name = std::move(name);
}

auto is_valid_component(const entt::registry& registry, const ComponentID& compId) -> bool
{
  return find_component_def(registry, compId) != entt::null;
}

auto is_component_name_taken(const entt::registry& registry, const std::string_view name)
    -> bool
{
  return find_component_def(registry, name) != entt::null;
}

auto get_first_available_component_def(const entt::registry& registry)
    -> Maybe<ComponentID>
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

auto find_component_def(const entt::registry& registry, const ComponentID& compId)
    -> entt::entity
{
  for (auto&& [entity, component] : registry.view<comp::ComponentDef>().each()) {
    if (component.id == compId) {
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

auto get_component_def(entt::registry& registry, const ComponentID& compId)
    -> std::pair<entt::entity, comp::ComponentDef&>
{
  const auto entity = find_component_def(registry, compId);
  if (entity != entt::null) {
    return {entity, checked_get<comp::ComponentDef>(registry, entity)};
  }
  else {
    panic("Failed to find component definition with specified ID!");
  }
}

auto get_component_def(const entt::registry& registry, const ComponentID& compId)
    -> std::pair<entt::entity, const comp::ComponentDef&>
{
  const auto entity = find_component_def(registry, compId);
  if (entity != entt::null) {
    return {entity, checked_get<comp::ComponentDef>(registry, entity)};
  }
  else {
    panic("Failed to find component definition with specified ID!");
  }
}

auto get_component_def_name(const entt::registry& registry, const ComponentID& compId)
    -> const std::string&
{
  const auto& [entity, def] = get_component_def(registry, compId);
  return def.name;
}

void make_component_attribute(entt::registry& registry,
                              const ComponentID& compId,
                              const std::string& name)
{
  make_component_attribute(registry, compId, name, std::string{});
}

void make_component_attribute(entt::registry& registry,
                              const ComponentID& compId,
                              const std::string& name,
                              const Attribute& value)
{
  spdlog::debug("Adding attribute '{}' to component '{}'", name, compId);

  auto [defEntity, def] = get_component_def(registry, compId);
  def.attributes[name] = value;

  /* Updates existing components of the affected type to feature the new attribute */
  for (auto&& [entity, component] : registry.view<comp::Component>().each()) {
    if (component.type == compId) {
      component.values[name] = value;
    }
  }
}

void remove_component_attribute(entt::registry& registry,
                                const ComponentID& compId,
                                const std::string_view name)
{
  spdlog::debug("Removing attribute '{}' from component '{}'", name, compId);

  TACTILE_ASSERT(find_component_def(registry, compId) != entt::null);
  TACTILE_ASSERT(is_component_attribute_name_taken(registry, compId, name));

  const auto removeAttributeFrom = [name](auto& map) {
    std::erase_if(map, [name](const auto& pair) { return pair.first == name; });
  };

  auto [defEntity, def] = get_component_def(registry, compId);
  removeAttributeFrom(def.attributes);

  for (auto&& [entity, component] : registry.view<comp::Component>().each()) {
    if (component.type == compId) {
      removeAttributeFrom(component.values);
    }
  }
}

void rename_component_attribute(entt::registry& registry,
                                const ComponentID& compId,
                                const std::string& current,
                                std::string updated)
{
  TACTILE_ASSERT(!is_component_attribute_name_taken(registry, compId, updated));

  spdlog::debug("Renaming attribute '{}' in component '{}' to '{}'",
                current,
                compId,
                updated);

  auto [defEntity, def] = get_component_def(registry, compId);

  TACTILE_ASSERT(def.attributes.contains(current));
  auto value = def.attributes.at(current);

  def.attributes.erase(current);
  def.attributes[std::move(updated)] = std::move(value);
}

auto duplicate_component_attribute(entt::registry& registry,
                                   const ComponentID& compId,
                                   const std::string_view attribute) -> std::string
{
  spdlog::debug("Duplicating attribute '{}' in component '{}'", attribute, compId);

  auto [defEntity, def] = get_component_def(registry, compId);
  auto iter = _get_component_def_attr(registry, compId, attribute);

  int suffix = 1;
  std::string candidateName;
  do {
    candidateName = fmt::format("{} ({})", attribute, suffix);
    ++suffix;
  } while (is_component_attribute_name_taken(registry, compId, candidateName));

  def.attributes[candidateName] = iter->second;

  return candidateName;
}

auto set_component_attribute_type(entt::registry& registry,
                                  const ComponentID& compId,
                                  const std::string_view attrName,
                                  const AttributeType type) -> SetComponentAttrTypeResult
{
  spdlog::trace("Setting type of attribute '{}' in component '{}' to '{}'",
                attrName,
                compId,
                type);

  SetComponentAttrTypeResult result;
  result.comp_id = compId;
  result.attr_name = std::string{attrName};
  result.values = _collect_attributes(registry, compId, attrName);

  auto iter = _get_component_def_attr(registry, compId, attrName);
  result.base_value = iter->second;

  iter->second.reset_to_default(type);

  _visit_attributes(registry, compId, attrName, [type](ContextID, Attribute& attr) {
    attr.reset_to_default(type);
  });

  return result;
}

void restore_component_attribute_type(entt::registry& registry,
                                      const SetComponentAttrTypeResult& snapshot)
{
  spdlog::trace("Restoring type of attribute '{}' in component '{}'",
                snapshot.attr_name,
                snapshot.comp_id);

  {
    auto&& [entity, def] = get_component_def(registry, snapshot.comp_id);
    def.attributes[snapshot.attr_name] = snapshot.base_value;
  }

  for (auto&& [ctx, attr] : snapshot.values) {
    auto& comp = _get_component(registry, ctx, snapshot.comp_id);
    auto iter = comp.values.find(snapshot.attr_name);
    iter->second = attr;
  }
}

void set_component_attribute_value(entt::registry& registry,
                                   const ComponentID& compId,
                                   const std::string_view attribute,
                                   Attribute value)
{
  auto iter = _get_component_def_attr(registry, compId, attribute);

  TACTILE_ASSERT_MSG(iter->second.type() == value.type(),
                     "Requested default value had wrong type!");
  iter->second = std::move(value);
}

auto get_component_attribute_type(const entt::registry& registry,
                                  const ComponentID& compId,
                                  const std::string_view attribute) -> AttributeType
{
  const auto iter = _get_component_def_attr(registry, compId, attribute);
  return iter->second.type();
}

auto get_component_attribute_value(const entt::registry& registry,
                                   const ComponentID& compId,
                                   const std::string_view attribute) -> const Attribute&
{
  const auto iter = _get_component_def_attr(registry, compId, attribute);
  return iter->second;
}

auto is_component_attribute_name_taken(const entt::registry& registry,
                                       const ComponentID& compId,
                                       const std::string_view name) -> bool
{
  const auto& [entity, def] = get_component_def(registry, compId);

  for (const auto& [attributeName, attribute] : def.attributes) {
    if (attributeName == name) {
      return true;
    }
  }

  return false;
}

auto get_component_attribute_count(const entt::registry& registry,
                                   const ComponentID& compId) -> usize
{
  const auto [entity, def] = get_component_def(registry, compId);
  return def.attributes.size();
}

auto add_component(entt::registry& registry,
                   const ContextID context,
                   const ComponentID& component) -> comp::Component&
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
                      const ContextID contextId,
                      const ComponentID& compId) -> RemoveComponentResult
{
  TACTILE_ASSERT(find_component_def(registry, compId) != entt::null);
  TACTILE_ASSERT(has_component(registry, contextId, compId));

  spdlog::debug("Removing component '{}' from context '{}'", compId, contextId);

  auto& context = get_context(registry, contextId);
  entt::entity match = entt::null;

  RemoveComponentResult snapshot;
  snapshot.context = contextId;
  snapshot.component = compId;

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == compId) {
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

  spdlog::debug("Restoring component '{}' for context '{}'",
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
                      const ContextID contextId,
                      const ComponentID& compId,
                      const std::string_view attribute,
                      Attribute value)
{
  auto& component = _get_component(registry, contextId, compId);

  auto iter = _get_component_value(component, attribute);
  iter->second = std::move(value);
}

auto reset_component(entt::registry& registry,
                     const ContextID contextId,
                     const ComponentID& compId) -> ResetComponentResult
{
  TACTILE_ASSERT(has_component(registry, contextId, compId));

  spdlog::debug("Resetting component '{}' in context '{}'", compId, contextId);

  const auto& [defEntity, def] = get_component_def(registry, compId);
  auto& component = _get_component(registry, contextId, compId);

  ResetComponentResult result;
  for (auto& [name, value] : component.values) {
    result.values[name] = value;
    value = def.attributes.at(name);
  }

  return result;
}

auto has_component(const entt::registry& registry,
                   const ContextID contextId,
                   const ComponentID& compId) -> bool
{
  const auto& context = get_context(registry, contextId);

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == compId) {
      return true;
    }
  }

  return false;
}

auto get_component(const entt::registry& registry,
                   const ContextID contextId,
                   const ComponentID& compId) -> const comp::Component&
{
  const auto& context = get_context(registry, contextId);

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == compId) {
      return component;
    }
  }

  panic("Context did not feature requested component!");
}

auto get_component_attribute(const entt::registry& registry,
                             const ContextID contextId,
                             const ComponentID& compId,
                             const std::string_view attribute) -> const Attribute&
{
  const auto& context = get_context(registry, contextId);

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);
    if (component.type == compId) {
      if (const auto it = component.values.find(attribute);
          it != component.values.end()) {
        return it->second;
      }
    }
  }

  panic("Did not find component attribute!");
}

auto get_component_count(const entt::registry& registry, const ContextID contextId)
    -> usize
{
  return get_context(registry, contextId).components.size();
}

}  // namespace tactile::sys
