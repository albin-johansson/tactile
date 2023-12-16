// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-json-format/emit/tile_emitter.hpp"

#include <utility>  // move

#include "tactile/tiled-json-format/emit/meta_emitter.hpp"
#include "tactile/tiled-json-format/emit/object_emitter.hpp"

namespace tactile::tiled::tmj {
namespace {

[[nodiscard]]
auto _emit_tile_nested_object_layer(const ir::Tile& tile) -> JSON
{
  auto object_layer_json = JSON::object();

  object_layer_json["type"] = "objectgroup";
  object_layer_json["draworder"] = "index";
  object_layer_json["name"] = "";
  object_layer_json["opacity"] = 1;
  object_layer_json["visible"] = true;
  object_layer_json["x"] = 0;
  object_layer_json["y"] = 0;
  object_layer_json["objects"] = emit_object_array(tile.objects);

  return object_layer_json;
}

[[nodiscard]]
auto _emit_tile_animation_array(const ir::Tile& tile) -> JSON
{
  auto animation_json_array = JSON::array();

  for (const auto& animation_frame : tile.animation) {
    auto animation_frame_json = JSON::object();

    animation_frame_json["tileid"] = animation_frame.tile_index.value;
    animation_frame_json["duration"] = animation_frame.duration.count();

    animation_json_array += std::move(animation_frame_json);
  }

  return animation_json_array;
}

}  // namespace

auto emit_tile_definition(const ir::Tile& tile) -> JSON
{
  auto tile_json = JSON::object();
  tile_json["id"] = tile.index.value;

  if (!tile.meta.properties.empty()) {
    tile_json["properties"] = emit_property_array(tile.meta);
  }

  if (!tile.animation.empty()) {
    tile_json["animation"] = _emit_tile_animation_array(tile);
  }

  if (!tile.objects.empty()) {
    tile_json["objectgroup"] = _emit_tile_nested_object_layer(tile);
  }

  return tile_json;
}

auto emit_tile_definition_array(const Vector<ir::Tile>& tiles) -> JSON
{
  auto tile_json_array = JSON::array();

  for (const auto& tile : tiles) {
    tile_json_array += emit_tile_definition(tile);
  }

  return tile_json_array;
}

}  // namespace tactile::tiled::tmj
