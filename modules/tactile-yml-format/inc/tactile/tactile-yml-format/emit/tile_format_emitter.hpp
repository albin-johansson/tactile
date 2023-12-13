// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/io/compression/compression_kind.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

TACTILE_YML_FORMAT_API void emit(YAML::Emitter& emitter, TileEncoding tile_encoding);

TACTILE_YML_FORMAT_API void emit(YAML::Emitter& emitter,
                                 CompressionKind compression_kind);

TACTILE_YML_FORMAT_API void emit_tile_format(YAML::Emitter& emitter,
                                             const ir::TileFormat& format);

}  // namespace tactile::yml_format
