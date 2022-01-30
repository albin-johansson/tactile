#pragma once

#include <compare>  // <=>

#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup core
/// \{

/**
 * \brief Represents a tile position in a map or tileset.
 *
 * \note This class may represent positions with negative indices.
 */
class tile_position final {
 public:
  /**
   * \brief Creates a tile position at origin, (0, 0).
   */
  constexpr tile_position() noexcept = default;

  /**
   * \brief Creates a tile position.
   *
   * \param row the row index.
   * \param column the column index.
   */
  constexpr tile_position(const int32 row, const int32 column) noexcept
      : mRow{row}
      , mCol{column}
  {}

  [[nodiscard]] constexpr static auto from(const usize row, const usize column) noexcept
      -> tile_position
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
                                                 const int32 nCols) noexcept
      -> tile_position
  {
    return tile_position{index / nCols, index % nCols};
  }

  /**
   * \brief Sets the row index associated with the position.
   */
  constexpr void set_row(const int32 row) noexcept { mRow = row; }

  /**
   * \brief Sets the column index associated with the position.
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
                                         const int32 column) const noexcept
      -> tile_position
  {
    return {mRow + row, mCol + column};
  }

  /**
   * \brief Returns a tile position that is one step north of this tile position.
   *
   * \return a position one step to the "north".
   */
  [[nodiscard]] constexpr auto north() const noexcept -> tile_position
  {
    return {mRow - 1, mCol};
  }

  /**
   * \brief Returns a tile position that is one step south of this tile position.
   *
   * \return a position one step to the "south".
   */
  [[nodiscard]] constexpr auto south() const noexcept -> tile_position
  {
    return {mRow + 1, mCol};
  }

  /**
   * \brief Returns a tile position that is one step to the west of this map
   * position.
   *
   * a position one step to the "west".
   */
  [[nodiscard]] constexpr auto west() const noexcept -> tile_position
  {
    return {mRow, mCol - 1};
  }

  /**
   * \brief Returns a tile position that is one step to the east of this map
   * position.
   *
   * \return a position one step to the "east".
   */
  [[nodiscard]] constexpr auto east() const noexcept -> tile_position
  {
    return {mRow, mCol + 1};
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

  [[nodiscard]] constexpr auto operator<=>(const tile_position&) const noexcept = default;

 private:
  int32 mRow{};
  int32 mCol{};
};

/// \name Tile position operators
/// \{

[[nodiscard]] constexpr auto operator+(const tile_position& lhs,
                                       const tile_position& rhs) noexcept -> tile_position
{
  return {lhs.row() + rhs.row(), lhs.col() + rhs.col()};
}

[[nodiscard]] constexpr auto operator-(const tile_position& lhs,
                                       const tile_position& rhs) noexcept -> tile_position
{
  return {lhs.row() - rhs.row(), lhs.col() - rhs.col()};
}

[[nodiscard]] constexpr auto operator*(const tile_position& lhs,
                                       const tile_position& rhs) noexcept -> tile_position
{
  return {lhs.row() * rhs.row(), lhs.col() * rhs.col()};
}

[[nodiscard]] constexpr auto operator/(const tile_position& lhs,
                                       const tile_position& rhs) noexcept -> tile_position
{
  return {lhs.row() / rhs.row(), lhs.col() / rhs.col()};
}

/// \} End of tile position operators

/// \} End of group core

}  // namespace tactile
