#include "save_object.hpp"

#include "save_properties.hpp"

namespace Tactile::IO {

auto SaveObject(const ObjectData& object, const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();

  json["id"] = object.id.get();
  json["name"] = object.name;
  json["x"] = object.x;
  json["y"] = object.y;
  json["width"] = object.width;
  json["height"] = object.height;
  json["visible"] = object.visible;
  json["type"] = object.custom_type;
  json["rotation"] = 0;

  if (object.type == ObjectType::Point) {
    json["point"] = true;
  }
  else if (object.type == ObjectType::Ellipse) {
    json["ellipse"] = true;
  }

  if (!object.properties.empty()) {
    json["properties"] = SaveProperties(object.properties, dir);
  }

  return json;
}

}  // namespace Tactile::IO
