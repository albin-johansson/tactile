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

#include "tactile/core/contexts/component_bundle.hpp"

#include <utility>  // move

#include "tactile/core/common/error.hpp"
#include "tactile/core/containers/lookup.hpp"

namespace tactile {

void ComponentBundle::attach(ComponentInstance component)
{
  const auto component_id = component.type_id;
  auto [iter, did_insert] = mComponents.try_emplace(component_id, std::move(component));

  if (!did_insert) {
    throw Error {"Cannot attach same component to context more than once"};
  }
}

auto ComponentBundle::detach(const ComponentID id) -> ComponentInstance
{
  return erase_in(mComponents, id);
}

auto ComponentBundle::get(const ComponentID id) -> ComponentInstance&
{
  return lookup_in(mComponents, id);
}

auto ComponentBundle::get(const ComponentID id) const -> const ComponentInstance&
{
  return lookup_in(mComponents, id);
}

auto ComponentBundle::has(const ComponentID id) const -> bool
{
  return exists_in(mComponents, id);
}

auto ComponentBundle::size() const -> usize
{
  return mComponents.size();
}

auto ComponentBundle::begin() const noexcept -> const_iterator
{
  return mComponents.begin();
}

auto ComponentBundle::end() const noexcept -> const_iterator
{
  return mComponents.end();
}

}  // namespace tactile
