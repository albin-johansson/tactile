// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/emit/map_emitter.hpp"

#include "tactile/tiled-xml-format/emit/layer_emitter.hpp"
#include "tactile/tiled-xml-format/emit/property_emitter.hpp"
#include "tactile/tiled-xml-format/emit/tileset_emitter.hpp"
#include "tactile/tiled-xml-format/version.hpp"

namespace tactile::tiled::tmx {

void append_map_node(pugi::xml_node root_node,
                     const ir::Map& map,
                     const SaveFormatWriteOptions& options)
{
  auto map_node = root_node.append_child("map");

  map_node.append_attribute("version").set_value(kTiledFormatVersion);
  map_node.append_attribute("tiledversion").set_value(kTiledVersion);

  map_node.append_attribute("orientation").set_value("orthogonal");
  map_node.append_attribute("renderorder").set_value("right-down");
  map_node.append_attribute("infinite").set_value(0);

  map_node.append_attribute("tilewidth").set_value(map.tile_width);
  map_node.append_attribute("tileheight").set_value(map.tile_height);

  map_node.append_attribute("width").set_value(map.col_count);
  map_node.append_attribute("height").set_value(map.row_count);

  map_node.append_attribute("nextlayerid").set_value(map.next_layer_id);
  map_node.append_attribute("nextobjectid").set_value(map.next_object_id);

  append_properties_node(map_node, map.meta.properties);

  for (const auto& tileset_ref : map.tilesets) {
    append_tileset_node(map_node, tileset_ref, options);
  }

  for (const auto& layer : map.layers) {
    append_layer_node(map_node, map, layer, options);
  }
}

}  // namespace tactile::tiled::tmx
