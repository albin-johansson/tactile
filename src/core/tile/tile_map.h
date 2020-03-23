#pragma once

#include <vector>

#include "maybe.h"
#include "tile_id.h"
#include "tile_layer.h"
#include "tile_map_renderer.h"

class QPainter;

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
  friend class TileMapRenderer;

  /**
   * Creates a tile map instance with one layer. The amount of rows or
   * columns in the tile map is always at least 1.
   *
   * @param nRows the initial number of rows in the tile map.
   * @param nCols the initial number of columns in the tile map.
   * @since 0.1.0
   */
  TileMap(int nRows, int nCols);

  ~TileMap() noexcept;

  /**
   * Renders the tile map.
   *
   * @param painter the painter that will be used.
   * @since 0.1.0
   */
  void draw(QPainter& painter) const noexcept;

  /**
   * Selects the tile layer associated with the specified index. This method has
   * no effect if the supplied index is invalid.
   *
   * @param layer the layer index of the tile layer that will be selected.
   * @since 0.1.0
   */
  void select(int layer) noexcept;

  /**
   * Adds an empty layer to the tile map.
   *
   * @since 0.1.0
   */
  void add_layer() noexcept;

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
   * Removes a row from the tile map. This method has no effect if the tile
   * map only contains one row.
   *
   * @since 0.1.0
   */
  void remove_row() noexcept;

  /**
   * Removes a column from the tile map. This method has no effect if the tile
   * map only contains one column.
   *
   * @since 0.1.0
   */
  void remove_col() noexcept;

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
   * Sets the visibility of a tile layer. This method has no effect if the
   * specified layer index isn't associated with a tile layer.
   *
   * @param layer the index of the layer that will have its visibility changed.
   * @param visibility true if the layer should be visible; false otherwise.
   * @since 0.1.0
   */
  void set_visibility(int layer, bool visibility) noexcept;

  /**
   * Indicates whether or not the layer associated with the specified index is
   * visible. This method returns false if the supplied index isn't associated
   * with a tile layer. Tile layers are visible by default.
   *
   * @param layer the index of the tile layer that will be checked. An invalid
   * index results in the method returning false.
   * @return true if the layer is visible; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool is_visible(int layer) const noexcept;

  /**
   * Returns the amount of layers present in the tile map. Tile maps are created
   * with 1 tile layer.
   *
   * @return the amount of layers present in the tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int get_layer_amount() const noexcept;

  /**
   * Returns the total number of rows in the tile map.
   *
   * @return the amount of rows in the tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int rows() const noexcept { return m_nRows; }

  /**
   * Returns the total number of columns in the tile map.
   *
   * @return the amount of columns in the tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int cols() const noexcept { return m_nCols; }

 private:
  int m_nRows;
  int m_nCols;
  int m_activeLayer;
  std::vector<TileLayer> m_layers;
  TileMapRenderer m_renderer;

  /**
   * Indicates whether or not the specified layer index is associated with a
   * tile layer.
   *
   * @param layer the tile layer index that will be checked.
   * @return true if the supplied layer index is associated with a tile layer;
   * false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool has_layer(int layer) const noexcept;
};

}  // namespace tactile
