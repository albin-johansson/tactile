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

#include "misc/panic.hpp"

namespace tactile {

void PropertyBundle::add(std::string name, AttributeType type)
{
  if (!contains(name)) {
    mProps[std::move(name)].reset_to_default(type);
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

void PropertyBundle::add(std::string name, Attribute value)
{
  if (!contains(name)) {
    mProps[std::move(name)] = std::move(value);
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

void PropertyBundle::update(std::string_view name, Attribute value)
{
  auto& property = at(name);
  property = value;
}

void PropertyBundle::remove(std::string_view name)
{
  if (const auto iter = mProps.find(name); iter != mProps.end()) {
    mProps.erase(iter);
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

void PropertyBundle::rename(std::string_view current, std::string updated)
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

auto PropertyBundle::change_type(std::string_view name, const AttributeType type)
    -> Attribute
{
  auto& property = at(name);

  auto previous = property;
  property.reset_to_default(type);

  return previous;
}

auto PropertyBundle::find(std::string_view name) -> Attribute*
{
  if (const auto iter = mProps.find(name); iter != mProps.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto PropertyBundle::find(std::string_view name) const -> const Attribute*
{
  if (const auto iter = mProps.find(name); iter != mProps.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto PropertyBundle::at(std::string_view name) -> Attribute&
{
  if (auto* property = find(name)) {
    return *property;
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

auto PropertyBundle::at(std::string_view name) const -> const Attribute&
{
  if (const auto* property = find(name)) {
    return *property;
  }
  else {
    throw TactileError {"Invalid property name!"};
  }
}

auto PropertyBundle::contains(std::string_view name) const -> bool
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