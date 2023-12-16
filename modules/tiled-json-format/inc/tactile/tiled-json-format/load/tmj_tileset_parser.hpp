// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"
#include "tactile/tiled-json-format/common/json.hpp"

namespace tactile::tmj {

[[nodiscard]]
TACTILE_TMJ_API auto parse_tile_animation_frame(const JSON& frame_json)
    -> Result<ir::AnimationFrame>;

[[nodiscard]]
TACTILE_TMJ_API auto parse_tile_animation(const JSON& json, ir::TileAnimation& animation)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMJ_API auto parse_tile_objects(const JSON& json, Vector<ir::Object>& objects)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMJ_API auto parse_tileset_tile(const JSON& json, const ir::Tileset& tileset)
    -> Result<ir::Tile>;

[[nodiscard]]
TACTILE_TMJ_API auto parse_tileset_tiles(const JSON& json, ir::Tileset& tileset)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMJ_API auto parse_tileset(const JSON& json, const SaveFormatReadOptions& options)
    -> Result<ir::Tileset>;

[[nodiscard]]
TACTILE_TMJ_API auto parse_tileset_ref(const JSON& json,
                                       const SaveFormatReadOptions& options)
    -> Result<ir::TilesetRef>;

[[nodiscard]]
TACTILE_TMJ_API auto parse_tilesets(const JSON& map_json,
                                    const SaveFormatReadOptions& options,
                                    ir::Map& map) -> Result<void>;

}  // namespace tactile::tmj
