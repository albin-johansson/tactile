#include "xml_attributes.hpp"

#include <cstring>     // strcmp
#include <filesystem>  // path
#include <string>      // string
#include <utility>     // move

#include <centurion.hpp>

#include "assert.hpp"
#include "io/maps/xml_utils.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_value(pugi::xml_node node,
                                const char* type,
                                attribute_value& value) -> parse_error
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
      return parse_error::corrupt_property_value;
    }
  }
  else {
    return parse_error::unsupported_property_type;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_property(pugi::xml_node node,
                                   ir::attribute_context_data& context) -> parse_error
{
  std::string propertyName;

  if (auto name = string_attribute(node, "name")) {
    propertyName = std::move(*name);
  }
  else {
    return parse_error::no_property_name;
  }

  auto& value = context.properties[std::move(propertyName)];

  /* String properties may exclude the type attribute */
  const char* type = node.attribute("type").as_string("string");

  if (const auto err = _parse_value(node, type, value); err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

}  // namespace

auto parse_properties(pugi::xml_node node, ir::attribute_context_data& context)
    -> parse_error
{
  for (const auto propertyNode : node.child("properties").children("property")) {
    if (const auto err = _parse_property(propertyNode, context);
        err != parse_error::none) {
      return err;
    }
  }

  return parse_error::none;
}

}  // namespace tactile::parsing