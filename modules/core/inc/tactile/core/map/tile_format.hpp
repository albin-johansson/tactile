// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/io/compression/compression_kind.hpp"
#include "tactile/foundation/misc/tile_encoding.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * Describes the data format used by tile layers in a tilemap.
 */
struct TileFormat final {
  TileEncoding encoding {TileEncoding::kPlainText};
  CompressionMode compression {CompressionMode::kNone};
  Maybe<int> compression_level {};
};

}  // namespace tactile::core
