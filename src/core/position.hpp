#pragma once

#include <nenya.hpp>  // strong_type
#include <utility>    // pair

#include "algorithm.hpp"
#include "ints.hpp"

namespace tactile {

namespace tags {
struct row_tag;
struct col_tag;
}  // namespace tags

using row_t = nenya::strong_type<int, tags::row_tag>;
using col_t = nenya::strong_type<int, tags::col_tag>;

/**
 * \brief Creates a row value.
 *
 * \param value the row index.
 *
 * \return a row value.
 *
 * \since 0.1.0
 */
[[nodiscard]] constexpr auto operator"" _row(const ulonglong value) noexcept
    -> row_t
{
  return row_t{static_cast<row_t::value_type>(value)};
}

/**
 * \brief Creates a column value.
 *
 * \param value the column index.
 *
 * \return a column value.
 *
 * \since 0.1.0
 */
[[nodiscard]] constexpr auto operator"" _col(const ulonglong value) noexcept
    -> col_t
{
  return col_t{static_cast<col_t::value_type>(value)};
}

namespace core {

/**
 * \class Position
 *
 * \brief Represents a row- and column-based position.
 *
 * \since 0.1.0
 *
 * \headerfile position.hpp
 */
class Position final
{
 public:
  /**
   * \brief Creates a map position that represents (0, 0).
   *
   * \since 0.1.0
   */
  constexpr Position() noexcept = default;

  /**
   * \brief Creates a map position.
   *
   * \param row the row value of the map position, clamped to zero if negative.
   * \param col the column value of the map position, clamped to zero if
   * negative.
   *
   * \since 0.1.0
   */
  constexpr Position(const row_t row, const col_t col) noexcept
      : mRow{AtLeast(row.get(), 0)}
      , mColumn{AtLeast(col.get(), 0)}
  {}

  /**
   * \brief Sets the row coordinate of the map position.
   *
   * \param row the new row value of the map position, clamped to zero if
   * negative.
   *
   * \since 0.1.0
   */
  constexpr void SetRow(const row_t row) noexcept
  {
    mRow = AtLeast(row.get(), 0);
  }

  /**
   * \brief Sets the column coordinate of the map position.
   *
   * \param column the new column value of the map position, clamped to zero if
   * negative.
   *
   * \since 0.1.0
   */
  constexpr void SetColumn(const col_t column) noexcept
  {
    mColumn = AtLeast(column.get(), 0);
  }

  /**
   * \brief Creates and returns a position that is offset from this position.
   *
   * \param row the offset that will be added to the row index.
   * \param column the offset that will be added to the column index.
   *
   * \return a position that is offset from this position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto OffsetBy(const row_t row,
                                        const col_t column) const noexcept
      -> Position
  {
    return {row_t{mRow} + row, col_t{mColumn} + column};
  }

  /**
   * \brief Returns a map position that is one step north of this map position.
   *
   * \return a map position that is one step north of this map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto North() const noexcept -> Position
  {
    return {row_t{mRow - 1}, col_t{mColumn}};
  }

  /**
   * \brief Returns a map position that is one step to the east of this map
   * position.
   *
   * \return a map position that is one step to the east of this map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto East() const noexcept -> Position
  {
    return {row_t{mRow}, col_t{mColumn + 1}};
  }

  /**
   * \brief Returns a map position that is one step south of this map position.
   *
   * \return a map position that is one step south of this map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto South() const noexcept -> Position
  {
    return {row_t{mRow + 1}, col_t{mColumn}};
  }

  /**
   * \brief Returns a map position that is one step to the west of this map
   * position.
   *
   * \return a map position that is one step to the west of this map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto West() const noexcept -> Position
  {
    return {row_t{mRow}, col_t{mColumn - 1}};
  }

  /**
   * \brief Returns a pair that holds the row and column values.
   *
   * \details This function is meant to be used with structured bindings, as a
   * convenient way to deconstruct the position.
   *
   * \return a pair that holds the coordinates of the position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto Unpack() const noexcept
      -> std::pair<row_t, col_t>
  {
    return {Row(), Column()};
  }

  /**
   * \brief Returns the row index of the map position.
   *
   * \note The returned value is never negative.
   *
   * \return the row index of the map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto Row() const noexcept -> row_t
  {
    return row_t{mRow};
  }

  /**
   * \brief Returns the column index of the map position.
   *
   * \note The returned value is never negative.
   *
   * \return the column index of the map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto Column() const noexcept -> col_t
  {
    return col_t{mColumn};
  }

  /**
   * \brief Returns the row index associated with the map position.
   *
   * \details This function is meant to be used when indexing vectors, etc.
   *
   * \return the row index.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto RowIndex() const noexcept -> usize
  {
    return static_cast<usize>(mRow);
  }

  /**
   * \brief Returns the column index associated with the map position.
   *
   * \details This function is meant to be used when indexing vectors, etc.
   *
   * \return the column index.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto ColumnIndex() const noexcept -> usize
  {
    return static_cast<usize>(mColumn);
  }

  /**
   * \brief Returns the column index converted to an x-coordinate.
   *
   * \param tileSize the current tile size.
   *
   * \return the column index converted to an x-coordinate.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto ColumnToX(const int tileSize) const noexcept
      -> int
  {
    return mColumn * tileSize;
  }

  /**
   * \brief Returns the row index converted to a y-coordinate.
   *
   * \param tileSize the current tile size.
   *
   * \return the row index converted to a y-coordinate.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto RowToY(const int tileSize) const noexcept -> int
  {
    return mRow * tileSize;
  }

  [[nodiscard]] auto operator<=>(const Position&) const noexcept = default;

 private:
  // Not using the row and col types in order to default the spaceship operator
  int mRow{};
  int mColumn{};
};

/**
 * \brief Adds two positions and returns the result.
 *
 * \param lhs the left-hand side position.
 * \param rhs the right-hand side position.
 *
 * \return a position that is the sum of two positions.
 *
 * \since 0.1.0
 */
[[nodiscard]] constexpr auto operator+(const Position& lhs,
                                       const Position& rhs) noexcept -> Position
{
  return Position{lhs.Row() + rhs.Row(), lhs.Column() + rhs.Column()};
}

/**
 * \brief Subtracts two positions and returns the result.
 *
 * \param lhs the left-hand side position.
 * \param rhs the right-hand side position.
 *
 * \return a position that is the difference between two positions.
 *
 * \since 0.1.0
 */
[[nodiscard]] constexpr auto operator-(const Position& lhs,
                                       const Position& rhs) noexcept -> Position
{
  return Position{lhs.Row() - rhs.Row(), lhs.Column() - rhs.Column()};
}

}  // namespace core
}  // namespace tactile
