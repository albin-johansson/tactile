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

#include "tiles.hpp"

namespace tactile {

auto make_tile_row(const usize n_cols) -> TileRow
{
  TileRow row;
  row.reserve(n_cols);
  row.assign(n_cols, 0);
  return row;
}

auto make_tile_matrix(const usize n_rows, const usize n_cols) -> TileMatrix
{
  TileMatrix tiles;
  tiles.reserve(n_rows);
  tiles.assign(n_rows, make_tile_row(n_cols));
  return tiles;
}

}  // namespace tactile
