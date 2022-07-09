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

#include "component_definition.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "misc/panic.hpp"

namespace tactile {

ComponentDefinition::ComponentDefinition() = default;

ComponentDefinition::ComponentDefinition(const UUID& id) : mId {id} {}

auto ComponentDefinition::instantiate() const -> Component
{
  return {mId, mAttributes};
}

void ComponentDefinition::add_attr(std::string key, const AttributeType type)
{
  if (!mAttributes.contains(key)) {
    mAttributes[std::move(key)].reset_to_default(type);
  }
  else {
    throw TactileError {"Attribute key must be unique!"};
  }
}

void ComponentDefinition::add_attr(std::string key, Attribute value)
{
  if (!mAttributes.contains(key)) {
    mAttributes[std::move(key)] = std::move(value);
  }
  else {
    throw TactileError {"Attribute key must be unique!"};
  }
}

void ComponentDefinition::update_attr(std::string_view key, Attribute value)
{
  auto& attribute = lookup_in(mAttributes, key);
  attribute = std::move(value);
}

void ComponentDefinition::remove_attr(std::string_view key)
{
  if (const auto iter = mAttributes.find(key); iter != mAttributes.end()) {
    mAttributes.erase(iter);
  }
  else {
    throw TactileError {"Tried to remove non-existent attribute!"};
  }
}

void ComponentDefinition::rename_attr(std::string_view current, std::string updated)
{
  if (mAttributes.contains(updated)) {
    throw TactileError {"Attribute name must be unique!"};
  }

  if (const auto iter = mAttributes.find(current); iter != mAttributes.end()) {
    auto value = iter->second;
    mAttributes.erase(iter);
    mAttributes[std::move(updated)] = std::move(value);
  }
  else {
    throw TactileError {"Tried to rename non-existent attribute!"};
  }
}

auto ComponentDefinition::duplicate_attr(std::string_view key) -> std::string
{
  auto value = lookup_in(mAttributes, key);

  int         suffix = 1;
  std::string newKey;
  do {
    newKey = fmt::format("{} ({})", key, suffix);
    ++suffix;
  } while (mAttributes.contains(newKey));

  mAttributes[newKey] = std::move(value);
  return newKey;
}

auto ComponentDefinition::get_attr(std::string_view key) const -> const Attribute&
{
  return lookup_in(mAttributes, key);
}

auto ComponentDefinition::has_attr(std::string_view key) const -> bool
{
  return mAttributes.contains(key);
}

void ComponentDefinition::set_name(std::string name)
{
  mName = std::move(name);
}

auto ComponentDefinition::get_name() const -> const std::string&
{
  return mName;
}

auto ComponentDefinition::size() const -> usize
{
  return mAttributes.size();
}

auto ComponentDefinition::empty() const -> bool
{
  return mAttributes.empty();
}

auto ComponentDefinition::get_uuid() const -> const UUID&
{
  return mId;
}

}  // namespace tactile
