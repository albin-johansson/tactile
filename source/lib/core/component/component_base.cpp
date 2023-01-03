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

#include "component_base.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "common/debug/panic.hpp"
#include "common/util/assoc.hpp"

namespace tactile {

ComponentBase::ComponentBase(const UUID& type)
    : mTypeId {type}
{
}

void ComponentBase::add(String key, AttributeType type)
{
  if (!has_key(mAttributes, key)) {
    mAttributes[std::move(key)].reset_to_default(type);
  }
  else {
    throw TactileError {"Attribute key must be unique!"};
  }
}

void ComponentBase::add(String key, Attribute value)
{
  if (!has_key(mAttributes, key)) {
    mAttributes[std::move(key)] = std::move(value);
  }
  else {
    throw TactileError {"Attribute key must be unique!"};
  }
}

void ComponentBase::update(StringView key, Attribute value)
{
  auto& attribute = lookup_in(mAttributes, key);
  attribute = std::move(value);
}

auto ComponentBase::remove(StringView key) -> bool
{
  if (const auto iter = mAttributes.find(key); iter != mAttributes.end()) {
    mAttributes.erase(iter);
    return true;
  }
  else {
    return false;
  }
}

auto ComponentBase::rename(StringView current, String updated) -> bool
{
  if (has_key(mAttributes, updated)) {
    throw TactileError {"Attribute name must be unique!"};
  }

  if (const auto iter = mAttributes.find(current); iter != mAttributes.end()) {
    auto value = iter->second;
    mAttributes.erase(iter);
    mAttributes[std::move(updated)] = std::move(value);
    return true;
  }
  else {
    return false;
  }
}

auto ComponentBase::duplicate(StringView key) -> String
{
  auto value = lookup_in(mAttributes, key);

  int suffix = 1;
  String new_key;
  do {
    new_key = fmt::format("{} ({})", key, suffix);
    ++suffix;
  } while (has_key(mAttributes, new_key));

  mAttributes[new_key] = std::move(value);
  return new_key;
}

auto ComponentBase::at(StringView key) const -> const Attribute&
{
  return lookup_in(mAttributes, key);
}

auto ComponentBase::has(StringView key) const -> bool
{
  return has_key(mAttributes, key);
}

auto ComponentBase::size() const -> usize
{
  return mAttributes.size();
}

auto ComponentBase::empty() const -> bool
{
  return mAttributes.empty();
}

}  // namespace tactile