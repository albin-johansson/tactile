#include "parse_properties.hpp"

#include <filesystem>  // path
#include <string>      // string

#include <centurion.hpp>

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseValue(const JSON& json,
                              Property& property,
                              const std::string& type) -> ParseError
{
  const auto value = json.at("value");

  if (type == "string") {
    AssignString(property, value.get<std::string>().c_str());
  }
  else if (type == "int") {
    AssignInt(property, value.get<int32>());
  }
  else if (type == "float") {
    AssignFloat(property, value.get<float>());
  }
  else if (type == "bool") {
    AssignBool(property, value.get<bool>());
  }
  else if (type == "file") {
    AssignFile(property, std::filesystem::path{value.get<std::string>()}.c_str());
  }
  else if (type == "object") {
    AssignObject(property, value.get<int32>());
  }
  else if (type == "color") {
    const auto string = value.get<std::string>();
    const auto color = (string.size() == 9) ? cen::color::from_argb(string)
                                            : cen::color::from_rgb(string);
    if (color) {
      AssignColor(property,
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

[[nodiscard]] auto ParseProperty(const JSON& json, Property& property) -> ParseError
{
  if (const auto it = json.find("name"); it != json.end()) {
    SetName(property, it->get<std::string>().c_str());
  }
  else {
    return ParseError::PropertyMissingName;
  }

  if (const auto it = json.find("type"); it != json.end()) {
    const auto type = it->get<std::string>();
    if (const auto err = ParseValue(json, property, type); err != ParseError::None) {
      return err;
    }
  }
  else {
    return ParseError::PropertyMissingType;
  }

  return ParseError::None;
}

template <typename T>
auto ParsePropertiesImpl(const JSON& json, T& source) -> ParseError
{
  if (const auto it = json.find("properties"); it != json.end()) {
    ReserveProperties(source, it->size());

    for (const auto& [key, value] : it->items()) {
      auto& property = AddProperty(source);
      if (const auto err = ParseProperty(value, property); err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace

auto ParseProperties(const JSON& json, Map& map) -> ParseError
{
  return ParsePropertiesImpl(json, map);
}

auto ParseProperties(const JSON& json, Tileset& tileset) -> ParseError
{
  return ParsePropertiesImpl(json, tileset);
}

auto ParseProperties(const JSON& json, Tile& tile) -> ParseError
{
  return ParsePropertiesImpl(json, tile);
}

auto ParseProperties(const JSON& json, Layer& layer) -> ParseError
{
  return ParsePropertiesImpl(json, layer);
}

auto ParseProperties(const JSON& json, Object& object) -> ParseError
{
  return ParsePropertiesImpl(json, object);
}

}  // namespace Tactile::IO
