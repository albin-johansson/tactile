// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tmj-format/api.hpp"
#include "tactile/tmj-format/common/json.hpp"

namespace tactile::tmj {

[[nodiscard]]
TACTILE_TMJ_API auto emit_embedded_tileset(const ir::Tileset& tileset,
                                           TileID first_tile_id,
                                           const SaveFormatWriteOptions& options)
    -> Result<JSON>;

[[nodiscard]]
TACTILE_TMJ_API auto emit_external_tileset(const ir::TilesetRef& tileset_ref,
                                           const SaveFormatWriteOptions& options)
    -> Result<JSON>;

[[nodiscard]]
TACTILE_TMJ_API auto emit_tileset_ref(const ir::TilesetRef& tileset_ref,
                                      const SaveFormatWriteOptions& options)
    -> Result<JSON>;

[[nodiscard]]
TACTILE_TMJ_API auto emit_tileset_array(const ir::Map& map,
                                        const SaveFormatWriteOptions& options)
    -> Result<JSON>;

}  // namespace tactile::tmj
