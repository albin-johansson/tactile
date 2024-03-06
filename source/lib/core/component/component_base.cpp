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

#include "component_base.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "common/util/assoc.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

ComponentBase::ComponentBase(const UUID& type)
    : mTypeId {type}
{
}

void ComponentBase::add_attr(String key, AttributeType type)
{
  if (!has_key(mAttributes, key)) {
    mAttributes[std::move(key)].reset_to_default(type);
  }
  else {
    throw Exception {"Attribute key must be unique!"};
  }
}

void ComponentBase::add_attr(String key, Attribute value)
{
  if (!has_key(mAttributes, key)) {
    mAttributes[std::move(key)] = std::move(value);
  }
  else {
    throw Exception {"Attribute key must be unique!"};
  }
}

void ComponentBase::update_attr(StringView key, Attribute value)
{
  auto& attribute = lookup_in(mAttributes, key);
  attribute = std::move(value);
}

auto ComponentBase::remove_attr(StringView key) -> Result
{
  if (const auto iter = mAttributes.find(key); iter != mAttributes.end()) {
    mAttributes.erase(iter);
    return success;
  }
  else {
    return failure;
  }
}

auto ComponentBase::rename_attr(StringView old_key, String new_key) -> Result
{
  if (has_key(mAttributes, new_key)) {
    return failure;
  }

  if (const auto iter = mAttributes.find(old_key); iter != mAttributes.end()) {
    auto value = iter->second;
    mAttributes.erase(iter);
    mAttributes[std::move(new_key)] = std::move(value);
    return success;
  }
  else {
    return failure;
  }
}

auto ComponentBase::duplicate_attr(StringView key) -> String
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

auto ComponentBase::get_attr(StringView key) const -> const Attribute&
{
  return lookup_in(mAttributes, key);
}

auto ComponentBase::has_attr(StringView key) const -> bool
{
  return has_key(mAttributes, key);
}

auto ComponentBase::attr_count() const -> usize
{
  return mAttributes.size();
}

auto ComponentBase::empty() const -> bool
{
  return mAttributes.empty();
}

}  // namespace tactile
