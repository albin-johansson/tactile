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

#include "tactile.hpp"

namespace tactile {

/**
 * \brief Represents the different supported tile layer data encodings.
 */
enum class tile_encoding
{
  plain,  ///< Encode the tile layer data in ordinary plain text.
  base64  ///< Use Base64 encoding.
};

/**
 * \brief Represents the different supported tile data compression methods.
 */
enum class tile_compression
{
  none,  ///< Apply no compression to tile layer data.
  zlib   ///< Compress tile layer data using the Zlib library.
};

/**
 * \brief Provides information about a map context.
 */
struct MapInfo final
{
  usize row_count{};     ///< Total amount of rows.
  usize column_count{};  ///< Total amount of columns.

  int32 tile_width{};   ///< Logical width of all tiles.
  int32 tile_height{};  ///< Logical height of all tiles.

  layer_id next_layer_id{1};    ///< The next available layer identifier.
  object_id next_object_id{1};  ///< The next available object identifier.

  int32 tile_layer_suffix{1};    ///< Incrementing tile layer suffix.
  int32 object_layer_suffix{1};  ///< Incrementing object layer suffix.
  int32 group_layer_suffix{1};   ///< Incrementing group layer suffix.

  tile_encoding encoding{tile_encoding::plain};          ///< Tile layer data encoding.
  tile_compression compression{tile_compression::none};  ///< Tile layer data compression.
};

}  // namespace tactile
