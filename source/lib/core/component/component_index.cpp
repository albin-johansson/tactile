// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "component_index.hpp"

#include <algorithm>  // any_of
#include <utility>    // move

#include "tactile/core/debug/exception.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {

auto ComponentIndex::define_comp(String name) -> UUID
{
  if (has_comp(name)) {
    throw Exception {"Component definition name is not unique"};
  }

  const auto component_id = UUID::generate();

  ComponentDefinition def {component_id};
  def.set_name(std::move(name));

  mDefs[component_id] = std::move(def);
  return component_id;
}

void ComponentIndex::restore_comp(ComponentDefinition component_def)
{
  if (has_comp(component_def.get_name())) {
    throw Exception {"Component definition name is not unique"};
  }

  const auto component_id = component_def.get_uuid();
  mDefs[component_id] = std::move(component_def);
}

auto ComponentIndex::remove_comp(const UUID& component_id) -> Result<void>
{
  if (const auto iter = mDefs.find(component_id); iter != mDefs.end()) {
    mDefs.erase(iter);
    return kOK;
  }
  else {
    return unexpected(make_error(GenericError::kInvalidParam));
  }
}

auto ComponentIndex::rename_comp(const UUID& component_id, String name) -> Result<void>
{
  if (!exists_in(mDefs, component_id) || has_comp(name)) {
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  auto& component_def = lookup_in(mDefs, component_id);
  component_def.set_name(std::move(name));

  return kOK;
}

auto ComponentIndex::get_comp(const UUID& component_id) -> ComponentDefinition&
{
  return lookup_in(mDefs, component_id);
}

auto ComponentIndex::get_comp(const UUID& component_id) const
    -> const ComponentDefinition&
{
  return lookup_in(mDefs, component_id);
}

auto ComponentIndex::get_comp(StringView name) -> ComponentDefinition&
{
  for (auto& [id, component_def] : mDefs) {
    if (component_def.get_name() == name) {
      return component_def;
    }
  }

  throw Exception {"Invalid component name"};
}

auto ComponentIndex::has_comp(const UUID& component_id) const -> bool
{
  return mDefs.find(component_id) != mDefs.end();
}

auto ComponentIndex::has_comp(StringView name) const -> bool
{
  return std::any_of(mDefs.begin(), mDefs.end(), [name](const auto& pair) {
    return pair.second.get_name() == name;
  });
}

auto ComponentIndex::comp_count() const -> usize
{
  return mDefs.size();
}

auto ComponentIndex::empty() const -> bool
{
  return mDefs.empty();
}

}  // namespace tactile
