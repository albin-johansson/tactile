// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/compression/compression_kind.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/misc/tile_encoding.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_tile_encoding(StringView name) -> Maybe<TileEncoding>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_compression_kind(StringView name)
    -> Maybe<CompressionKind>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_tile_format(const YAML::Node& root_node)
    -> Result<ir::TileFormat>;

}  // namespace tactile::yml_format
