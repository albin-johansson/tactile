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

#include "xml_utils.hpp"

#include <algorithm>  // any_of
#include <cstring>    // strcmp

#include "editor/constants.hpp"
#include "misc/assert.hpp"

namespace tactile {

auto collect_children(pugi::xml_node source, const char* name)
    -> std::vector<pugi::xml_node>
{
  std::vector<pugi::xml_node> children;

  for (auto child : source.children(name)) {
    children.push_back(child);
  }

  return children;
}

auto has_attribute(pugi::xml_node node, const char* attributeName) -> bool
{
  TACTILE_ASSERT(attributeName != nullptr);

  const auto begin = node.attributes_begin();
  const auto end = node.attributes_end();

  return std::any_of(begin, end, [attributeName](const pugi::xml_attribute attribute) {
    return std::strcmp(attribute.name(), attributeName) == 0;
  });
}

auto string_attribute(pugi::xml_node node, const char* attributeName)
    -> std::optional<std::string>
{
  TACTILE_ASSERT(attributeName);

  if (const char* str = node.attribute(attributeName).as_string(nullptr)) {
    return str;
  }
  else {
    return std::nullopt;
  }
}

auto int_attribute(pugi::xml_node node, const char* attributeName) -> std::optional<int32>
{
  TACTILE_ASSERT(attributeName);

  const auto attribute = node.attribute(attributeName);
  const auto value = attribute.as_int(max_int);

  if (!attribute.empty() && value != max_int) {
    return value;
  }
  else {
    return std::nullopt;
  }
}

auto uint_attribute(pugi::xml_node node, const char* attributeName)
    -> std::optional<uint32>
{
  TACTILE_ASSERT(attributeName);

  const auto attribute = node.attribute(attributeName);
  const auto value = attribute.as_uint(max_uint);

  if (!attribute.empty() && value != max_uint) {
    return value;
  }
  else {
    return std::nullopt;
  }
}

auto float_attribute(pugi::xml_node node, const char* attributeName)
    -> std::optional<float>
{
  TACTILE_ASSERT(attributeName);

  const auto attribute = node.attribute(attributeName);
  const auto value = attribute.as_float(max_float);

  if (!attribute.empty() && value != max_float) {
    return value;
  }
  else {
    return std::nullopt;
  }
}

auto bool_attribute(pugi::xml_node node, const char* attributeName) -> std::optional<bool>
{
  TACTILE_ASSERT(attributeName);

  const auto attribute = node.attribute(attributeName);
  if (!attribute.empty()) {
    return attribute.as_bool();
  }
  else {
    return std::nullopt;
  }
}

}  // namespace tactile