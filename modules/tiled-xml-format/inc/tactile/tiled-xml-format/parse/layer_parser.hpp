// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/misc/layer_type.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_layer_type(pugi::xml_node layer_node)
    -> Result<LayerType>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tile_layer_csv_data_node(pugi::xml_node data_node,
                                                           ir::Layer& layer,
                                                           ir::TileFormat& tile_format)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tile_layer_base64_data_node(pugi::xml_node data_node,
                                                              ir::Layer& layer,
                                                              ir::TileFormat& tile_format)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tile_layer_verbose_data_node(
    pugi::xml_node data_node,
    ir::Layer& layer,
    ir::TileFormat& tile_format) -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tile_layer_data_node(pugi::xml_node layer_node,
                                                       ir::Layer& layer,
                                                       ir::TileFormat& tile_format)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tile_layer_node(pugi::xml_node layer_node,
                                                  ir::Layer& layer,
                                                  ir::TileFormat& tile_format)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_object_layer_node(pugi::xml_node layer_node,
                                                    ir::Layer& layer) -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_group_layer_node(pugi::xml_node layer_node,
                                                   ir::Layer& layer,
                                                   ir::TileFormat& tile_format,
                                                   const SaveFormatReadOptions& options)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_layer_node(pugi::xml_node layer_node,
                                             ir::TileFormat& tile_format,
                                             const SaveFormatReadOptions& options)
    -> Result<ir::Layer>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_layers(pugi::xml_node map_node,
                                         ir::TileFormat& tile_format,
                                         const SaveFormatReadOptions& options)
    -> Result<Vector<ir::Layer>>;

}  // namespace tactile::tiled::tmx
