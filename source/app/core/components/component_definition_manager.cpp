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

#include "component_definition_manager.hpp"

#include <algorithm>  // any_of
#include <utility>    // move

#include "misc/panic.hpp"

namespace tactile::core {

auto ComponentDefinitionManager::define_comp(std::string name) -> UUID
{
  if (contains(name)) {
    throw TactileError{"Component definition name is not unique!"};
  }

  ComponentDefinition def;
  def.set_name(std::move(name));

  const auto id = def.get_uuid();
  mDefs[id] = std::move(def);

  return id;
}

void ComponentDefinitionManager::remove_comp(const UUID& id)
{
  if (const auto iter = mDefs.find(id); iter != mDefs.end()) {
    mDefs.erase(iter);
  }
  else {
    throw TactileError{"Tried to remove non-existent component definition!"};
  }
}

void ComponentDefinitionManager::rename_comp(const UUID& id, std::string name)
{
  if (contains(name)) {
    throw TactileError{"New component name was not unique!"};
  }

  if (const auto iter = mDefs.find(id); iter != mDefs.end()) {
    auto& def = iter->second;
    def.set_name(std::move(name));
  }
  else {
    throw TactileError{"Tried to rename non-existent component definition!"};
  }
}

auto ComponentDefinitionManager::at(const UUID& id) const -> const ComponentDefinition&
{
  if (const auto iter = mDefs.find(id); iter != mDefs.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid component definition identifier!"};
  }
}

auto ComponentDefinitionManager::contains(std::string_view name) const -> bool
{
  return std::any_of(mDefs.begin(), mDefs.end(), [name](const auto& pair) {
    return pair.second.get_name() == name;
  });
}

auto ComponentDefinitionManager::size() const -> usize
{
  return mDefs.size();
}

}  // namespace tactile::core
