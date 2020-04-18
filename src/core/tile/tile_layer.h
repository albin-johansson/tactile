#pragma once

#include <memory>
#include <vector>

#include "maybe.h"
#include "tile_id.h"

namespace tactile {

class MapPosition;

/**
 * The <code>TileLayer</code> class represents a layer of tiles in a tilemap.
 *
 * @since 0.1.0
 */
class TileLayer final {
 public:
  friend class TilemapRenderer;

  /**
   * Creates a tile layer with the specified dimensions. All of the tiles in
   * the created layer are initialized with the value of the
   * <code>empty</code> constant.
   *
   * @param nRows the initial number of rows in the layer; clamped to 1 if
   * the supplied value is less than 1.
   * @param nCols the initial number of columns in the layer; clamped to 1 if
   * the supplied value is less than 1.
   * @since 0.1.0
   */
  TileLayer(int nRows, int nCols) noexcept;

  /**
   * Runs a flood fill in the tile layer. This method has no effect if the
   * flood fill cannot be performed.
   *
   * @param position the starting position of the flood fill.
   * @param target the tile type that will be replaced.
   * @param replacement the tile type that will replace the target type.
   * @since 0.1.0
   */
  void flood(const MapPosition& position,
             TileID target,
             TileID replacement) noexcept;

  /**
   * Adds a row to the tile layer.
   *
   * @param id the tile ID of all of the new cells in the added row.
   * @since 0.1.0
   */
  void add_row(TileID id) noexcept;

  /**
   * Adds a column to the tile layer.
   *
   * @param id the tile ID of all of the new cells in the added column.
   * @since 0.1.0
   */
  void add_col(TileID id) noexcept;

  /**
   * Removes a row from the tile layer. Note that a tile layer will always
   * have at least one row, so this method has no effect if the tile layer
   * has exactly one row.
   *
   * @since 0.1.0
   */
  void remove_row() noexcept;

  /**
   * Removes a column from the tile layer. Note that a tile layer will always
   * have at least one column, so this method has no effect if the tile layer
   * has exactly one column.
   *
   * @since 0.1.0
   */
  void remove_col() noexcept;

  /**
   * Sets the tile ID of a tile in the tile layer. This method has no effect
   * if the supplied position is out-of-bounds.
   *
   * @param position the position of the tile that will be changed.
   * @param id the new tile ID.
   * @since 0.1.0
   */
  void set_tile(const MapPosition& position, TileID id) noexcept;

  /**
   * Sets whether or not the tile layer is visible.
   *
   * @param visible true if the tile layer should be visible; false otherwise.
   * @since 0.1.0
   */
  void set_visible(bool visible) noexcept;

  /**
   * Returns the number of rows in the tile layer. A tile layer always
   * contains at least one row.
   *
   * @return the number of rows in the tile layer.
   * @since 0.1.0
   */
  [[nodiscard]] int rows() const noexcept;

  /**
   * Returns the number of columns in the tile layer. A tile layer always
   * contains at least one column.
   *
   * @return the number of columns in the tile layer.
   * @since 0.1.0
   */
  [[nodiscard]] int cols() const noexcept;

  /**
   * Returns the ID of the tile at the specified position. This method
   * returns <code>nothing</code> if the supplied position is out-of-bounds.
   *
   * @param position the position to obtain the tile ID of.
   * @return the ID of the tile at the specified position;
   * <code>nothing</code> if the position is out-of-bounds.
   * @since 0.1.0
   */
  [[nodiscard]] Maybe<TileID> tile_at(const MapPosition& position) const
      noexcept;

  /**
   * Indicates whether or not the specified position is in bounds of the tile
   * layer.
   *
   * @param position the position that will be checked.
   * @return true if the position is in bounds; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool in_bounds(const MapPosition& position) const noexcept;

  /**
   * Indicates whether or not the tile layer is visible. Tile layers are visible
   * by default.
   *
   * @return true if the tile layer is visible; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool visible() const noexcept { return m_visible; }

 private:
  using TileRow = std::vector<TileID>;
  using TileMatrix = std::vector<TileRow>;

  TileMatrix m_tiles;
  bool m_visible;
};

}  // namespace tactile
