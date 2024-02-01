// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/component_set.hpp"

#include <algorithm>  // find_if
#include <utility>    // move

#include "tactile/foundation/container/lookup.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

auto ComponentSet::define_component(String name) -> Maybe<UUID>
{
  if (const auto existing_component_uuid = get_component_with_name(name)) {
    TACTILE_LOG_ERROR("Attempted to define component with the taken name '{}'", name);
    return kNothing;
  }

  const auto component_uuid = UUID::generate();

  Component component {component_uuid};
  ComponentDefinition definition {std::move(name), std::move(component)};

  mComponents.insert_or_assign(component_uuid, std::move(definition));

  return component_uuid;
}

auto ComponentSet::remove_component(const UUID& uuid) -> Maybe<ComponentDefinition>
{
  return erase_from(mComponents, uuid);
}

auto ComponentSet::get_component_with_name(const StringView name) const -> Maybe<UUID>
{
  const auto definition_iter =
      std::find_if(mComponents.begin(),
                   mComponents.end(),
                   [=](const ComponentDefinitionMap::value_type& entry) {
                     const auto& definition = entry.second;
                     return definition.name == name;
                   });

  if (definition_iter != mComponents.end()) {
    return definition_iter->first;
  }

  return kNothing;
}

auto ComponentSet::get_component(const UUID& uuid) -> Component&
{
  return lookup_in(mComponents, uuid).prototype;
}

auto ComponentSet::get_component(const UUID& uuid) const -> const Component&
{
  return lookup_in(mComponents, uuid).prototype;
}

auto ComponentSet::has_component(const UUID& uuid) const -> bool
{
  return exists_in(mComponents, uuid);
}

auto ComponentSet::component_count() const -> ssize
{
  return std::ssize(mComponents);
}

}  // namespace tactile::core
