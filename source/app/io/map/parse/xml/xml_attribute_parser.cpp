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

#include <cstring>  // strcmp
#include <string>   // string
#include <utility>  // move

#include <centurion/color.hpp>

#include "core/common/fs.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/util/xml.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto _parse_value(XMLNode node, const char* type, Attribute& value)
    -> ParseError
{
  TACTILE_ASSERT(type);

  if (std::strcmp(type, "string") == 0) {
    value = as_string(node, "value").value();
  }
  else if (std::strcmp(type, "int") == 0) {
    value = as_int(node, "value").value();
  }
  else if (std::strcmp(type, "float") == 0) {
    value = as_float(node, "value").value();
  }
  else if (std::strcmp(type, "bool") == 0) {
    value = as_bool(node, "value").value();
  }
  else if (std::strcmp(type, "file") == 0) {
    fs::path path = as_string(node, "value").value();
    value = std::move(path);
  }
  else if (std::strcmp(type, "object") == 0) {
    value = object_t {as_int(node, "value").value()};
  }
  else if (std::strcmp(type, "color") == 0) {
    const auto hex = as_string(node, "value").value();
    if (hex.empty()) {
      value.reset_to_default(AttributeType::Color);
    }
    else {
      if (const auto color = (hex.size() == 9) ? cen::color::from_argb(hex)
                                               : cen::color::from_rgb(hex)) {
        value = *color;
      }
      else {
        return ParseError::CorruptPropertyValue;
      }
    }
  }
  else {
    return ParseError::UnsupportedPropertyType;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_property(XMLNode node, ir::ContextData& context) -> ParseError
{
  std::string propertyName;

  if (auto name = as_string(node, "name")) {
    propertyName = std::move(*name);
  }
  else {
    return ParseError::NoPropertyName;
  }

  auto& value = context.properties[std::move(propertyName)];

  /* String properties may exclude the type attribute */
  const char* type = node.attribute("type").as_string("string");

  if (const auto err = _parse_value(node, type, value); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto parse_properties(XMLNode node, ir::ContextData& context) -> ParseError
{
  for (const auto propertyNode : node.child("properties").children("property")) {
    if (const auto err = _parse_property(propertyNode, context);
        err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace tactile::io