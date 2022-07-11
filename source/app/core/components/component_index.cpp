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

#include "component_index.hpp"

#include <algorithm>  // any_of
#include <utility>    // move

#include "core/components/component_definition.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

auto ComponentIndex::define_comp(std::string name) -> UUID
{
  if (contains(name)) {
    throw TactileError {"Component definition name is not unique!"};
  }

  const auto id = make_uuid();

  ComponentDefinition def {id};
  def.set_name(std::move(name));

  mDefs[id] = std::move(def);

  return id;
}

void ComponentIndex::restore_comp(ComponentDefinition def)
{
  if (contains(def.get_name())) {
    throw TactileError {"Component definition name is not unique!"};
  }

  const auto id = def.get_uuid();
  TACTILE_ASSERT(!mDefs.contains(id));

  mDefs[id] = std::move(def);
}

void ComponentIndex::remove_comp(const UUID& id)
{
  if (const auto iter = mDefs.find(id); iter != mDefs.end()) {
    mDefs.erase(iter);
  }
  else {
    throw TactileError {"Tried to remove non-existent component definition!"};
  }
}

void ComponentIndex::rename_comp(const UUID& id, std::string name)
{
  if (contains(name)) {
    throw TactileError {"New component name was not unique!"};
  }

  auto& def = lookup_in(mDefs, id);
  def.set_name(std::move(name));
}

auto ComponentIndex::at(const UUID& id) -> ComponentDefinition&
{
  return lookup_in(mDefs, id);
}

auto ComponentIndex::at(const UUID& id) const -> const ComponentDefinition&
{
  return lookup_in(mDefs, id);
}

auto ComponentIndex::with_name(std::string_view name) -> ComponentDefinition&
{
  for (auto& [id, def] : mDefs) {
    if (def.get_name() == name) {
      return def;
    }
  }

  throw TactileError {"Invalid component name!"};
}

auto ComponentIndex::contains(const UUID& id) const -> bool
{
  return mDefs.contains(id);
}

auto ComponentIndex::contains(std::string_view name) const -> bool
{
  return std::any_of(mDefs.begin(), mDefs.end(), [name](const auto& pair) {
    return pair.second.get_name() == name;
  });
}

auto ComponentIndex::size() const -> usize
{
  return mDefs.size();
}

auto ComponentIndex::empty() const -> bool
{
  return mDefs.empty();
}

}  // namespace tactile
