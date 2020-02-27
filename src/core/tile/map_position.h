#pragma once

#include <type_traits>

namespace tactile {

/**
 * The <code>MapPosition</code> class represents a row- and column-based
 * position in a tile map.
 *
 * @since 0.1.0
 */
class MapPosition final {
 public:
  /**
   * Creates a map position with the coordinates (0, 0).
   *
   * @since 0.1.0
   */
  MapPosition() noexcept;

  /**
   * Creates a map position.
   *
   * @param row the row value of the map position, clamped to zero if negative.
   * @param col the column value of the map position, clamped to zero if
   * negative.
   * @since 0.1.0
   */
  MapPosition(int row, int col) noexcept;

  /**
   * Sets the row coordinate of the map position.
   *
   * @param row the new row value of the map position, clamped to zero if
   * negative.
   * @since 0.1.0
   */
  void set_row(int row) noexcept;

  /**
   * Sets the column coordinate of the map position.
   *
   * @param row the new column value of the map position, clamped to zero if
   * negative.
   * @since 0.1.0
   */
  void set_col(int col) noexcept;

  /**
   * Returns the row value of the map position. The returned value is never
   * negative.
   *
   * @return the row value of the map position.
   * @since 0.1.0
   */
  [[nodiscard]] int row() const noexcept
  {
    return m_row;
  }

  /**
   * Returns the column value of the map position. The returned value is never
   * negative.
   *
   * @return the column value of the map position.
   * @since 0.1.0
   */
  [[nodiscard]] int col() const noexcept
  {
    return m_col;
  }

 private:
  int m_row;
  int m_col;
};

static_assert(std::is_nothrow_default_constructible_v<MapPosition>);

static_assert(std::is_nothrow_copy_constructible_v<MapPosition>);
static_assert(std::is_nothrow_move_constructible_v<MapPosition>);

static_assert(std::is_nothrow_copy_assignable_v<MapPosition>);
static_assert(std::is_nothrow_move_assignable_v<MapPosition>);

}  // namespace tactile
