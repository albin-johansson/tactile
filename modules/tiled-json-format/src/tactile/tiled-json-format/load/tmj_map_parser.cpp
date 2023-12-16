// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-json-format/load/tmj_map_parser.hpp"

#include <utility>  // move

#include "tactile/foundation/log/logger.hpp"
#include "tactile/tiled-json-format/load/tmj_layer_parser.hpp"
#include "tactile/tiled-json-format/load/tmj_meta_parser.hpp"
#include "tactile/tiled-json-format/load/tmj_tileset_parser.hpp"

namespace tactile::tmj {

auto parse_map(const JSON& json, String filename, const SaveFormatReadOptions& options)
    -> Result<ir::Map>
{
  ir::Map map {};
  map.name = std::move(filename);

  String orientation {};
  return parse(json, "orientation", orientation)
      .and_then([&]() -> Result<void> {
        if (orientation != "orthogonal") {
          return unexpected(
              make_save_format_error(SaveFormatError::kUnsupportedOrientation));
        }
        return kSuccess;
      })
      .and_then([&] { return parse(json, "width", map.col_count); })
      .and_then([&] { return parse(json, "height", map.row_count); })
      .and_then([&] { return parse(json, "tilewidth", map.tile_width); })
      .and_then([&] { return parse(json, "tileheight", map.tile_height); })
      .and_then([&] { return parse(json, "nextlayerid", map.next_layer_id); })
      .and_then([&] { return parse(json, "nextobjectid", map.next_object_id); })
      .and_then([&] { return parse_metadata(json, map.meta); })
      .and_then([&] { return parse_tilesets(json, options, map); })
      .and_then([&] { return parse_layers(json, map); })
      .transform([&] { return std::move(map); });
}

}  // namespace tactile::tmj