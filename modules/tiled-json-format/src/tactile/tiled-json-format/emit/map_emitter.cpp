// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-json-format/emit/map_emitter.hpp"

#include <utility>  // move

#include "tactile/tiled-json-format/common.hpp"
#include "tactile/tiled-json-format/emit/layer_emitter.hpp"
#include "tactile/tiled-json-format/emit/meta_emitter.hpp"
#include "tactile/tiled-json-format/emit/tileset_emitter.hpp"

namespace tactile::tiled::tmj {

auto emit_map(const ir::Map& map, const SaveFormatWriteOptions& options) -> Result<JSON>
{
  auto map_json = JSON::object();

  map_json["tiledversion"] = "1.9.2";
  map_json["version"] = "1.7";

  if (map.tile_format.compression == CompressionMode::kZlib &&
      map.tile_format.compression_level.has_value()) {
    map_json["compressionlevel"] = *map.tile_format.compression_level;
  }
  else if (map.tile_format.compression == CompressionMode::kZstd &&
           map.tile_format.compression_level.has_value()) {
    map_json["compressionlevel"] = *map.tile_format.compression_level;
  }
  else {
    map_json["compressionlevel"] = -1;
  }

  map_json["type"] = "map";
  map_json["width"] = map.col_count;
  map_json["height"] = map.row_count;
  map_json["tilewidth"] = map.tile_width;
  map_json["tileheight"] = map.tile_height;
  map_json["nextlayerid"] = map.next_layer_id;
  map_json["nextobjectid"] = map.next_object_id;
  map_json["orientation"] = "orthogonal";
  map_json["renderorder"] = "right-down";
  map_json["infinite"] = false;
  map_json["properties"] = emit_property_array(map.meta);

  if (auto tileset_json_array = emit_tileset_array(map, options)) {
    map_json["tilesets"] = std::move(*tileset_json_array);
  }
  else {
    return propagate_unexpected(tileset_json_array);
  }

  if (auto layer_json_array = emit_layer_array(map)) {
    map_json["layers"] = std::move(*layer_json_array);
  }
  else {
    return propagate_unexpected(layer_json_array);
  }

  return map_json;
}

}  // namespace tactile::tiled::tmj
