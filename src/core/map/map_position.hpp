#pragma once

#include "aliases/col.hpp"
#include "aliases/ints.hpp"
#include "aliases/row.hpp"

namespace Tactile {

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
  constexpr MapPosition(const row_t row, const col_t column)
      : mRow{row.get()}
      , mCol{column.get()}
  {}

  /**
   * \brief Sets the row index associated with the position.
   */
  constexpr void SetRow(const row_t row)
  {
    mRow = row.get();
  }

  /**
   * \brief Sets the column index associated with the position.
   */
  constexpr void SetColumn(const col_t column)
  {
    mCol = column.get();
  }

  /**
   * \brief Creates and position that is offset from this position.
   *
   * \param row the offset that will be added to the row index.
   * \param column the offset that will be added to the column index.
   *
   * \return a position that is offset from this position.
   */
  [[nodiscard]] constexpr auto OffsetBy(const row_t row,
                                        const col_t column) const -> MapPosition
  {
    return {row_t{mRow} + row, col_t{mCol} + column};
  }

  /**
   * \brief Returns a map position that is one step north of this map position.
   *
   * \return a position one step to the "north".
   */
  [[nodiscard]] constexpr auto North() const -> MapPosition
  {
    return {row_t{mRow - 1}, col_t{mCol}};
  }

  /**
   * \brief Returns a map position that is one step south of this map position.
   *
   * \return a position one step to the "south".
   */
  [[nodiscard]] constexpr auto South() const -> MapPosition
  {
    return {row_t{mRow + 1}, col_t{mCol}};
  }

  /**
   * \brief Returns a map position that is one step to the west of this map
   * position.
   *
   * a position one step to the "west".
   */
  [[nodiscard]] constexpr auto West() const -> MapPosition
  {
    return {row_t{mRow}, col_t{mCol - 1}};
  }

  /**
   * \brief Returns a map position that is one step to the east of this map
   * position.
   *
   * \return a position one step to the "east".
   */
  [[nodiscard]] constexpr auto East() const -> MapPosition
  {
    return {row_t{mRow}, col_t{mCol + 1}};
  }

  /**
   * \brief Returns the row index of the map position.
   *
   * \return the associated row index.
   */
  [[nodiscard]] constexpr auto GetRow() const -> row_t
  {
    return row_t{mRow};
  }

  /**
   * \brief Returns the column index of the map position.
   *
   * \return the associated column index.
   */
  [[nodiscard]] constexpr auto GetColumn() const -> col_t
  {
    return col_t{mCol};
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
  [[nodiscard]] constexpr auto RowToY(const int tileSize) const noexcept -> int
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
  [[nodiscard]] constexpr auto ColumnToX(const int tileSize) const noexcept
      -> int
  {
    return mCol * tileSize;
  }

  // clang-format off
  [[nodiscard]] constexpr auto operator<=>(const MapPosition&) const noexcept = default;
  // clang-format on

 private:
  // We don't use strong types here in order to enable the spaceship operator
  row_t::value_type mRow{};
  col_t::value_type mCol{};
};

/**
 * \brief Adds two positions and returns the result.
 *
 * \param lhs the left-hand side position.
 * \param rhs the right-hand side position.
 *
 * \return a position that is the sum of two positions.
 */
[[nodiscard]] constexpr auto operator+(const MapPosition& lhs,
                                       const MapPosition& rhs) -> MapPosition
{
  return MapPosition{lhs.GetRow() + rhs.GetRow(),
                     lhs.GetColumn() + rhs.GetColumn()};
}

/**
 * \brief Subtracts two positions and returns the result.
 *
 * \param lhs the left-hand side position.
 * \param rhs the right-hand side position.
 *
 * \return a position that is the difference between two positions.
 */
[[nodiscard]] constexpr auto operator-(const MapPosition& lhs,
                                       const MapPosition& rhs) -> MapPosition
{
  return MapPosition{lhs.GetRow() - rhs.GetRow(),
                     lhs.GetColumn() - rhs.GetColumn()};
}

}  // namespace Tactile
