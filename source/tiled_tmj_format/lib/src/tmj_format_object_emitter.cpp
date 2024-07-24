// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_object_emitter.hpp"

#include "tactile/base/document/object_view.hpp"
#include "tactile/tiled_tmj_format/tmj_format_meta_emitter.hpp"

namespace tactile {

auto emit_tiled_tmj_object(const IObjectView& object) -> nlohmann::json
{
  const auto object_position = object.get_position();
  const auto object_size = object.get_size();
  const auto object_type = object.get_type();

  auto object_json = nlohmann::json::object();
  object_json["id"] = object.get_id();
  object_json["x"] = object_position.x();
  object_json["y"] = object_position.y();
  object_json["width"] = object_size.x();
  object_json["height"] = object_size.y();
  object_json["visible"] = object.is_visible();
  object_json["type"] = object.get_tag();
  object_json["rotation"] = 0;

  if (object_type == ObjectType::kPoint) {
    object_json["point"] = true;
  }
  else if (object_type == ObjectType::kEllipse) {
    object_json["ellipse"] = true;
  }

  emit_tiled_tmj_metadata(object.get_meta(), object_json);

  return object_json;
}

}  // namespace tactile
