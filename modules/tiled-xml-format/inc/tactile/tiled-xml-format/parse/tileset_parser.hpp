// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tileset_tile_animation_frame_node(
    pugi::xml_node animation_frame_node) -> Result<ir::AnimationFrame>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tileset_tile_animation_node(pugi::xml_node tile_node)
    -> Result<ir::TileAnimation>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tileset_tile_node(pugi::xml_node tile_node)
    -> Result<ir::Tile>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tileset_tiles(pugi::xml_node tileset_node)
    -> Result<Vector<ir::Tile>>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tileset_image_node(pugi::xml_node tileset_node,
                                                     ir::Tileset& tileset,
                                                     const SaveFormatReadOptions& options)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_embedded_tileset_node(
    pugi::xml_node tileset_node,
    const SaveFormatReadOptions& options) -> Result<ir::Tileset>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_external_tileset(StringView tileset_filename,
                                                   const SaveFormatReadOptions& options)
    -> Result<ir::Tileset>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tileset_node(pugi::xml_node tileset_node,
                                               const SaveFormatReadOptions& options)
    -> Result<ir::TilesetRef>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_tilesets(pugi::xml_node map_node,
                                           const SaveFormatReadOptions& options)
    -> Result<Vector<ir::TilesetRef>>;

}  // namespace tactile::tiled::tmx
