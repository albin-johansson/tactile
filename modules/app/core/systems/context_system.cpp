#include "context_system.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "component_system.hpp"
#include "property_system.hpp"
#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile::sys {
namespace {

/* Identifier used to distinguish attribute contexts, generated on a
   session-by-session basis and not stored anywhere in save files. */
inline context_id _next_context_id{1};

}  // namespace

void reset_next_context_id() noexcept
{
  _next_context_id = 1;
}

auto get_and_update_next_context_id() noexcept -> context_id
{
  return _next_context_id++;
}

auto next_context_id() noexcept -> context_id
{
  return _next_context_id;
}

auto add_attribute_context(entt::registry& registry, const entt::entity entity)
    -> comp::attribute_context&
{
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.emplace<comp::attribute_context>(entity);
  context.id = get_and_update_next_context_id();

  return context;
}

auto copy_attribute_context(const entt::registry& registry, const entt::entity source)
    -> attribute_context_snapshot
{
  TACTILE_ASSERT(source != entt::null);
  TACTILE_ASSERT(registry.all_of<comp::attribute_context>(source));
  const auto& context = registry.get<comp::attribute_context>(source);

  attribute_context_snapshot snapshot;
  snapshot.id = context.id;
  snapshot.name = context.name;

  for (const auto propertyEntity : context.properties) {
    const auto& property = registry.get<comp::property>(propertyEntity);
    snapshot.properties.try_emplace(property.name, property.value);
  }

  for (const auto componentEntity : context.components) {
    const auto& component = registry.get<comp::component>(componentEntity);

    auto& componentSnapshot = snapshot.components[component.type];
    for (const auto& [attrName, attrValue] : component.values) {
      componentSnapshot.attributes[attrName] = attrValue;
    }
  }

  return snapshot;
}

void restore_attribute_context(entt::registry& registry,
                               const entt::entity entity,
                               attribute_context_snapshot snapshot)
{
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get_or_emplace<comp::attribute_context>(entity);
  context.id = snapshot.id;
  context.name = std::move(snapshot.name);

  for (auto&& [propertyName, propertyValue] : snapshot.properties) {
    add_property(registry, context, propertyName, propertyValue);
  }

  for (auto&& [componentId, componentValue] : snapshot.components) {
    auto& component = add_component(registry, context.id, componentId);

    for (const auto& [attrName, attrValue] : componentValue.attributes) {
      component.values.at(attrName) = attrValue;
    }
  }
}

auto get_context(entt::registry& registry, const context_id id)
    -> comp::attribute_context&
{
  if (auto& context = registry.ctx<comp::attribute_context>(); context.id == id) {
    return context;
  }

  for (auto&& [entity, context] : registry.view<comp::attribute_context>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  throw_traced(tactile_error{"No matching attribute context!"});
}

auto get_context(const entt::registry& registry, const context_id id)
    -> const comp::attribute_context&
{
  if (const auto& context = registry.ctx<comp::attribute_context>(); context.id == id) {
    return context;
  }

  for (auto&& [entity, context] : registry.view<comp::attribute_context>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  throw_traced(tactile_error{"No matching attribute context!"});
}

auto current_context(const entt::registry& registry) -> const comp::attribute_context&
{
  const auto& current = registry.ctx<comp::active_attribute_context>();
  return (current.entity != entt::null)
             ? registry.get<comp::attribute_context>(current.entity)
             : registry.ctx<comp::attribute_context>();
}

auto current_context_id(const entt::registry& registry) -> context_id
{
  const auto& context = current_context(registry);
  return context.id;
}

}  // namespace tactile::sys