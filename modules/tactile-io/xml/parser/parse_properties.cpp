#include "parse_properties.hpp"

#include <cstring>     // strcmp
#include <filesystem>  // path

#include <tactile-base/tactile_std.hpp>

#include <centurion.hpp>

#include "xml_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseValue(const pugi::xml_node node,
                              const CStr type,
                              Property& property) -> ParseError
{
  if (std::strcmp(type, "string") == 0) {
    IO::AssignString(property, node.attribute("value").as_string());
  }
  else if (std::strcmp(type, "int") == 0) {
    IO::AssignInt(property, GetInt(node, "value").value());
  }
  else if (std::strcmp(type, "float") == 0) {
    IO::AssignFloat(property, GetFloat(node, "value").value());
  }
  else if (std::strcmp(type, "bool") == 0) {
    IO::AssignBool(property, node.attribute("value").as_bool());
  }
  else if (std::strcmp(type, "file") == 0) {
    const std::filesystem::path file = node.attribute("value").as_string();
    IO::AssignFile(property, file.c_str());
  }
  else if (std::strcmp(type, "object") == 0) {
    IO::AssignObject(property, GetInt(node, "value").value());
  }
  else if (std::strcmp(type, "color") == 0) {
    const auto string = GetString(node, "value").value();
    const auto color = (string.size() == 9) ? cen::color::from_argb(string)
                                            : cen::color::from_rgb(string);
    if (color) {
      IO::AssignColor(property,
                      {color->red(), color->green(), color->blue(), color->alpha()});
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

[[nodiscard]] auto ParseProperty(const pugi::xml_node node, Property& property)
    -> ParseError
{
  if (const auto* name = node.attribute("name").as_string(nullptr)) {
    IO::SetName(property, name);
  }
  else {
    return ParseError::PropertyMissingName;
  }

  // String properties in the XML format do not feature explicit type attributes
  if (const auto* type = node.attribute("type").as_string(nullptr)) {
    if (const auto err = ParseValue(node, type, property); err != ParseError::None) {
      return err;
    }
  }
  else if (const auto err = ParseValue(node, "string", property); err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

template <typename T>
[[nodiscard]] auto ParsePropertiesImpl(const pugi::xml_node node, T& target) -> ParseError
{
  for (const auto propertyNode : node.child("properties").children("property")) {
    auto& property = IO::AddProperty(target);
    if (const auto err = ParseProperty(propertyNode, property); err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace

auto ParseProperties(const pugi::xml_node node, Map& map) -> ParseError
{
  return ParsePropertiesImpl(node, map);
}

auto ParseProperties(const pugi::xml_node node, Tileset& tileset) -> ParseError
{
  return ParsePropertiesImpl(node, tileset);
}

auto ParseProperties(const pugi::xml_node node, Tile& tile) -> ParseError
{
  return ParsePropertiesImpl(node, tile);
}

auto ParseProperties(const pugi::xml_node node, Layer& layer) -> ParseError
{
  return ParsePropertiesImpl(node, layer);
}

auto ParseProperties(const pugi::xml_node node, Object& object) -> ParseError
{
  return ParsePropertiesImpl(node, object);
}

}  // namespace Tactile::IO
