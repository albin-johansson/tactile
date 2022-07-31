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

#include "xml.hpp"

#include <algorithm>  // any_of
#include <cstring>    // strcmp

#include "editor/constants.hpp"
#include "misc/assert.hpp"

namespace tactile::io {

using XMLAttr = pugi::xml_attribute;

auto collect_children(XMLNode source, const char* name) -> std::vector<XMLNode>
{
  std::vector<XMLNode> children;

  for (auto child : source.children(name)) {
    children.push_back(child);
  }

  return children;
}

auto has_attribute(XMLNode node, const char* attr_name) -> bool
{
  TACTILE_ASSERT(attr_name != nullptr);
  return std::any_of(node.attributes_begin(),
                     node.attributes_end(),
                     [attr_name](const XMLAttr attribute) {
                       return std::strcmp(attribute.name(), attr_name) == 0;
                     });
}

auto string_attribute(XMLNode node, const char* attr_name) -> Maybe<std::string>
{
  TACTILE_ASSERT(attr_name);

  if (const char* str = node.attribute(attr_name).as_string(nullptr)) {
    return str;
  }
  else {
    return nothing;
  }
}

auto int_attribute(XMLNode node, const char* attr_name) -> Maybe<int32>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  const auto value = attribute.as_int(max_int);

  if (!attribute.empty() && value != max_int) {
    return value;
  }
  else {
    return nothing;
  }
}

auto uint_attribute(XMLNode node, const char* attr_name) -> Maybe<uint32>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  const auto value = attribute.as_uint(max_uint);

  if (!attribute.empty() && value != max_uint) {
    return value;
  }
  else {
    return nothing;
  }
}

auto float_attribute(XMLNode node, const char* attr_name) -> Maybe<float>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  const auto value = attribute.as_float(max_float);

  if (!attribute.empty() && value != max_float) {
    return value;
  }
  else {
    return nothing;
  }
}

auto bool_attribute(XMLNode node, const char* attr_name) -> Maybe<bool>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  if (!attribute.empty()) {
    return attribute.as_bool();
  }
  else {
    return nothing;
  }
}

}  // namespace tactile::io