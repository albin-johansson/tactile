#pragma once

#include <cstddef>  // size_t

#include "algorithm.hpp"
#include "nenya.hpp"

namespace tactile::core {
namespace detail {

struct row_tag final
{};

struct col_tag final
{};

}  // namespace detail

using row = nenya::mirror_type<int, detail::row_tag>;
using col = nenya::mirror_type<int, detail::col_tag>;

[[nodiscard]] constexpr auto operator"" _row(unsigned long long value) noexcept
    -> row
{
  return row{static_cast<int>(value)};
}

[[nodiscard]] constexpr auto operator"" _col(unsigned long long value) noexcept
    -> col
{
  return col{static_cast<int>(value)};
}

/**
 * @class position
 *
 * @brief Represents a row- and column-based position.
 *
 * @since 0.1.0
 *
 * @headerfile position.hpp
 */
class position final
{
 public:
  /**
   * @brief Creates a map position that represents (0, 0).
   *
   * @since 0.1.0
   */
  constexpr position() noexcept = default;

  /**
   * @brief Creates a map position.
   *
   * @param row the row value of the map position, clamped to zero if negative.
   * @param col the column value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  constexpr position(row row, col col) noexcept
      : m_row{at_least(row.get(), 0)}, m_col{at_least(col.get(), 0)}
  {}

  /**
   * @brief Sets the row coordinate of the map position.
   *
   * @param row the new row value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  constexpr void set_row(row row) noexcept
  {
    m_row = at_least(row.get(), 0);
  }

  /**
   * @brief Sets the column coordinate of the map position.
   *
   * @param col the new column value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  constexpr void set_col(col col) noexcept
  {
    m_col = at_least(col.get(), 0);
  }

  /**
   * @brief Returns a map position that is one step north of this map position.
   *
   * @return a map position that is one step north of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto north() const noexcept -> position
  {
    return {row{m_row - 1}, col{m_col}};
  }

  /**
   * @brief Returns a map position that is one step to the east of this map
   * position.
   *
   * @return a map position that is one step to the east of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto east() const noexcept -> position
  {
    return {row{m_row}, col{m_col + 1}};
  }

  /**
   * @brief Returns a map position that is one step south of this map position.
   *
   * @return a map position that is one step south of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto south() const noexcept -> position
  {
    return {row{m_row + 1}, col{m_col}};
  }

  /**
   * @brief Returns a map position that is one step to the west of this map
   * position.
   *
   * @return a map position that is one step to the west of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto west() const noexcept -> position
  {
    return {row{m_row}, col{m_col - 1}};
  }

  /**
   * @brief Returns the row index of the map position.
   *
   * @note The returned value is never negative.
   *
   * @return the row index of the map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto get_row() const noexcept -> row
  {
    return row{m_row};
  }

  /**
   * @brief Returns the column index of the map position.
   *
   * @note The returned value is never negative.
   *
   * @return the column index of the map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto get_col() const noexcept -> col
  {
    return col{m_col};
  }

  /**
   * @brief Returns the row index associated with the map position.
   *
   * @details This function is meant to be used when indexing vectors, etc.
   *
   * @return the row index.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto urow() const noexcept -> std::size_t
  {
    return static_cast<std::size_t>(m_row);
  }

  /**
   * @brief Returns the column index associated with the map position.
   *
   * @details This function is meant to be used when indexing vectors, etc.
   *
   * @return the column index.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto ucol() const noexcept -> std::size_t
  {
    return static_cast<std::size_t>(m_col);
  }

  [[nodiscard]] auto operator<=>(const position&) const noexcept = default;

 private:
  // not using the row and col types in order to default spaceship operator
  int m_row{};
  int m_col{};
};

}  // namespace tactile::core
