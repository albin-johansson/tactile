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

#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

Component::Component(const UUID& definitionId, AttributeMap attributes)
    : mDefinitionId {definitionId}
    , mAttributes {std::move(attributes)}
{}

void Component::add_attr(std::string key, Attribute value)
{
  if (!mAttributes.contains(key)) {
    mAttributes[std::move(key)] = std::move(value);
  }
  else {
    throw TactileError {"Attribute already exists!"};
  }
}

void Component::remove_attr(std::string_view key)
{
  if (const auto iter = mAttributes.find(key); iter != mAttributes.end()) {
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

void Component::rename_attr(std::string_view oldKey, std::string newKey)
{
  TACTILE_ASSERT(!mAttributes.contains(newKey));
  auto value = lookup_in(mAttributes, oldKey);

  if (const auto iter = mAttributes.find(oldKey); iter != mAttributes.end()) {
    mAttributes.erase(iter);
  }

  mAttributes[std::move(newKey)] = std::move(value);
}

auto Component::get_attr(std::string_view key) const -> const Attribute&
{
  return lookup_in(mAttributes, key);
}

auto Component::has_attr(std::string_view key) const -> bool
{
  return mAttributes.contains(key);
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
