// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/parse/map_parser.hpp"

#include <utility>  // move

#include "tactile/tiled-xml-format/common.hpp"
#include "tactile/tiled-xml-format/parse/layer_parser.hpp"
#include "tactile/tiled-xml-format/parse/meta_parser.hpp"
#include "tactile/tiled-xml-format/parse/tileset_parser.hpp"

namespace tactile::tiled::tmx {

auto parse_map_node(const pugi::xml_node map_node,
                    String filename,
                    const SaveFormatReadOptions& options) -> Result<ir::Map>
{
  ir::Map map {};
  map.meta.name = std::move(filename);

  return parse_to(map_node, "width", map.col_count)
      .and_then([&] { return parse_to(map_node, "height", map.row_count); })
      .and_then([&] { return parse_to(map_node, "tilewidth", map.tile_width); })
      .and_then([&] { return parse_to(map_node, "tileheight", map.tile_height); })
      .and_then([&] { return parse_to(map_node, "nextlayerid", map.next_layer_id); })
      .and_then([&] { return parse_to(map_node, "nextobjectid", map.next_object_id); })
      .and_then([&] { return parse_tilesets(map_node, options); })
      .and_then([&](Vector<ir::TilesetRef>&& tilesets) {
        map.tilesets = std::move(tilesets);
        return kOK;
      })
      .and_then([&] { return parse_layers(map_node, map.tile_format, options); })
      .and_then([&](Vector<ir::Layer>&& layers) {
        map.layers = std::move(layers);
        return kOK;
      })
      .and_then([&] { return parse_metadata(map_node, map.meta); })
      .transform([&] { return std::move(map); });
}

}  // namespace tactile::tiled::tmx
