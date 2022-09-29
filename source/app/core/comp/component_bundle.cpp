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

#include "component_bundle.hpp"

#include <utility>  // move

#include "misc/panic.hpp"

namespace tactile {

void ComponentBundle::add(Component component)
{
  if (contains(component.definition_id())) {
    throw TactileError {"Bundle cannot hold multiple components of same type!"};
  }

  const auto comp_id = component.definition_id();
  mComps.try_emplace(comp_id, std::move(component));
}

auto ComponentBundle::erase(const UUID& comp_id) -> Component
{
  if (const auto iter = mComps.find(comp_id); iter != mComps.end()) {
    auto component = iter->second;
    mComps.erase(iter);
    return component;
  }
  else {
    throw TactileError {"Tried to remove non-existent component from bundle!"};
  }
}

auto ComponentBundle::at(const UUID& comp_id) -> Component&
{
  return lookup_in(mComps, comp_id);
}

auto ComponentBundle::at(const UUID& comp_id) const -> const Component&
{
  return lookup_in(mComps, comp_id);
}

auto ComponentBundle::try_get(const UUID& comp_id) -> Component*
{
  if (const auto iter = mComps.find(comp_id); iter != mComps.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto ComponentBundle::contains(const UUID& comp_id) const -> bool
{
  return mComps.find(comp_id) != mComps.end();
}

auto ComponentBundle::size() const -> usize
{
  return mComps.size();
}

auto ComponentBundle::empty() const -> bool
{
  return mComps.empty();
}

}  // namespace tactile