/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "tile_matrix.hpp"

namespace tactile {

auto make_tile_row(const usize column_count) -> TileRow
{
  TileRow row;
  row.reserve(column_count);
  row.assign(column_count, 0);
  return row;
}

auto make_tile_matrix(const TileExtent extent) -> TileMatrix
{
  TileMatrix tiles;
  tiles.reserve(extent.rows);
  tiles.assign(extent.rows, make_tile_row(extent.cols));
  return tiles;
}

}  // namespace tactile
