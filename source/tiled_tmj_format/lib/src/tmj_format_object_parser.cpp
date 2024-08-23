// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_object_parser.hpp"

#include <utility>  // move

#include "tactile/tiled_tmj_format/tmj_format_attribute_parser.hpp"

namespace tactile {

auto parse_tiled_tmj_object(const nlohmann::json& object_json)
    -> std::expected<ir::Object, SaveFormatParseError>
{
  if (!object_json.contains("name")) {
    return std::unexpected {SaveFormatParseError::kNoObjectName};
  }

  ir::Object object {};

  if (auto metadata = parse_tiled_tmj_metadata(object_json)) {
    object.meta = std::move(*metadata);
  }
  else {
    return std::unexpected {metadata.error()};
  }

  if (const auto id_iter = object_json.find("id"); id_iter != object_json.end()) {
    id_iter->get_to(object.id);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoObjectId};
  }

  if (const auto type_iter = object_json.find("type"); type_iter != object_json.end()) {
    type_iter->get_to(object.tag);
  }

  if (const auto x_iter = object_json.find("x"); x_iter != object_json.end()) {
    x_iter->get_to(object.position[0]);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoObjectX};
  }

  if (const auto y_iter = object_json.find("y"); y_iter != object_json.end()) {
    y_iter->get_to(object.position[1]);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoObjectY};
  }

  if (const auto width_iter = object_json.find("width"); width_iter != object_json.end()) {
    width_iter->get_to(object.size[0]);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoObjectWidth};
  }

  if (const auto height_iter = object_json.find("height"); height_iter != object_json.end()) {
    height_iter->get_to(object.size[1]);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoObjectHeight};
  }

  if (const auto visible_iter = object_json.find("visible");
      visible_iter != object_json.end()) {
    visible_iter->get_to(object.visible);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoObjectVisibility};
  }

  if (const auto point_iter = object_json.find("point");
      point_iter != object_json.end() && point_iter->is_boolean() && point_iter->get<bool>()) {
    object.type = ObjectType::kPoint;
  }
  else if (const auto ellipse_iter = object_json.find("ellipse");
           ellipse_iter != object_json.end() && ellipse_iter->is_boolean() &&
           ellipse_iter->get<bool>()) {
    object.type = ObjectType::kEllipse;
  }
  else {
    object.type = ObjectType::kRect;
  }

  return object;
}

}  // namespace tactile
