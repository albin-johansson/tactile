#include "parse_object_layer.hpp"

#include <string>  // string

#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseObject(const JSON& json, Object& object) -> ParseError
{
  if (const auto it = json.find("id"); it != json.end()) {
    SetId(object, it->get<int32>());
  }
  else {
    return ParseError::ObjectMissingId;
  }

  if (const auto it = json.find("x"); it != json.end()) {
    SetX(object, it->get<float>());
  }
  else {
    SetX(object, 0);
  }

  if (const auto it = json.find("y"); it != json.end()) {
    SetY(object, it->get<float>());
  }
  else {
    SetY(object, 0);
  }

  if (const auto it = json.find("width"); it != json.end()) {
    SetWidth(object, it->get<float>());
  }
  else {
    SetWidth(object, 0);
  }

  if (const auto it = json.find("height"); it != json.end()) {
    SetHeight(object, it->get<float>());
  }
  else {
    SetHeight(object, 0);
  }

  if (const auto it = json.find("name"); it != json.end()) {
    SetName(object, it->get<std::string>().c_str());
  }

  if (const auto it = json.find("type"); it != json.end()) {
    SetTag(object, it->get<std::string>().c_str());
  }

  if (const auto it = json.find("visible"); it != json.end()) {
    SetVisible(object, it->get<bool>());
  }
  else {
    SetVisible(object, true);
  }

  if (json.contains("point")) {
    SetType(object, ObjectType::Point);
  }
  else if (json.contains("ellipse")) {
    SetType(object, ObjectType::Ellipse);
  }
  else {
    SetType(object, ObjectType::Rectangle);
  }

  if (const auto err = ParseProperties(json, object); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

template <typename T>
[[nodiscard]] auto ParseObjectLayerImpl(const JSON& json, T& source) -> ParseError
{
  if (const auto it = json.find("objects"); it != json.end()) {
    ReserveObjects(source, it->size());

    for (const auto& [key, value] : it->items()) {
      auto& object = AddObject(source);
      if (const auto err = ParseObject(value, object); err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace

auto ParseObjectLayer(const JSON& json, Layer& layer) -> ParseError
{
  auto& objectLayer = MarkAsObjectLayer(layer);
  return ParseObjectLayerImpl(json, objectLayer);
}

auto ParseObjectLayer(const JSON& json, Tile& tile) -> ParseError
{
  return ParseObjectLayerImpl(json, tile);
}

}  // namespace Tactile::IO
