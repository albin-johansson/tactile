#pragma once

#include "type_utils.hpp"

namespace tactile {

/**
 * @class map_position
 *
 * @brief Represents a row- and column-based position in a tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile map_position.hpp
 */
class map_position final {
 public:
  map_position() noexcept = default;

  /**
   * @brief Creates a map position.
   *
   * @param row the row value of the map position, clamped to zero if negative.
   * @param col the column value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  map_position(int row, int col) noexcept;

  /**
   * @brief Sets the row coordinate of the map position.
   *
   * @param row the new row value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  void set_row(int row) noexcept;

  /**
   * @brief Sets the column coordinate of the map position.
   *
   * @param row the new column value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  void set_col(int col) noexcept;

  /**
   * @brief Returns a map position that is one step north of this map position.
   *
   * @return a map position that is one step north of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto north() const noexcept -> map_position;

  /**
   * @brief Returns a map position that is one step to the east of this map
   * position.
   *
   * @return a map position that is one step to the east of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto east() const noexcept -> map_position;

  /**
   * @brief Returns a map position that is one step south of this map position.
   *
   * @return a map position that is one step south of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto south() const noexcept -> map_position;

  /**
   * @brief Returns a map position that is one step to the west of this map
   * position.
   *
   * @return a map position that is one step to the west of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto west() const noexcept -> map_position;

  /**
   * @brief Returns the row value of the map position.
   *
   * @note The returned value is never negative.
   *
   * @return the row value of the map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto row() const noexcept -> int { return m_row; }

  /**
   * @brief Returns the column value of the map position.
   *
   * @note The returned value is never negative.
   *
   * @return the column value of the map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto col() const noexcept -> int { return m_col; }

 private:
  int m_row{};
  int m_col{};
};

/**
 * @brief Indicates whether or not two map positions are the same.
 *
 * @param lhs the left-hand side map position.
 * @param rhs the right-hand side map position.
 *
 * @return `true` if the map positions have the same coordinates; `false`
 * otherwise.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto operator==(const map_position& lhs,
                              const map_position& rhs) noexcept -> bool;

/**
 * @brief Indicates whether or not two map positions aren't the same.
 *
 * @param lhs the left-hand side map position.
 * @param rhs the right-hand side map position.
 *
 * @return `true` if the map positions don't have the same coordinates; `false`
 * otherwise.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto operator!=(const map_position& lhs,
                              const map_position& rhs) noexcept -> bool;

static_assert(std::is_nothrow_default_constructible_v<map_position>);
static_assert(std::is_nothrow_destructible_v<map_position>);

}  // namespace tactile
