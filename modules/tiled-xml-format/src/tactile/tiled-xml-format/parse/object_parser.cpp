// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/parse/object_parser.hpp"

#include <iterator>  // distance
#include <utility>   // move

#include "tactile/tiled-xml-format/common.hpp"
#include "tactile/tiled-xml-format/parse/meta_parser.hpp"

namespace tactile::tiled::tmx {
namespace {

[[nodiscard]]
auto _parse_object_type(const pugi::xml_node object_node) -> Result<ObjectType>
{
  if (!object_node.child("point").empty()) {
    return ObjectType::kPoint;
  }
  else if (!object_node.child("ellipse").empty()) {
    return ObjectType::kEllipse;
  }

  return ObjectType::kRect;
}

}  // namespace

auto parse_object_node(const pugi::xml_node object_node) -> Result<ir::Object>
{
  ir::Object object {};

  return parse_to(object_node, "id", object.id.value)
      .and_then([&] { return try_parse_to(object_node, "name", object.meta.name); })
      .and_then([&] { return try_parse_to(object_node, "type", object.tag); })
      .and_then([&] { return try_parse_to(object_node, "x", object.x); })
      .and_then([&] { return try_parse_to(object_node, "y", object.y); })
      .and_then([&] { return try_parse_to(object_node, "width", object.width); })
      .and_then([&] { return try_parse_to(object_node, "height", object.height); })
      .and_then(
          [&] { return try_parse_to(object_node, "visible", object.visible, true); })
      .and_then([&] { return _parse_object_type(object_node); })
      .and_then([&](const ObjectType object_type) {
        object.type = object_type;
        return kOK;
      })
      .and_then([&] { return parse_metadata(object_node, object.meta); })
      .transform([&] { return std::move(object); });
}

auto parse_objects(const pugi::xml_node parent_node) -> Result<Vector<ir::Object>>
{
  Vector<ir::Object> objects {};

  const auto object_node_range = parent_node.children("object");
  if (object_node_range.empty()) {
    return objects;
  }

  objects.reserve(std::distance(object_node_range.begin(), object_node_range.end()));

  for (const auto object_node : object_node_range) {
    if (auto object = parse_object_node(object_node)) {
      objects.push_back(std::move(*object));
    }
    else {
      return propagate_unexpected(object);
    }
  }

  return objects;
}

}  // namespace tactile::tiled::tmx
