#include "parse_object_layer.hpp"

#include <string>  // string

#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseObject(const JSON& json,
                               ObjectData& data,
                               const std::filesystem::path& dir) -> ParseError
{
  if (const auto it = json.find("id"); it != json.end()) {
    data.id = ObjectID{it->get<ObjectID::value_type>()};
  }
  else {
    return ParseError::ObjectMissingId;
  }

  if (const auto it = json.find("x"); it != json.end()) {
    it->get_to(data.x);
  }
  else {
    data.x = 0;
  }

  if (const auto it = json.find("y"); it != json.end()) {
    it->get_to(data.y);
  }
  else {
    data.y = 0;
  }

  if (const auto it = json.find("width"); it != json.end()) {
    it->get_to(data.width);
  }
  else {
    data.width = 0;
  }

  if (const auto it = json.find("height"); it != json.end()) {
    it->get_to(data.height);
  }
  else {
    data.height = 0;
  }

  if (const auto it = json.find("name"); it != json.end()) {
    it->get_to(data.name);
  }

  if (const auto it = json.find("type"); it != json.end()) {
    it->get_to(data.custom_type);
  }

  if (const auto it = json.find("visible"); it != json.end()) {
    it->get_to(data.visible);
  }
  else {
    data.visible = true;
  }

  if (json.contains("point")) {
    data.type = ObjectType::Point;
  }
  else if (json.contains("ellipse")) {
    data.type = ObjectType::Ellipse;
  }
  else {
    data.type = ObjectType::Rectangle;
  }

  if (const auto err = ParseProperties(json, data.properties, dir);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseObjectLayer(const JSON& json,
                      LayerData& layer,
                      const std::filesystem::path& dir) -> ParseError
{
  auto& data = layer.data.emplace<ObjectLayerData>();

  if (const auto it = json.find("objects"); it != json.end()) {
    for (const auto& [key, object] : it->items()) {
      auto& objectData = data.objects.emplace_back();
      if (const auto err = ParseObject(object, objectData, dir); err != ParseError::None)
      {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
