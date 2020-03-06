#pragma once

//#include "image.h"
#include "tile_id.h"

namespace tactile {

/**
 * The <code>TileSet</code> class represents a set of tile identifiers
 * associated with different tile sheets.
 *
 * @since 0.1.0
 */
class TileSet final {
 public:
  class Cell;

  /**
   * Selects the tile at the specified coordinates.
   *
   * @param x the x-coordinate of the mouse in the active tile sheet.
   * @param y the y-coordinate of the mouse in the active tile sheet.
   * @since 0.1.0
   */
  void select(int x, int y) noexcept;

  /**
   * Clears any currently selected tiles.
   *
   * @since 0.1.0
   */
  void clear_selection() noexcept;

  [[nodiscard]] int get_tile_amount() const noexcept;

  [[nodiscard]] int width() const noexcept;

  [[nodiscard]] int height() const noexcept;

 // [[nodiscard]] TileID first_id

//  [[nodiscard]] const Image& sheet() const noexcept;

 private:

};

class TileSet::Cell final {
 public:
  Cell(int x, int y, int index) noexcept;

  [[nodiscard]] int x() const noexcept
  {
    return m_x;
  }

  [[nodiscard]] int y() const noexcept
  {
    return m_y;
  }

  [[nodiscard]] int index() const noexcept
  {
    return m_index;
  }

 private:
  int m_x;
  int m_y;
  int m_index;
};

}  // namespace tactile
