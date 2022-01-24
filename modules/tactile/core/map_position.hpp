#pragma once

#include <compare>  // <=>

#include <tactile_def.hpp>

namespace tactile {

/// \addtogroup core
/// \{

/**
 * \brief Represents a tile position in a map or tileset.
 *
 * \note This class may represent map positions with negative indices.
 */
class map_position final {
 public:
  /**
   * \brief Creates a map position at origin, (0, 0).
   */
  constexpr map_position() noexcept = default;

  /**
   * \brief Creates a map position.
   *
   * \param row the row index.
   * \param column the column index.
   */
  constexpr map_position(const int32 row, const int32 column) noexcept
      : mRow{row}
      , mCol{column}
  {}

  [[nodiscard]] constexpr static auto from(const usize row, const usize column) noexcept
      -> map_position
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
      -> map_position
  {
    return map_position{index / nCols, index % nCols};
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
      -> map_position
  {
    return {mRow + row, mCol + column};
  }

  /**
   * \brief Returns a map position that is one step north of this map position.
   *
   * \return a position one step to the "north".
   */
  [[nodiscard]] constexpr auto north() const noexcept -> map_position
  {
    return {mRow - 1, mCol};
  }

  /**
   * \brief Returns a map position that is one step south of this map position.
   *
   * \return a position one step to the "south".
   */
  [[nodiscard]] constexpr auto south() const noexcept -> map_position
  {
    return {mRow + 1, mCol};
  }

  /**
   * \brief Returns a map position that is one step to the west of this map
   * position.
   *
   * a position one step to the "west".
   */
  [[nodiscard]] constexpr auto west() const noexcept -> map_position
  {
    return {mRow, mCol - 1};
  }

  /**
   * \brief Returns a map position that is one step to the east of this map
   * position.
   *
   * \return a position one step to the "east".
   */
  [[nodiscard]] constexpr auto east() const noexcept -> map_position
  {
    return {mRow, mCol + 1};
  }

  /**
   * \brief Returns the row index of the map position.
   *
   * \return the row index.
   */
  [[nodiscard]] constexpr auto row() const noexcept -> int32 { return mRow; }

  /**
   * \brief Returns the column index of the map position.
   *
   * \return the column index.
   */
  [[nodiscard]] constexpr auto col() const noexcept -> int32 { return mCol; }

  /**
   * \brief Returns the raw row index associated with the map position.
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
   * \brief Returns the column index associated with the map position.
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

  [[nodiscard]] constexpr auto operator<=>(const map_position&) const noexcept = default;

 private:
  int32 mRow{};
  int32 mCol{};
};

/// \name Map position operators
/// \{

[[nodiscard]] constexpr auto operator+(const map_position& lhs,
                                       const map_position& rhs) noexcept -> map_position
{
  return {lhs.row() + rhs.row(), lhs.col() + rhs.col()};
}

[[nodiscard]] constexpr auto operator-(const map_position& lhs,
                                       const map_position& rhs) noexcept -> map_position
{
  return {lhs.row() - rhs.row(), lhs.col() - rhs.col()};
}

[[nodiscard]] constexpr auto operator*(const map_position& lhs,
                                       const map_position& rhs) noexcept -> map_position
{
  return {lhs.row() * rhs.row(), lhs.col() * rhs.col()};
}

[[nodiscard]] constexpr auto operator/(const map_position& lhs,
                                       const map_position& rhs) noexcept -> map_position
{
  return {lhs.row() / rhs.row(), lhs.col() / rhs.col()};
}

/// \} End of map position operators

/// \} End of group core

}  // namespace tactile
