// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/tile_format.hpp"
#include "core/tile/tile_extent.hpp"
#include "core/tile/tile_matrix.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

/// Encodes a tile matrix using Base64.
///
/// This function works by converting the tile matrix into a one-dimensional array of tile
/// identifiers, to which then the specified compression method is applied. The result is
/// subsequently encoded using Base64 as a plain string.
///
/// \param tiles the source tile data to encode.
/// \param extent the dimensions of the tile data.
/// \param compression the compression method.
///
/// \return the encoded (and potentially compressed) tile data.
[[nodiscard]] auto base64_encode_tiles(const TileMatrix& tiles,
                                       TileExtent extent,
                                       CompressionType compression) -> String;

/// Decodes tile data using Base64 and turns it into a tile matrix.
///
/// The compression method must be the same as when the tiles were encoded!
///
/// \param tiles the encoded tile data that will be decoded.
/// \param extent the dimensions of the encoded tiles.
/// \param compression the compression method used when encoding the tiles.
///
/// \return the decoded tile data.
[[nodiscard]] auto base64_decode_tiles(StringView tiles,
                                       TileExtent extent,
                                       CompressionType compression) -> TileMatrix;

}  // namespace tactile
