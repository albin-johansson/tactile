#include "parse_properties.hpp"

#include <centurion.hpp>
#include <cstring>  // strcmp

#include "aliases/cstr.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseValue(const pugi::xml_node node,
                              const CStr type,
                              PropertyValue& property) -> ParseError
{
  if (std::strcmp(type, "string") == 0) {
    property.SetValue(GetString(node, "value").value());
  }
  else if (std::strcmp(type, "int") == 0) {
    property.SetValue(GetInt(node, "value").value());
  }
  else if (std::strcmp(type, "float") == 0) {
    property.SetValue(GetFloat(node, "value").value());
  }
  else if (std::strcmp(type, "bool") == 0) {
    property.SetValue(node.attribute("value").as_bool());
  }
  else if (std::strcmp(type, "file") == 0) {
    const auto file = GetString(node, "value").value();
    property.SetValue(std::filesystem::path{file});
  }
  else if (std::strcmp(type, "object") == 0) {
    const auto object = GetInt(node, "value").value();
    property.SetValue(ObjectRef{object});
  }
  else if (std::strcmp(type, "color") == 0) {
    const auto string = GetString(node, "value").value();
    const auto color = (string.size() == 9) ? cen::color::from_argb(string)
                                            : cen::color::from_rgb(string);
    if (color) {
      property.SetValue(*color);
    }
    else {
      return ParseError::CouldNotParseProperty;
    }
  }
  else {
    return ParseError::PropertyUnknownType;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseProperty(const pugi::xml_node node, PropertyData& data)
    -> ParseError
{
  if (const auto* name = node.attribute("name").as_string(nullptr)) {
    data.name = name;
  }
  else {
    return ParseError::PropertyMissingName;
  }

  // String properties in the XML format do not feature explicit type attributes
  if (const auto* type = node.attribute("type").as_string(nullptr)) {
    if (const auto err = ParseValue(node, type, data.property); err != ParseError::None) {
      return err;
    }
  }
  else {
    if (const auto err = ParseValue(node, "string", data.property);
        err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace

auto ParseProperties(const pugi::xml_node node, std::vector<PropertyData>& properties)
    -> ParseError
{
  for (const auto propertyNode : node.child("properties").children("property")) {
    auto& propertyData = properties.emplace_back();
    if (const auto err = ParseProperty(propertyNode, propertyData);
        err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
