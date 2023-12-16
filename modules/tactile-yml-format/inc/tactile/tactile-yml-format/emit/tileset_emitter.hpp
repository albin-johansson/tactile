// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

TACTILE_YML_FORMAT_API void emit_tileset_tile_animation(YAML::Emitter& emitter,
                                                        const ir::Tile& tile);

TACTILE_YML_FORMAT_API void emit_tileset_tile(YAML::Emitter& emitter,
                                              const ir::Tile& tile);

TACTILE_YML_FORMAT_API void emit_tileset_tile_seq(YAML::Emitter& emitter,
                                                  const ir::Tileset& tileset);

TACTILE_YML_FORMAT_API void emit_tileset(YAML::Emitter& emitter,
                                         const ir::Tileset& tileset);

TACTILE_YML_FORMAT_API void emit_tileset_ref(YAML::Emitter& emitter,
                                             const ir::TilesetRef& tileset_ref,
                                             const SaveFormatWriteOptions& options);

TACTILE_YML_FORMAT_API void emit_tileset_seq(YAML::Emitter& emitter,
                                             const ir::Map& map,
                                             const SaveFormatWriteOptions& options);

}  // namespace tactile::yml_format
