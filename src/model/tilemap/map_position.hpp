#pragma once

#include <cstddef>  // size_t

#include "algorithm.hpp"

namespace tactile::model {

/**
 * @class map_position
 *
 * @brief Represents a row- and column-based position in a tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile map_position.hpp
 */
class map_position final
{
 public:
  /**
   * @brief Creates a map position that represents (0, 0).
   *
   * @since 0.1.0
   */
  constexpr map_position() noexcept = default;

  /**
   * @brief Creates a map position.
   *
   * @param row the row value of the map position, clamped to zero if negative.
   * @param col the column value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  constexpr map_position(int row, int col) noexcept
      : m_row{at_least(row, 0)}, m_col{at_least(col, 0)}
  {}

  /**
   * @brief Sets the row coordinate of the map position.
   *
   * @param row the new row value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  constexpr void set_row(int row) noexcept { m_row = at_least(row, 0); }

  /**
   * @brief Sets the column coordinate of the map position.
   *
   * @param col the new column value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  constexpr void set_col(int col) noexcept { m_col = at_least(col, 0); }

  /**
   * @brief Returns a map position that is one step north of this map position.
   *
   * @return a map position that is one step north of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto north() const noexcept -> map_position
  {
    return {m_row - 1, m_col};
  }

  /**
   * @brief Returns a map position that is one step to the east of this map
   * position.
   *
   * @return a map position that is one step to the east of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto east() const noexcept -> map_position
  {
    return {m_row, m_col + 1};
  }

  /**
   * @brief Returns a map position that is one step south of this map position.
   *
   * @return a map position that is one step south of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto south() const noexcept -> map_position
  {
    return {m_row + 1, m_col};
  }

  /**
   * @brief Returns a map position that is one step to the west of this map
   * position.
   *
   * @return a map position that is one step to the west of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto west() const noexcept -> map_position
  {
    return {m_row, m_col - 1};
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
  [[nodiscard]] constexpr auto row() const noexcept -> int { return m_row; }

  /**
   * @brief Returns the column index of the map position.
   *
   * @note The returned value is never negative.
   *
   * @return the column index of the map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto col() const noexcept -> int { return m_col; }

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

  auto operator==(const map_position&) const noexcept -> bool = default;

 private:
  int m_row{};
  int m_col{};
};

}  // namespace tactile::model
