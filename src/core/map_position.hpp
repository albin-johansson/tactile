#pragma once

#include "aliases/ints.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

/**
 * \class MapPosition
 *
 * \brief Represents a row/column position in a tilemap.
 *
 * \headerfile map_position.hpp
 */
class MapPosition final
{
 public:
  /// Creates a map position that represents (0, 0).
  constexpr MapPosition() noexcept = default;

  /**
   * \brief Creates a map position.
   *
   * \param row the row index.
   * \param column the column index.
   */
  constexpr MapPosition(const int32 row, const int32 column) noexcept
      : mRow{row}
      , mCol{column}
  {}

  /**
   * \brief Creates a position based on an index and the total number of columns.
   *
   * \param index the index of the position.
   * \param nCols the total number of columns.
   *
   * \return a row/column position translated from the supplied index.
   */
  [[nodiscard]] constexpr static auto FromIndex(const int32 index,
                                                const int32 nCols) noexcept -> MapPosition
  {
    return MapPosition{index / nCols, index % nCols};
  }

  /**
   * \brief Sets the row index associated with the position.
   */
  constexpr void SetRow(const int32 row)
  {
    mRow = row;
  }

  /**
   * \brief Sets the column index associated with the position.
   */
  constexpr void SetColumn(const int32 column)
  {
    mCol = column;
  }

  /**
   * \brief Creates and position that is offset from this position.
   *
   * \param row the offset that will be added to the row index.
   * \param column the offset that will be added to the column index.
   *
   * \return a position that is offset from this position.
   */
  [[nodiscard]] constexpr auto OffsetBy(const int32 row, const int32 column) const
      -> MapPosition
  {
    return {mRow + row, mCol + column};
  }

  /**
   * \brief Returns a map position that is one step north of this map position.
   *
   * \return a position one step to the "north".
   */
  [[nodiscard]] constexpr auto North() const -> MapPosition
  {
    return {mRow - 1, mCol};
  }

  /**
   * \brief Returns a map position that is one step south of this map position.
   *
   * \return a position one step to the "south".
   */
  [[nodiscard]] constexpr auto South() const -> MapPosition
  {
    return {mRow + 1, mCol};
  }

  /**
   * \brief Returns a map position that is one step to the west of this map
   * position.
   *
   * a position one step to the "west".
   */
  [[nodiscard]] constexpr auto West() const -> MapPosition
  {
    return {mRow, mCol - 1};
  }

  /**
   * \brief Returns a map position that is one step to the east of this map
   * position.
   *
   * \return a position one step to the "east".
   */
  [[nodiscard]] constexpr auto East() const -> MapPosition
  {
    return {mRow, mCol + 1};
  }

  /**
   * \brief Returns the row index of the map position.
   *
   * \return the associated row index.
   */
  [[nodiscard]] constexpr auto GetRow() const -> int32
  {
    return mRow;
  }

  /**
   * \brief Returns the column index of the map position.
   *
   * \return the associated column index.
   */
  [[nodiscard]] constexpr auto GetColumn() const -> int32
  {
    return mCol;
  }

  /**
   * \brief Returns the raw row index associated with the map position.
   *
   * \details This function is meant to be used when indexing vectors, etc.
   *
   * \return the raw row index.
   */
  [[nodiscard]] constexpr auto GetRowIndex() const noexcept -> usize
  {
    return static_cast<usize>(mRow);
  }

  /**
   * \brief Returns the raw column index associated with the map position.
   *
   * \details This function is meant to be used when indexing vectors, etc.
   *
   * \return the raw column index.
   */
  [[nodiscard]] constexpr auto GetColumnIndex() const noexcept -> usize
  {
    return static_cast<usize>(mCol);
  }

  /**
   * \brief Returns the row index converted to a y-coordinate.
   *
   * \param tileSize the current tile size.
   *
   * \return the row index converted to a y-coordinate.
   */
  [[nodiscard]] constexpr auto RowToY(const int32 tileSize) const noexcept -> int32
  {
    return mRow * tileSize;
  }

  /**
   * \brief Returns the column index converted to an x-coordinate.
   *
   * \param tileSize the current tile size.
   *
   * \return the column index converted to an x-coordinate.
   */
  [[nodiscard]] constexpr auto ColumnToX(const int32 tileSize) const noexcept -> int32
  {
    return mCol * tileSize;
  }

  // clang-format off
  [[nodiscard]] constexpr auto operator<=>(const MapPosition&) const noexcept = default;
  // clang-format on

 private:
  int32 mRow{};
  int32 mCol{};
};

/**
 * \brief Adds two positions and returns the result.
 *
 * \param lhs the left-hand side position.
 * \param rhs the right-hand side position.
 *
 * \return a position that is the sum of two positions.
 */
[[nodiscard]] constexpr auto operator+(const MapPosition& lhs, const MapPosition& rhs)
    -> MapPosition
{
  return MapPosition{lhs.GetRow() + rhs.GetRow(), lhs.GetColumn() + rhs.GetColumn()};
}

/**
 * \brief Subtracts two positions and returns the result.
 *
 * \param lhs the left-hand side position.
 * \param rhs the right-hand side position.
 *
 * \return a position that is the difference between two positions.
 */
[[nodiscard]] constexpr auto operator-(const MapPosition& lhs, const MapPosition& rhs)
    -> MapPosition
{
  return MapPosition{lhs.GetRow() - rhs.GetRow(), lhs.GetColumn() - rhs.GetColumn()};
}

[[nodiscard]] constexpr auto operator*(const MapPosition& lhs, const MapPosition& rhs)
    -> MapPosition
{
  return MapPosition{lhs.GetRow() * rhs.GetRow(), lhs.GetColumn() * rhs.GetColumn()};
}

[[nodiscard]] constexpr auto operator/(const MapPosition& lhs, const MapPosition& rhs)
    -> MapPosition
{
  return MapPosition{lhs.GetRow() / rhs.GetRow(), lhs.GetColumn() / rhs.GetColumn()};
}

/// \} End of group core

}  // namespace Tactile
