// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/parse/object_parser.hpp"

#include <utility>  // move

#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/parse/common.hpp"
#include "tactile/tactile-yml-format/parse/meta_parser.hpp"

namespace tactile::yml_format {

auto parse_object_type(const StringView name) -> Result<ObjectType>
{
  if (name == "point") {
    return ObjectType::kPoint;
  }
  else if (name == "rect") {
    return ObjectType::kRect;
  }
  else if (name == "ellipse") {
    return ObjectType::kEllipse;
  }

  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedObjectType));
}

auto parse_object(const YAML::Node& object_node, const ir::Map& map) -> Result<ir::Object>
{
  TACTILE_LOG_TRACE("Parsing object node at {}:{}",
                    object_node.Mark().line,
                    object_node.Mark().column);

  ir::Object object {};

  return parse<ObjectID::value_type>(object_node, "id")
      .and_then([&](const ObjectID::value_type id) {
        object.id.value = id;
        return kOK;
      })
      .and_then([&] { return parse<String>(object_node, "type"); })
      .and_then([&](const String& type_name) { return parse_object_type(type_name); })
      .and_then([&](const ObjectType type) {
        object.type = type;
        return kOK;
      })
      .and_then([&] { return parse_to(object_node, "name", object.name); })
      .and_then([&] { return parse_to(object_node, "tag", object.tag); })
      .and_then([&] {
        object.x = try_parse<float>(object_node, "x").value_or(0.0f);
        return kOK;
      })
      .and_then([&] {
        object.y = try_parse<float>(object_node, "y").value_or(0.0f);
        return kOK;
      })
      .and_then([&] {
        object.width = try_parse<float>(object_node, "width").value_or(0.0f);
        return kOK;
      })
      .and_then([&] {
        object.height = try_parse<float>(object_node, "height").value_or(0.0f);
        return kOK;
      })
      .and_then([&] {
        object.visible = try_parse<bool>(object_node, "visible").value_or(true);
        return kOK;
      })
      .and_then([&] { return parse_metadata(object_node, map, object.meta); })
      .transform([&] { return std::move(object); });
}

auto parse_objects(const YAML::Node& parent_node, const ir::Map& map)
    -> Result<Vector<ir::Object>>
{
  Vector<ir::Object> objects {};

  if (const auto object_seq = parent_node["objects"]) {
    objects.reserve(object_seq.size());

    for (const auto object_node : object_seq) {
      if (auto object = parse_object(object_node, map)) {
        objects.push_back(std::move(*object));
      }
      else {
        return propagate_unexpected(object);
      }
    }
  }

  return objects;
}

}  // namespace tactile::yml_format
