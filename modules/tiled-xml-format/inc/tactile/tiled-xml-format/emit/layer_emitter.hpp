// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

TACTILE_TMX_FORMAT_API void append_tile_layer_node(pugi::xml_node map_node,
                                                   const ir::Layer& layer,
                                                   const ir::TileFormat& tile_format,
                                                   const SaveFormatWriteOptions& options);

TACTILE_TMX_FORMAT_API void append_object_layer_node(pugi::xml_node map_node,
                                                     const ir::Layer& layer);

TACTILE_TMX_FORMAT_API void append_group_layer_node(
    pugi::xml_node map_node,
    const ir::Map& map,
    const ir::Layer& layer,
    const SaveFormatWriteOptions& options);

TACTILE_TMX_FORMAT_API void append_layer_node(pugi::xml_node map_node,
                                              const ir::Map& map,
                                              const ir::Layer& layer,
                                              const SaveFormatWriteOptions& options);

}  // namespace tactile::tiled::tmx
