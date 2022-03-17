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

#include "xml_attribute_parser.hpp"

#include <cstring>     // strcmp
#include <filesystem>  // path
#include <string>      // string
#include <utility>     // move

#include <centurion.hpp>

#include "io/maps/xml_utils.hpp"
#include "misc/assert.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_value(pugi::xml_node node, const char* type, Attribute& value)
    -> ParseError
{
  TACTILE_ASSERT(type);

  if (node.attribute("value").empty()) {
    // TODO no_property_value
  }

  if (std::strcmp(type, "string") == 0) {
    value = string_attribute(node, "value").value();
  }
  else if (std::strcmp(type, "int") == 0) {
    value = int_attribute(node, "value").value();
  }
  else if (std::strcmp(type, "float") == 0) {
    value = float_attribute(node, "value").value();
  }
  else if (std::strcmp(type, "bool") == 0) {
    value = bool_attribute(node, "value").value();
  }
  else if (std::strcmp(type, "file") == 0) {
    std::filesystem::path path = string_attribute(node, "value").value();
    value = std::move(path);
  }
  else if (std::strcmp(type, "object") == 0) {
    value = object_t{int_attribute(node, "value").value()};
  }
  else if (std::strcmp(type, "color") == 0) {
    const auto hex = string_attribute(node, "value").value();
    if (const auto color =
            (hex.size() == 9) ? cen::color::from_argb(hex) : cen::color::from_rgb(hex)) {
      value = *color;
    }
    else {
      return ParseError::corrupt_property_value;
    }
  }
  else {
    return ParseError::unsupported_property_type;
  }

  return ParseError::none;
}

[[nodiscard]] auto _parse_property(pugi::xml_node node, ir::AttributeContextData& context)
    -> ParseError
{
  std::string propertyName;

  if (auto name = string_attribute(node, "name")) {
    propertyName = std::move(*name);
  }
  else {
    return ParseError::no_property_name;
  }

  auto& value = context.properties[std::move(propertyName)];

  /* String properties may exclude the type attribute */
  const char* type = node.attribute("type").as_string("string");

  if (const auto err = _parse_value(node, type, value); err != ParseError::none) {
    return err;
  }

  return ParseError::none;
}

}  // namespace

auto parse_properties(pugi::xml_node node, ir::AttributeContextData& context)
    -> ParseError
{
  for (const auto propertyNode : node.child("properties").children("property")) {
    if (const auto err = _parse_property(propertyNode, context);
        err != ParseError::none) {
      return err;
    }
  }

  return ParseError::none;
}

}  // namespace tactile::parsing