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

#include "property_bundle.hpp"

#include <utility>  // move

#include "core/util/assoc.hpp"
#include "misc/panic.hpp"

namespace tactile {

void PropertyBundle::add(String name, AttributeType type)
{
  if (!contains(name)) {
    mProps[std::move(name)].reset_to_default(type);
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

void PropertyBundle::add(String name, Attribute value)
{
  if (!contains(name)) {
    mProps[std::move(name)] = std::move(value);
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

void PropertyBundle::update(StringView name, Attribute value)
{
  auto& property = at(name);
  property = value;
}

void PropertyBundle::remove(StringView name)
{
  if (const auto iter = mProps.find(name); iter != mProps.end()) {
    mProps.erase(iter);
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

void PropertyBundle::rename(StringView current, String updated)
{
  if (contains(updated)) {
    throw TactileError {"Duplicated property name!"};
  }

  if (const auto iter = mProps.find(current); iter != mProps.end()) {
    auto value = iter->second;
    mProps.erase(iter);
    mProps[std::move(updated)] = value;
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

auto PropertyBundle::change_type(StringView name, const AttributeType type) -> Attribute
{
  auto& property = at(name);

  auto previous = property;
  property.reset_to_default(type);

  return previous;
}

auto PropertyBundle::find(StringView name) -> Attribute*
{
  if (const auto iter = mProps.find(name); iter != mProps.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto PropertyBundle::find(StringView name) const -> const Attribute*
{
  if (const auto iter = mProps.find(name); iter != mProps.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto PropertyBundle::at(StringView name) -> Attribute&
{
  if (auto* property = find(name)) {
    return *property;
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

auto PropertyBundle::at(StringView name) const -> const Attribute&
{
  if (const auto* property = find(name)) {
    return *property;
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

auto PropertyBundle::contains(StringView name) const -> bool
{
  return find(name) != nullptr;
}

auto PropertyBundle::size() const -> usize
{
  return mProps.size();
}

auto PropertyBundle::empty() const -> bool
{
  return mProps.empty();
}

}  // namespace tactile