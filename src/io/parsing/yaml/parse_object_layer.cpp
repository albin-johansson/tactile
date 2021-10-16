#include "parse_object_layer.hpp"

#include <string>   // string
#include <utility>  // move

#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseObject(const YAML::Node& node) -> Expected<ObjectData, ParseError>
{
  ObjectData data;

  if (auto id = node["id"]) {
    data.id = ObjectID{id.as<ObjectID::value_type>()};
  }
  else {
    return tl::make_unexpected(ParseError::ObjectMissingId);
  }

  if (auto type = node["type"]) {
    const auto str = type.as<std::string>();
    if (str == "point") {
      data.type = ObjectType::Point;
    }
    else if (str == "rect") {
      data.type = ObjectType::Rectangle;
    }
    else if (str == "ellipse") {
      data.type = ObjectType::Ellipse;
    }
    else {
      return tl::make_unexpected(ParseError::ObjectInvalidType);
    }
  }
  else {
    return tl::make_unexpected(ParseError::ObjectMissingType);
  }

  if (auto name = node["name"]) {
    data.name = name.as<std::string>();
  }

  if (auto tag = node["tag"]) {
    data.custom_type = tag.as<std::string>();
  }

  if (auto visible = node["visible"]) {
    data.visible = visible.as<bool>();
  }
  else {
    data.visible = true;
  }

  if (auto x = node["x"]) {
    data.x = x.as<float>();
  }
  else {
    data.x = 0;
  }

  if (auto y = node["y"]) {
    data.y = y.as<float>();
  }
  else {
    data.y = 0;
  }

  if (auto width = node["width"]) {
    data.width = width.as<float>();
  }
  else {
    data.width = 0;
  }

  if (auto height = node["height"]) {
    data.height = height.as<float>();
  }
  else {
    data.height = 0;
  }

  if (auto props = ParseProperties(node)) {
    data.properties = std::move(*props);
  }
  else {
    return tl::make_unexpected(props.error());
  }

  return data;
}

}  // namespace

auto ParseObjectLayer(const YAML::Node& node) -> Expected<ObjectLayerData, ParseError>
{
  ObjectLayerData data;

  if (auto objects = node["objects"]) {
    data.objects.reserve(objects.size());

    for (const auto& objectNode : objects) {
      if (auto object = ParseObject(objectNode)) {
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