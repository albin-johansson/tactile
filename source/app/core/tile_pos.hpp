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

#include <compare>  // <=>

#include <boost/container_hash/hash.hpp>

#include "core/common/ints.hpp"
#include "misc/assert.hpp"

namespace tactile {

/// \addtogroup core
/// \{

/**
 * \brief Represents a tile position in a map or tileset.
 *
 * \note This class may represent positions with negative indices.
 */
class TilePos final
{
 public:
  /**
   * \brief Creates a tile position at origin, (0, 0).
   */
  constexpr TilePos() noexcept = default;

  /**
   * \brief Creates a tile position.
   *
   * \param row the row index.
   * \param column the column index.
   */
  constexpr TilePos(const int32 row, const int32 column) noexcept
      : mRow{row}
      , mCol{column}
  {}

  [[nodiscard]] constexpr static auto from(const usize row, const usize column) noexcept
      -> TilePos
  {
    return {static_cast<int32>(row), static_cast<int32>(column)};
  }

  /**
   * \brief Creates a position based on an index and the total number of columns.
   *
   * \param index the index of the position.
   * \param nCols the total number of columns.
   *
   * \return a position translated from the supplied index.
   */
  [[nodiscard]] constexpr static auto from_index(const int32 index,
                                                 const int32 nCols) noexcept -> TilePos
  {
    return TilePos{index / nCols, index % nCols};
  }

  /**
   * \brief Sets the row index associated with the position.
   *
   * \param row the new row index.
   */
  constexpr void set_row(const int32 row) noexcept { mRow = row; }

  /**
   * \brief Sets the column index associated with the position.
   *
   * \param column the new column index.
   */
  constexpr void set_col(const int32 column) noexcept { mCol = column; }

  /**
   * \brief Creates and position that is offset from this position.
   *
   * \param row the offset that will be added to the row index.
   * \param column the offset that will be added to the column index.
   *
   * \return a position that is offset from this position.
   */
  [[nodiscard]] constexpr auto offset_by(const int32 row,
                                         const int32 column) const noexcept -> TilePos
  {
    return {mRow + row, mCol + column};
  }

  /**
   * \brief Returns a tile position that is one step north of this tile position.
   *
   * \return a position one step to the "north".
   */
  [[nodiscard]] constexpr auto north() const noexcept -> TilePos
  {
    return offset_by(-1, 0);
  }

  /**
   * \brief Returns a tile position that is one step south of this tile position.
   *
   * \return a position one step to the "south".
   */
  [[nodiscard]] constexpr auto south() const noexcept -> TilePos
  {
    return offset_by(1, 0);
  }

  /**
   * \brief Returns a tile position that is one step to the west of this map
   * position.
   *
   * a position one step to the "west".
   */
  [[nodiscard]] constexpr auto west() const noexcept -> TilePos
  {
    return offset_by(0, -1);
  }

  /**
   * \brief Returns a tile position that is one step to the east of this map
   * position.
   *
   * \return a position one step to the "east".
   */
  [[nodiscard]] constexpr auto east() const noexcept -> TilePos
  {
    return offset_by(0, 1);
  }

  /**
   * \brief Returns the row index of the tile position.
   *
   * \return the row index.
   */
  [[nodiscard]] constexpr auto row() const noexcept -> int32 { return mRow; }

  /**
   * \brief Returns the column index of the tile position.
   *
   * \return the column index.
   */
  [[nodiscard]] constexpr auto col() const noexcept -> int32 { return mCol; }

  /**
   * \brief Returns the raw row index associated with the tile position.
   *
   * \details This function is meant to be used when indexing vectors, etc.
   *
   * \return the raw row index.
   */
  [[nodiscard]] constexpr auto row_index() const noexcept -> usize
  {
    TACTILE_ASSERT(mRow >= 0);
    return static_cast<usize>(mRow);
  }

  /**
   * \brief Returns the column index associated with the tile position.
   *
   * \details This function is meant to be used when indexing vectors, etc.
   *
   * \return the raw column index.
   */
  [[nodiscard]] constexpr auto col_index() const noexcept -> usize
  {
    TACTILE_ASSERT(mCol >= 0);
    return static_cast<usize>(mCol);
  }

  /**
   * \brief Returns the row index converted to a y-coordinate.
   *
   * \param tileHeight the current tile height.
   *
   * \return the row index converted to a y-coordinate.
   */
  [[nodiscard]] constexpr auto row_to_y(const int32 tileHeight) const noexcept -> int32
  {
    return mRow * tileHeight;
  }

  /**
   * \brief Returns the col index converted to an x-coordinate.
   *
   * \param tileWidth the current tile width.
   *
   * \return the column index converted to an x-coordinate.
   */
  [[nodiscard]] constexpr auto col_to_x(const int32 tileWidth) const noexcept -> int32
  {
    return mCol * tileWidth;
  }

  [[nodiscard]] constexpr auto operator<=>(const TilePos&) const noexcept = default;

 private:
  int32 mRow{};
  int32 mCol{};
};

/// \name Tile position operators
/// \{

[[nodiscard]] constexpr auto operator+(const TilePos& lhs, const TilePos& rhs) noexcept
    -> TilePos
{
  return {lhs.row() + rhs.row(), lhs.col() + rhs.col()};
}

[[nodiscard]] constexpr auto operator-(const TilePos& lhs, const TilePos& rhs) noexcept
    -> TilePos
{
  return {lhs.row() - rhs.row(), lhs.col() - rhs.col()};
}

[[nodiscard]] constexpr auto operator*(const TilePos& lhs, const TilePos& rhs) noexcept
    -> TilePos
{
  return {lhs.row() * rhs.row(), lhs.col() * rhs.col()};
}

[[nodiscard]] constexpr auto operator/(const TilePos& lhs, const TilePos& rhs) noexcept
    -> TilePos
{
  return {lhs.row() / rhs.row(), lhs.col() / rhs.col()};
}

/// \} End of tile position operators

/// \} End of group core

}  // namespace tactile

namespace std {

template <>
struct hash<tactile::TilePos> final
{
  [[nodiscard]] auto operator()(const tactile::TilePos& pos) const -> size_t
  {
    size_t res = 0;
    boost::hash_combine(res, pos.row());
    boost::hash_combine(res, pos.col());
    return res;
  }
};

}  // namespace std
