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

#include "tactile/core/contexts/property_bundle.hpp"

#include <utility>  // move

#include "tactile/core/common/error.hpp"
#include "tactile/core/containers/lookup.hpp"

namespace tactile {

auto PropertyBundle::add(std::string name) -> Property&
{
  auto [iter, did_insert] = mProperties.try_emplace(std::move(name));

  if (!did_insert) {
    throw Error {"Tried to add property with taken name"};
  }

  return iter->second;
}

auto PropertyBundle::remove(std::string_view name) -> Property
{
  return erase_in(mProperties, name).value();
}

void PropertyBundle::update(std::string_view name, Property value)
{
  auto& property = get(name);
  property = std::move(value);
}

void PropertyBundle::rename(std::string_view current_name, std::string new_name)
{
  auto old_property = remove(current_name);
  auto& new_property = add(std::move(new_name));
  new_property = std::move(old_property);
}

auto PropertyBundle::get(std::string_view name) -> Property&
{
  return lookup_in(mProperties, name);
}

auto PropertyBundle::get(std::string_view name) const -> const Property&
{
  return lookup_in(mProperties, name);
}

auto PropertyBundle::has(std::string_view name) const -> bool
{
  return exists_in(mProperties, name);
}

auto PropertyBundle::size() const -> usize
{
  return mProperties.size();
}

auto PropertyBundle::begin() const noexcept -> const_iterator
{
  return mProperties.begin();
}

auto PropertyBundle::end() const noexcept -> const_iterator
{
  return mProperties.end();
}

}  // namespace tactile
