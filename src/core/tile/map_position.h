#pragma once

#include <type_traits>

namespace tactile {

/**
 * The <code>MapPosition</code> class represents a row- and column-based
 * position in a tilemap.
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
   * Returns a map position that is one step north of this map position.
   *
   * @return a map position that is one step north of this map position.
   * @since 0.1.0
   */
  [[nodiscard]] MapPosition north() const noexcept;

  /**
   * Returns a map position that is one step to the east of this map position.
   *
   * @return a map position that is one step to the east of this map position.
   * @since 0.1.0
   */
  [[nodiscard]] MapPosition east() const noexcept;

  /**
   * Returns a map position that is one step south of this map position.
   *
   * @return a map position that is one step south of this map position.
   * @since 0.1.0
   */
  [[nodiscard]] MapPosition south() const noexcept;

  /**
   * Returns a map position that is one step to the west of this map position.
   *
   * @return a map position that is one step to the west of this map position.
   * @since 0.1.0
   */
  [[nodiscard]] MapPosition west() const noexcept;

  /**
   * Returns the row value of the map position. The returned value is never
   * negative.
   *
   * @return the row value of the map position.
   * @since 0.1.0
   */
  [[nodiscard]] int row() const noexcept { return m_row; }

  /**
   * Returns the column value of the map position. The returned value is never
   * negative.
   *
   * @return the column value of the map position.
   * @since 0.1.0
   */
  [[nodiscard]] int col() const noexcept { return m_col; }

 private:
  int m_row;
  int m_col;
};

/**
 * Indicates whether or not two map positions are the same.
 *
 * @param lhs the left-hand side map position.
 * @param rhs the right-hand side map position.
 * @return true if the map positions have the same coordinates; false otherwise.
 * @since 0.1.0
 */
[[nodiscard]] bool operator==(const MapPosition& lhs,
                              const MapPosition& rhs) noexcept;

/**
 * Indicates whether or not two map positions aren't the same.
 *
 * @param lhs the left-hand side map position.
 * @param rhs the right-hand side map position.
 * @return true if the map positions don't have the same coordinates; false
 * otherwise.
 * @since 0.1.0
 */
[[nodiscard]] bool operator!=(const MapPosition& lhs,
                              const MapPosition& rhs) noexcept;

static_assert(std::is_final_v<MapPosition>);

static_assert(std::is_nothrow_default_constructible_v<MapPosition>);
static_assert(std::is_nothrow_destructible_v<MapPosition>);

static_assert(std::is_nothrow_copy_constructible_v<MapPosition>);
static_assert(std::is_nothrow_move_constructible_v<MapPosition>);

static_assert(std::is_nothrow_copy_assignable_v<MapPosition>);
static_assert(std::is_nothrow_move_assignable_v<MapPosition>);

}  // namespace tactile
