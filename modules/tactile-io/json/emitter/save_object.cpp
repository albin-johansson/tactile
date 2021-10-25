#include "save_object.hpp"

#include "save_properties.hpp"

namespace Tactile::IO {

auto SaveObject(const Object& object, const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();

  json["id"] = GetId(object);
  json["name"] = GetName(object);
  json["x"] = GetX(object);
  json["y"] = GetY(object);
  json["width"] = GetWidth(object);
  json["height"] = GetHeight(object);
  json["visible"] = IsVisible(object);
  json["type"] = GetTag(object);
  json["rotation"] = 0;

  const auto type = GetType(object);
  if (type == ObjectType::Point) {
    json["point"] = true;
  }
  else if (type == ObjectType::Ellipse) {
    json["ellipse"] = true;
  }

  if (GetPropertyCount(object) != 0) {
    json["properties"] = SaveProperties(object, dir);
  }

  return json;
}

}  // namespace Tactile::IO
