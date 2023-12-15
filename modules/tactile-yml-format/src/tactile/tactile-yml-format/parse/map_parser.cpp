// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/parse/map_parser.hpp"

#include <utility>  // move

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/parse/common.hpp"
#include "tactile/tactile-yml-format/parse/component_parser.hpp"
#include "tactile/tactile-yml-format/parse/layer_parser.hpp"
#include "tactile/tactile-yml-format/parse/meta_parser.hpp"
#include "tactile/tactile-yml-format/parse/tile_format_parser.hpp"
#include "tactile/tactile-yml-format/parse/tileset_parser.hpp"

namespace tactile::yml_format {

auto parse_map(String map_name,
               const YAML::Node& root_node,
               const SaveFormatReadOptions& options) -> Result<ir::Map>
{
  TACTILE_LOG_TRACE("Parsing map node at {}:{}",
                    root_node.Mark().line,
                    root_node.Mark().column);

  ir::Map map {};
  map.meta.name = std::move(map_name);

  return parse_to(root_node, "row-count", map.row_count)
      .and_then([&] { return parse_to(root_node, "column-count", map.col_count); })
      .and_then([&] { return parse_to(root_node, "tile-width", map.tile_width); })
      .and_then([&] { return parse_to(root_node, "tile-height", map.tile_height); })
      .and_then([&] { return parse_to(root_node, "next-layer-id", map.next_layer_id); })
      .and_then([&] { return parse_to(root_node, "next-object-id", map.next_object_id); })
      .and_then([&] { return parse_components(root_node); })
      .and_then([&](Vector<ir::Component>&& components) {
        map.components = std::move(components);
        return kOK;
      })
      .and_then([&] { return parse_tile_format(root_node); })
      .and_then([&](const ir::TileFormat& tile_format) {
        map.tile_format = tile_format;
        return kOK;
      })
      .and_then([&] { return parse_tileset_refs(root_node, map, options); })
      .and_then([&](Vector<ir::TilesetRef>&& tileset_refs) {
        map.tilesets = std::move(tileset_refs);
        return kOK;
      })
      .and_then([&] { return parse_layers(root_node, map); })
      .and_then([&](Vector<ir::Layer>&& layers) {
        map.layers = std::move(layers);
        return kOK;
      })
      .and_then([&] { return parse_metadata(root_node, map, map.meta); })
      .transform([&] { return std::move(map); });
}

}  // namespace tactile::yml_format
