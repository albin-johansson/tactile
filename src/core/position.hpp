#pragma once

#include <cstddef>  // size_t
#include <utility>  // pair

#include "algorithm.hpp"
#include "nenya.hpp"

namespace tactile {
namespace detail {

struct row_tag final
{};

struct col_tag final
{};

}  // namespace detail

using row_t = nenya::mirror_type<int, detail::row_tag>;
using col_t = nenya::mirror_type<int, detail::col_tag>;

/**
 * \brief Creates a row value.
 *
 * \param value the row index.
 *
 * \return a row value.
 *
 * \since 0.1.0
 */
[[nodiscard]] constexpr auto operator"" _row(unsigned long long value) noexcept
    -> row_t
{
  return row_t{static_cast<int>(value)};
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
[[nodiscard]] constexpr auto operator"" _col(unsigned long long value) noexcept
    -> col_t
{
  return col_t{static_cast<int>(value)};
}

namespace core {

/**
 * \class position
 *
 * \brief Represents a row- and column-based position.
 *
 * \since 0.1.0
 *
 * \headerfile position.hpp
 */
class position final
{
 public:
  /**
   * \brief Creates a map position that represents (0, 0).
   *
   * \since 0.1.0
   */
  constexpr position() noexcept = default;

  /**
   * \brief Creates a map position.
   *
   * \param row the row value of the map position, clamped to zero if negative.
   * \param col the column value of the map position, clamped to zero if
   * negative.
   *
   * \since 0.1.0
   */
  constexpr position(const row_t row, const col_t col) noexcept
      : m_row{at_least(row.get(), 0)}
      , m_col{at_least(col.get(), 0)}
  {}

  /**
   * \brief Sets the row coordinate of the map position.
   *
   * \param row the new row value of the map position, clamped to zero if
   * negative.
   *
   * \since 0.1.0
   */
  constexpr void set_row(const row_t row) noexcept
  {
    m_row = at_least(row.get(), 0);
  }

  /**
   * \brief Sets the column coordinate of the map position.
   *
   * \param col the new column value of the map position, clamped to zero if
   * negative.
   *
   * \since 0.1.0
   */
  constexpr void set_col(const col_t col) noexcept
  {
    m_col = at_least(col.get(), 0);
  }

  /**
   * \brief Creates and returns a position that is offset from this position.
   *
   * \param row the offset that will be added to the row index.
   * \param col the offset that will be added to the column index.
   *
   * \return a position that is offset from this position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto offset_by(const row_t row,
                                         const col_t col) const noexcept
      -> position
  {
    return {row_t{m_row} + row, col_t{m_col} + col};
  }

  /**
   * \brief Returns a map position that is one step north of this map position.
   *
   * \return a map position that is one step north of this map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto north() const noexcept -> position
  {
    return {row_t{m_row - 1}, col_t{m_col}};
  }

  /**
   * \brief Returns a map position that is one step to the east of this map
   * position.
   *
   * \return a map position that is one step to the east of this map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto east() const noexcept -> position
  {
    return {row_t{m_row}, col_t{m_col + 1}};
  }

  /**
   * \brief Returns a map position that is one step south of this map position.
   *
   * \return a map position that is one step south of this map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto south() const noexcept -> position
  {
    return {row_t{m_row + 1}, col_t{m_col}};
  }

  /**
   * \brief Returns a map position that is one step to the west of this map
   * position.
   *
   * \return a map position that is one step to the west of this map position.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr auto west() const noexcept -> position
  {
    return {row_t{m_row}, col_t{m_col - 1}};
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
  [[nodiscard]] constexpr auto unpack() const noexcept
      -> std::pair<row_t, col_t>
  {
    return {row(), col()};
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
  [[nodiscard]] constexpr auto row() const noexcept -> row_t
  {
    return row_t{m_row};
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
  [[nodiscard]] constexpr auto col() const noexcept -> col_t
  {
    return col_t{m_col};
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
  [[nodiscard]] constexpr auto row_index() const noexcept -> std::size_t
  {
    return static_cast<std::size_t>(m_row);
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
  [[nodiscard]] constexpr auto col_index() const noexcept -> std::size_t
  {
    return static_cast<std::size_t>(m_col);
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
  [[nodiscard]] constexpr auto col_to_x(const int tileSize) const noexcept
      -> int
  {
    return m_col * tileSize;
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
  [[nodiscard]] constexpr auto row_to_y(const int tileSize) const noexcept
      -> int
  {
    return m_row * tileSize;
  }

  [[nodiscard]] auto operator<=>(const position&) const noexcept = default;

 private:
  // not using the row and col types in order to default the spaceship operator
  int m_row{};
  int m_col{};
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
[[nodiscard]] constexpr auto operator+(const position& lhs,
                                       const position& rhs) noexcept -> position
{
  return position{lhs.row() + rhs.row(), lhs.col() + rhs.col()};
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
[[nodiscard]] constexpr auto operator-(const position& lhs,
                                       const position& rhs) noexcept -> position
{
  return position{lhs.row() - rhs.row(), lhs.col() - rhs.col()};
}

}  // namespace core
}  // namespace tactile
