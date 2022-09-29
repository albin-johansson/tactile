/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>  // string

#include "core/common/vocabulary.hpp"
#include "core/layer/tile_format.hpp"
#include "core/type/vector.hpp"

namespace tactile::io {

/// Encodes a tile matrix using Base64.
///
/// This function works by converting the tile matrix into a one-dimensional array of tile
/// identifiers, to which then the specified compression method is applied. The result is
/// subsequently encoded using Base64 as a plain string.
///
/// \param tiles the source tile data to encode.
/// \param rows the amount of tile rows.
/// \param columns the amount of tile columns.
/// \param compression the compression method.
///
/// \return the encoded (and potentially compressed) tile data.
[[nodiscard]] auto base64_encode_tiles(const TileMatrix& tiles,
                                       usize rows,
                                       usize columns,
                                       TileCompression compression) -> std::string;

/// Decodes tile data using Base64 and turns it into a tile matrix.
///
/// The compression method must be the same as when the tiles were encoded!
///
/// \param tiles the encoded tile data that will be decoded.
/// \param rows the amount of tile rows.
/// \param columns the amount of tile columns.
/// \param compression the compression method used when encoding the tiles.
///
/// \return the decoded tile data.
[[nodiscard]] auto base64_decode_tiles(const std::string& tiles,
                                       usize rows,
                                       usize columns,
                                       TileCompression compression) -> TileMatrix;

}  // namespace tactile::io
