// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <optional>
#include <span>

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/io/byte_stream.hpp"
#include "tactile/base/util/tile_matrix.hpp"

namespace tactile::common {

/// Represents tile identifier format representations.
enum class TileIdFormat : std::uint8_t
{
  /// Native format, no special bits.
  kTactile,

  /// The format used by Tiled, where upper bits in tile identifiers have special meanings.
  kTiled,
};

/// Reconstructs a tile matrix from a byte stream.
auto deserialize_tile_matrix(ByteSpan byte_stream,
                             const Extent2D& extent,
                             TileIdFormat tile_id_format) -> std::optional<TileMatrix>;

/// Converts a tile matrix to a byte stream.
[[nodiscard]]
auto serialize_tile_matrix(const TileMatrix& tile_matrix,
                           std::span<std::uint8_t> output_buffer)
    -> std::expected<std::size_t, ErrorCode>;

}  // namespace tactile::common
