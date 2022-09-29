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

#include "component.hpp"

#include <utility>  // move

#include "core/util/assoc.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

Component::Component(const UUID& definition_id, AttributeMap attributes)
    : mDefinitionId {definition_id}
    , mAttributes {std::move(attributes)}
{
}

void Component::add_attr(std::string key, Attribute value)
{
  if (mAttributes.find(key) == mAttributes.end()) {
    mAttributes[std::move(key)] = std::move(value);
  }
  else {
    throw TactileError {"Attribute already exists!"};
  }
}

void Component::remove_attr(std::string_view key)
{
  if (const auto iter = find_in(mAttributes, key); iter != mAttributes.end()) {
    mAttributes.erase(iter);
  }
  else {
    throw TactileError {"Invalid attribute name!"};
  }
}

void Component::update_attr(std::string_view key, Attribute value)
{
  auto& attr = lookup_in(mAttributes, key);
  attr = std::move(value);
}

void Component::rename_attr(std::string_view old_key, std::string new_key)
{
  TACTILE_ASSERT(!has_key(mAttributes, new_key));
  auto value = lookup_in(mAttributes, old_key);

  if (const auto iter = find_in(mAttributes, old_key); iter != mAttributes.end()) {
    mAttributes.erase(iter);
  }

  mAttributes[std::move(new_key)] = std::move(value);
}

auto Component::get_attr(std::string_view key) const -> const Attribute&
{
  return lookup_in(mAttributes, key);
}

auto Component::has_attr(std::string_view key) const -> bool
{
  return has_key(mAttributes, key);
}

auto Component::size() const -> usize
{
  return mAttributes.size();
}

auto Component::empty() const -> bool
{
  return mAttributes.empty();
}

}  // namespace tactile
