// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_map_emitter.hpp"

#include <utility>  // move

#include "tactile/base/document/map_view.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/tiled_tmj_format/tmj_format_meta_emitter.hpp"

namespace tactile {

auto emit_tiled_tmj_map(const IMapView& map) -> nlohmann::json
{
  if (map.component_count() > 0) {
    log(LogLevel::kWarn, "Components are ignored in Tiled TMJ save files");
  }

  const auto extent = map.get_extent();
  const auto tile_size = map.get_tile_size();
  const auto tileset_count = map.tileset_count();
  const auto layer_count = map.layer_count();

  auto map_json = nlohmann::json::object();

  map_json["type"] = "map";
  map_json["tiledversion"] = "1.9.0";
  map_json["version"] = "1.7";
  map_json["renderorder"] = "right-down";
  map_json["orientation"] = "orthogonal";
  map_json["infinite"] = false;
  map_json["width"] = extent.cols;
  map_json["height"] = extent.rows;
  map_json["tilewidth"] = tile_size.x();
  map_json["tileheight"] = tile_size.y();
  map_json["nextlayerid"] = map.get_next_layer_id();
  map_json["nextobjectid"] = map.get_next_object_id();
  map_json["compressionlevel"] = map.get_compression_level().value_or(-1);

  emit_tiled_tmj_metadata(map.get_meta(), map_json);

  auto tileset_json_array = nlohmann::json::array();
  tileset_json_array.get_ref<nlohmann::json::array_t&>().reserve(tileset_count);
  map_json["tilesets"] = std::move(tileset_json_array);

  auto layer_json_array = nlohmann::json::array();
  layer_json_array.get_ref<nlohmann::json::array_t&>().reserve(layer_count);
  map_json["layers"] = std::move(layer_json_array);

  return map_json;
}

}  // namespace tactile
