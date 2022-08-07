#pragma once

#include <string>  // string
#include <vector>  // vector

#include "core/common/ints.hpp"
#include "core/common/tiles.hpp"
#include "core/compression.hpp"

namespace tactile {

/**
 * Encodes a tile matrix using Base64.
 *
 * This function works by converting the tile matrix into a one-dimensional array of tile
 * identifiers, to which then the specified compression method is applied. The result is
 * subsequently encoded using Base64 as a plain string.
 *
 * \param tiles the source tile data to encode.
 * \param rows the amount of tile rows.
 * \param columns the amount of tile columns.
 * \param compression the compression method.
 *
 * \return the encoded (and potentially compressed) tile data.
 */
[[nodiscard]] auto base64_encode_tiles(const TileMatrix& tiles,
                                       usize             rows,
                                       usize             columns,
                                       TileCompression   compression) -> std::string;

/**
 * Decodes tile data using Base64 and turns it into a tile matrix.
 *
 * The compression method must be the same as when the tiles were encoded!
 *
 * \param tiles the encoded tile data that will be decoded.
 * \param rows the amount of tile rows.
 * \param columns the amount of tile columns.
 * \param compression the compression method used when encoding the tiles.
 *
 * \return the decoded tile data.
 */
[[nodiscard]] auto base64_decode_tiles(const std::string& tiles,
                                       usize              rows,
                                       usize              columns,
                                       TileCompression    compression) -> TileMatrix;

}  // namespace tactile
