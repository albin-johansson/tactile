#include "parse_object.hpp"

#include <string>  // string

#include <yaml-cpp/yaml.h>

#include "parse_components.hpp"
#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

template <typename T>
[[nodiscard]] auto ParseObjectImpl(const YAML::Node& node, const Map& map, T& parent)
    -> ParseError
{
  auto& object = AddObject(parent);

  if (auto id = node["id"]) {
    SetId(object, id.as<int32>());
  }
  else {
    return ParseError::ObjectMissingId;
  }

  if (auto type = node["type"]) {
    const auto str = type.as<std::string>();
    if (str == "point") {
      SetType(object, ObjectType::Point);
    }
    else if (str == "rect") {
      SetType(object, ObjectType::Rectangle);
    }
    else if (str == "ellipse") {
      SetType(object, ObjectType::Ellipse);
    }
    else {
      return ParseError::ObjectInvalidType;
    }
  }
  else {
    return ParseError::ObjectMissingType;
  }

  if (auto name = node["name"]) {
    SetName(object, name.as<std::string>().c_str());
  }

  if (auto tag = node["tag"]) {
    SetTag(object, tag.as<std::string>().c_str());
  }

  if (auto visible = node["visible"]) {
    SetVisible(object, visible.as<bool>());
  }
  else {
    SetVisible(object, true);
  }

  if (auto x = node["x"]) {
    SetX(object, x.as<float>());
  }
  else {
    SetX(object, 0);
  }

  if (auto y = node["y"]) {
    SetY(object, y.as<float>());
  }
  else {
    SetY(object, 0);
  }

  if (auto width = node["width"]) {
    SetWidth(object, width.as<float>());
  }
  else {
    SetWidth(object, 0);
  }

  if (auto height = node["height"]) {
    SetHeight(object, height.as<float>());
  }
  else {
    SetHeight(object, 0);
  }

  if (const auto err = ParseProperties(node, object); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseComponents(map, node, object); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseObject(const YAML::Node& node, const Map& map, ObjectLayer& layer) -> ParseError
{
  return ParseObjectImpl(node, map, layer);
}

auto ParseObject(const YAML::Node& node, const Map& map, Tile& tile) -> ParseError
{
  return ParseObjectImpl(node, map, tile);
}

}  // namespace Tactile::IO
