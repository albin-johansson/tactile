#pragma once

#include <vector>

#include "tile.h"
#include "tile_layer.h"

namespace tactile {

/**
 * The <code>TileMap</code> class represents the main tile maps in the
 * Tactile application. Tile maps are composed by multiple tile layers.
 *
 * @see TileLayer
 * @since 0.1.0
 */
class TileMap final {
 public:
  /**
   * Creates a tile map instance with one layer. The amount of rows or
   * columns in the tile map is always at least 1.
   *
   * @param nRows the initial number of rows in the tile map.
   * @param nCols the initial number of columns in the tile map.
   * @since 0.1.0
   */
  TileMap(int nRows, int nCols);

  /**
   * Adds a row to the tile map.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   * @since 0.1.0
   */
  void add_row(TileID id = empty) noexcept;

  /**
   * Adds a column to the tile map.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   * @since 0.1.0
   */
  void add_col(TileID id = empty) noexcept;

  /**
   * Sets the total number of rows in the tile map.
   *
   * @param nRows the new number of rows in the tile map. Clamped to be at
   * least 1.
   * @since 0.1.0
   */
  void set_rows(int nRows) noexcept;

  /**
   * Sets the total number of columns in the tile map.
   *
   * @param nCols the new number of columns in the tile map. Clamped to be at
   * least 1.
   * @since 0.1.0
   */
  void set_cols(int nCols) noexcept;

  /**
   * Returns the total number of rows in the tile map.
   *
   * @return the amount of rows in the tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int rows() const noexcept;

  /**
   * Returns the total number of columns in the tile map.
   *
   * @return the amount of columns in the tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int cols() const noexcept;

  /**
   * Returns the amount of layers present in the tile map.
   *
   * @return the amount of layers present in the tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int get_layer_amount() const noexcept;

 private:
  int m_nRows;
  int m_nCols;
  std::vector<TileLayer> m_layers;
};

}  // namespace tactile
