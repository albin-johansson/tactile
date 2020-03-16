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

}  // namespace tactile
