/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "component_index.hpp"

#include <algorithm>  // any_of
#include <utility>    // move

#include "tactile/core/container/lookup.hpp"
#include "tactile/core/debug/error.hpp"

namespace tactile {

auto ComponentIndex::define_comp(String name) -> UUID
{
  if (has_comp(name)) {
    throw Error {"Component definition name is not unique"};
  }

  const auto component_id = make_uuid();

  ComponentDefinition def {component_id};
  def.set_name(std::move(name));

  mDefs[component_id] = std::move(def);
  return component_id;
}

void ComponentIndex::restore_comp(ComponentDefinition component_def)
{
  if (has_comp(component_def.get_name())) {
    throw Error {"Component definition name is not unique"};
  }

  const auto component_id = component_def.get_uuid();
  mDefs[component_id] = std::move(component_def);
}

auto ComponentIndex::remove_comp(const UUID& component_id) -> Result
{
  if (const auto iter = mDefs.find(component_id); iter != mDefs.end()) {
    mDefs.erase(iter);
    return success;
  }
  else {
    return failure;
  }
}

auto ComponentIndex::rename_comp(const UUID& component_id, String name) -> Result
{
  if (!exists_in(mDefs, component_id) || has_comp(name)) {
    return failure;
  }

  auto& component_def = lookup_in(mDefs, component_id);
  component_def.set_name(std::move(name));

  return success;
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
  for (auto& [id, component_def]: mDefs) {
    if (component_def.get_name() == name) {
      return component_def;
    }
  }

  throw Error {"Invalid component name"};
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
