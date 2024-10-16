// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>     // <=>
#include <cstddef>     // size_t
#include <functional>  // hash

#include "tactile/base/int.hpp"
#include "tactile/base/util/hash.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/// Represents a tile position in a map or tileset.
///
/// Note, this class may represent positions with negative indices.
class TilePos final {
 public:
  /// Creates a tile position at origin, (0, 0).
  constexpr TilePos() noexcept = default;

  constexpr TilePos(const int32 row, const int32 column) noexcept
      : mRow {row},
        mCol {column}
  {
  }

  /// Creates a position from unsigned row and column indices.
  [[nodiscard]] constexpr static auto from(const usize row, const usize column) noexcept
      -> TilePos
  {
    return {static_cast<int32>(row), static_cast<int32>(column)};
  }

  /// Creates a position based on an index and the total number of columns.
  [[nodiscard]] constexpr static auto from_index(const int32 index,
                                                 const int32 n_cols) noexcept -> TilePos
  {
    return TilePos {index / n_cols, index % n_cols};
  }

  /// Sets the row index associated with the position.
  constexpr void set_row(const int32 row) noexcept { mRow = row; }

  /// Sets the column index associated with the position.
  constexpr void set_col(const int32 column) noexcept { mCol = column; }

  /// Creates a new position, offset from this position.
  [[nodiscard]] constexpr auto offset_by(const int32 row,
                                         const int32 column) const noexcept -> TilePos
  {
    return {mRow + row, mCol + column};
  }

  /// Returns a tile position that is one step above this map position.
  [[nodiscard]] constexpr auto north() const noexcept -> TilePos
  {
    return offset_by(-1, 0);
  }

  /// Returns a tile position that is one step below this map position.
  [[nodiscard]] constexpr auto south() const noexcept -> TilePos
  {
    return offset_by(1, 0);
  }

  /// Returns a tile position that is one step to the left of this map position.
  [[nodiscard]] constexpr auto west() const noexcept -> TilePos
  {
    return offset_by(0, -1);
  }

  /// Returns a tile position that is one step to the right of this map position.
  [[nodiscard]] constexpr auto east() const noexcept -> TilePos
  {
    return offset_by(0, 1);
  }

  /// Returns the row index of the tile position.
  [[nodiscard]] constexpr auto row() const noexcept -> int32 { return mRow; }

  /// Returns the column index of the tile position.
  [[nodiscard]] constexpr auto col() const noexcept -> int32 { return mCol; }

  /// Returns the row index converted to a y-coordinate.
  [[nodiscard]] constexpr auto row_to_y(const int32 tile_height) const noexcept -> int32
  {
    return mRow * tile_height;
  }

  /// Returns the col index converted to an x-coordinate.
  [[nodiscard]] constexpr auto col_to_x(const int32 tile_width) const noexcept -> int32
  {
    return mCol * tile_width;
  }

  [[nodiscard]] constexpr auto as_vec2() const noexcept -> Int2 { return {mCol, mRow}; }

  [[nodiscard]] constexpr auto as_vec2f() const noexcept -> Float2
  {
    return {static_cast<float>(mCol), static_cast<float>(mRow)};
  }

  [[nodiscard]] constexpr auto operator<=>(const TilePos&) const noexcept = default;

 private:
  int32 mRow {};
  int32 mCol {};
};

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

}  // namespace tactile

template <>
struct std::hash<tactile::TilePos> final {
  [[nodiscard]] auto operator()(const tactile::TilePos& pos) const noexcept -> std::size_t
  {
    return tactile::hash_combine(pos.row(), pos.col());
  }
};
