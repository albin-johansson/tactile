/**
 * @file tile_layer.hpp
 *
 * @brief Provides the `tile_layer` class.
 *
 * @since 0.1.0
 *
 * @author Albin Johansson
 *
 * @date 2020
 *
 * @copyright GPL-3.0
 */

#pragma once

#include <optional>
#include <vector>

#include "map_position.hpp"
#include "tactile_types.hpp"

namespace tactile::model {

/**
 * @class tile_layer
 *
 * @brief Represents a layer of tiles in a tilemap.
 *
 * @since 0.1.0
 *
 * @see `tilemap`
 * @see `tilemap_renderer`
 *
 * @headerfile tile_layer.hpp
 */
class tile_layer final
{
 public:
  /**
   * @brief Creates a tile layer with the specified dimensions.
   *
   * @details All of the tiles in the created layer are initialized with the
   * value of the `empty` tile ID.
   *
   * @param nRows the initial number of rows in the layer; clamped to 1 if
   * the supplied value is less than 1.
   * @param nCols the initial number of columns in the layer; clamped to 1 if
   * the supplied value is less than 1.
   *
   * @since 0.1.0
   */
  tile_layer(int nRows, int nCols);

  /**
   * @brief Runs a flood fill in the tile layer.
   *
   * @note This method has no effect if the flood fill cannot be performed.
   *
   * @param position the starting position of the flood fill.
   * @param target the tile type that will be replaced.
   * @param replacement the tile type that will replace the target type.
   *
   * @since 0.1.0
   */
  void flood(const map_position& position,
             tile_id target,
             tile_id replacement) noexcept;

  /**
   * @brief Adds a row to the tile layer.
   *
   * @param id the tile ID of all of the new cells in the added row.
   *
   * @since 0.1.0
   */
  void add_row(tile_id id);

  /**
   * @brief Adds a column to the tile layer.
   *
   * @param id the tile ID of all of the new cells in the added column.
   *
   * @since 0.1.0
   */
  void add_col(tile_id id);

  /**
   * @brief Removes a row from the tile layer.
   *
   * @note The tile layer will always have at least one row, so this method
   * has no effect if the tile layer has exactly one row.
   *
   * @since 0.1.0
   */
  void remove_row() noexcept;

  /**
   * @brief Removes a column from the tile layer.
   *
   * @note The tile layer will always have at least one column, so this
   * method has no effect if the tile layer has exactly one column.
   *
   * @since 0.1.0
   */
  void remove_col() noexcept;

  /**
   * @brief Sets the total number of rows in the layer.
   *
   * @pre `nRows` must be greater than or equal to 1.
   *
   * @param nRows the new number of rows in the layer.
   *
   * @since 0.1.0
   */
  void set_rows(int nRows);

  /**
   * @brief Sets the total number of columns in the layer.
   *
   * @pre `nCols` must be greater than or equal to 1.
   *
   * @param nCols the new number of columns in the layer.
   *
   * @since 0.1.0
   */
  void set_cols(int nCols);

  /**
   * @brief Sets the tile ID of a tile in the tile layer.
   *
   * @note This method has no effect if the supplied position is out-of-bounds.
   *
   * @param position the position of the tile that will be changed.
   * @param id the new tile ID.
   *
   * @since 0.1.0
   */
  void set_tile(const map_position& position, tile_id id) noexcept;

  /**
   * @brief Sets whether or not the tile layer is visible.
   *
   * @param visible `true` if the tile layer should be visible; `false`
   * otherwise.
   *
   * @since 0.1.0
   */
  void set_visible(bool visible) noexcept;

  /**
   * @brief Returns the number of rows in the tile layer.
   *
   * @note A tile layer always contains at least one row.
   *
   * @return the number of rows in the tile layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto rows() const noexcept -> int;

  /**
   * @brief Returns the number of columns in the tile layer.
   *
   * @note A tile layer always contains at least one column.
   *
   * @return the number of columns in the tile layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto cols() const noexcept -> int;

  /**
   * @brief Returns the ID of the tile at the specified position.
   *
   * @note This method returns `std::nullopt` if the supplied position
   * is out-of-bounds.
   *
   * @param position the position to obtain the tile ID of.
   *
   * @return the ID of the tile at the specified position; `std::nullopt` if the
   * position is out-of-bounds.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_at(const map_position& position) const noexcept
      -> std::optional<tile_id>;

  /**
   * @brief Indicates whether or not the specified position is in bounds of the
   * tile layer.
   *
   * @param position the position that will be checked.
   *
   * @return `true` if the position is in bounds; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto in_bounds(const map_position& position) const noexcept
      -> bool;

  /**
   * @brief Indicates whether or not the tile layer is visible.
   *
   * @note Tile layers are visible by default.
   *
   * @return `true` if the tile layer is visible; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto visible() const noexcept -> bool;

 private:
  using tile_row = std::vector<tile_id>;
  using tile_matrix = std::vector<tile_row>;

  tile_matrix m_tiles;
  bool m_visible{true};
};

}  // namespace tactile::model
