// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/emit/tmj_object_emitter.hpp"

#include "tactile/tmj-format/emit/tmj_meta_emitter.hpp"

namespace tactile::tmj {

auto emit_object(const ir::Object& object) -> JSON
{
  auto object_json = JSON::object();

  object_json["id"] = object.id;
  object_json["name"] = object.meta.name;
  object_json["x"] = object.x;
  object_json["y"] = object.y;
  object_json["width"] = object.width;
  object_json["height"] = object.height;
  object_json["visible"] = object.visible;
  object_json["type"] = object.tag;
  object_json["rotation"] = 0;
  object_json["properties"] = emit_property_array(object.meta);

  // TMJ objects are assumed to be rectangles if there's no point or ellipse indicators.
  if (object.type == ObjectType::kPoint) {
    object_json["point"] = true;
  }
  else if (object.type == ObjectType::kEllipse) {
    object_json["ellipse"] = true;
  }

  return object_json;
}

auto emit_object_array(const Vector<ir::Object>& objects) -> JSON
{
  auto object_json_array = JSON::array();

  for (const auto& object : objects) {
    object_json_array += emit_object(object);
  }

  return object_json_array;
}

}  // namespace tactile::tmj
