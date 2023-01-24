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

#include "xml.hpp"

#include <algorithm>  // any_of
#include <cstring>    // strcmp

#include "common/debug/assert.hpp"
#include "ui/constants.hpp"

namespace tactile {

auto has_attr(XmlNode node, const char* attr_name) -> bool
{
  TACTILE_ASSERT(attr_name != nullptr);
  return std::any_of(node.attributes_begin(),
                     node.attributes_end(),
                     [attr_name](const XmlAttr attribute) {
                       return std::strcmp(attribute.name(), attr_name) == 0;
                     });
}

auto get_string_attr(XmlNode node, const char* attr_name) -> Maybe<String>
{
  TACTILE_ASSERT(attr_name);

  if (const char* str = node.attribute(attr_name).as_string(nullptr)) {
    return str;
  }
  else {
    return nothing;
  }
}

auto get_int_attr(XmlNode node, const char* attr_name) -> Maybe<int32>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  const auto value = attribute.as_int(kMaxInt);

  if (!attribute.empty() && value != kMaxInt) {
    return value;
  }
  else {
    return nothing;
  }
}

auto get_uint_attr(XmlNode node, const char* attr_name) -> Maybe<uint32>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  const auto value = attribute.as_uint(kMaxUint);

  if (!attribute.empty() && value != kMaxUint) {
    return value;
  }
  else {
    return nothing;
  }
}

auto get_float_attr(XmlNode node, const char* attr_name) -> Maybe<float>
{
  TACTILE_ASSERT(attr_name);

  const auto attribute = node.attribute(attr_name);
  const auto value = attribute.as_float(kMaxFloat);

  if (!attribute.empty() && value != kMaxFloat) {
    return value;
  }
  else {
    return nothing;
  }
}

auto get_bool_attr(XmlNode node, const char* attr_name) -> Maybe<bool>
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

}  // namespace tactile