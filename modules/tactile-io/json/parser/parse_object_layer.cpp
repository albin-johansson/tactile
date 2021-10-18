#include "parse_object_layer.hpp"

#include <string>   // string
#include <utility>  // move

#include <json.hpp>  // json

#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseObject(const JSON& json) -> tl::expected<ObjectData, ParseError>
{
  ObjectData data;

  if (const auto it = json.find("id"); it != json.end()) {
    data.id = ObjectID{it->get<ObjectID::value_type>()};
  }
  else {
    return tl::make_unexpected(ParseError::ObjectMissingId);
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

  if (auto props = ParseProperties(json)) {
    data.properties = std::move(*props);
  }
  else {
    return tl::make_unexpected(props.error());
  }

  return data;
}

}  // namespace

auto ParseObjectLayer(const JSON& json) -> tl::expected<ObjectLayerData, ParseError>
{
  ObjectLayerData data;

  if (const auto it = json.find("objects"); it != json.end()) {
    data.objects.reserve(it->size());
    for (const auto& [key, value] : it->items()) {
      if (auto object = ParseObject(value)) {
        data.objects.push_back(std::move(*object));
      }
      else {
        return tl::make_unexpected(object.error());
      }
    }
  }

  return data;
}

}  // namespace Tactile::IO
