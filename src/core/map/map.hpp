#pragma once

#include <concepts>  // invocable
#include <optional>  // optional
#include <vector>    // vector

#include "layer.hpp"
#include "tile_size.hpp"
#include "types.hpp"

namespace tactile::core {

/**
 * @class map
 *
 * @brief Represents the main tilemaps in the application.
 *
 * @details Tilemaps are composed by multiple tile layers.
 *
 * @see `layer`
 *
 * @since 0.1.0
 *
 * @headerfile map.hpp
 */
class map final
{
 public:
  using const_iterator = typename std::vector<layer>::const_iterator;

  /**
   * @brief Creates a map with one layer.
   *
   * @details The amount of rows and columns in the map is always at
   * least 1.
   *
   * @param nRows the initial number of rows in the map.
   * @param nCols the initial number of columns in the map.
   *
   * @since 0.1.0
   */
  map(row_t nRows, col_t nCols);

  template <std::invocable<layer_id, const layer&> T>
  void each_layer(T&& callable) const
  {
    layer_id id{0};
    for (const auto& layer : m_layers) {
      callable(id, layer);
      ++id;
    }
  }

  /**
   * @brief Runs a flood fill in the currently active layer.
   *
   * @param origin the starting position of the flood fill.
   * @param replacement the tile type that will be used as the replacement.
   * @param[out] positions the vector that the affected positions will be added
   * to.
   *
   * @since 0.1.0
   */
  void flood(const position& origin,
             tile_id replacement,
             std::vector<position>& positions);

  /**
   * @brief Sets the value of the tile at the specified position in the
   * currently active layer.
   *
   * @param pos the position of the tile that will be modified.
   * @param id the new value of the tile.
   *
   * @since 0.1.0
   */
  void set_tile(const position& pos, tile_id id);

  /**
   * @brief Removes all occurrences of the specified ID in all layers.
   *
   * @param id the ID that will be replaced with `empty`.
   *
   * @since 0.1.0
   */
  void remove_all(tile_id id);

  /**
   * @brief Selects the tile layer associated with the specified index.
   *
   * @note This method has no effect if the supplied index is invalid.
   *
   * @param layer the layer index of the tile layer that will be selected.
   *
   * @since 0.1.0
   */
  void select_layer(layer_id layer) noexcept;

  /**
   * @brief Adds an empty layer to the map.
   *
   * @since 0.1.0
   */
  void add_layer();

  /**
   * @brief Adds a row to the map.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   *
   * @since 0.1.0
   */
  void add_row(tile_id id = empty);

  /**
   * @brief Adds a column to the map.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   *
   * @since 0.1.0
   */
  void add_col(tile_id id = empty);

  /**
   * @brief Removes a row from the map.
   *
   * @note This method has no effect if the tile map only contains one row.
   *
   * @since 0.1.0
   */
  void remove_row() noexcept;

  /**
   * @brief Removes a column from the map.
   *
   * @note This method has no effect if the tile map only contains one column.
   *
   * @since 0.1.0
   */
  void remove_col() noexcept;

  /**
   * @copydoc tile_size::increase()
   */
  void increase_tile_size()
  {
    m_tileSize.increase();
  }

  /**
   * @copydoc tile_size::decrease()
   */
  void decrease_tile_size() noexcept
  {
    m_tileSize.decrease();
  }

  /**
   * @copydoc tile_size::reset()
   */
  void reset_tile_size() noexcept
  {
    m_tileSize.reset();
  }

  /**
   * @brief Sets the total number of rows in the map.
   *
   * @param nRows the new number of rows in the map. Clamped to be at
   * least 1.
   *
   * @since 0.1.0
   */
  void set_rows(row_t nRows);

  /**
   * @brief Sets the total number of columns in the map.
   *
   * @param nCols the new number of columns in the map. Clamped to be at
   * least 1.
   *
   * @since 0.1.0
   */
  void set_cols(col_t nCols);

  /**
   * @brief Sets the visibility of a tile layer.
   *
   * @note This method has no effect if the specified layer index isn't
   * associated with a tile layer.
   *
   * @param layer the index of the layer that will have its visibility changed.
   * @param visibility true if the layer should be visible; false otherwise.
   *
   * @since 0.1.0
   */
  void set_visibility(layer_id layer, bool visibility) noexcept;

  /**
   * @brief Returns the ID of the tile at the specified position.
   *
   * @param position the position of the tile to query.
   *
   * @return the ID of the tile at the specified position; `std::nullopt` if the
   * position is out-of-bounds.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_at(const position& position) const
      -> std::optional<tile_id>;

  /**
   * @brief Indicates whether or not the layer associated with the specified
   * index is visible.
   *
   * @details This method returns false if the supplied index isn't
   * associated with a tile layer. Tile layers are visible by default.
   *
   * @param layer the index of the tile layer that will be checked. An
   * invalid index results in the method returning `false`.
   *
   * @return `true` if the layer is visible; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto is_visible(layer_id layer) const noexcept -> bool;

  /**
   * @brief Returns the amount of layers present in the map.
   *
   * @details Tilemaps are initially created with 1 tile layer.
   *
   * @return the amount of layers present in the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto layer_count() const noexcept -> int;

  /**
   * @brief Indicates whether or not the specified layer index is associated
   * with a tile layer.
   *
   * @param layer the tile layer index that will be checked.
   *
   * @return `true` if the supplied layer index is associated with a tile layer;
   * `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto has_layer(layer_id layer) const noexcept -> bool;

  /**
   * @brief Indicates whether or not the supplied position is within the bounds
   * of the map.
   *
   * @param pos the position that will be checked.
   *
   * @return `true` if the supplied positions is in bounds; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto in_bounds(const position& pos) const -> bool;

  /**
   * @brief Returns the total number of rows in the map.
   *
   * @return the amount of rows in the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto row_count() const -> row_t;

  /**
   * @brief Returns the total number of columns in the map.
   *
   * @return the amount of columns in the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto col_count() const -> col_t;

  /**
   * @brief Returns the amount of tiles in the map.
   *
   * @return the number of tiles in the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_count() const -> int;

  /**
   * @brief Returns the pixel width of the map.
   *
   * @return the pixel width of the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto width() const -> int;

  /**
   * @brief Returns the pixel height of the map.
   *
   * @return the pixel height of the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto height() const -> int;

  /**
   * @brief Returns the ID of the currently active layer.
   *
   * @return the ID of the active layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto active_layer_id() const noexcept -> layer_id
  {
    return m_activeLayer;
  }

  /**
   * @brief Returns the current tile size value.
   *
   * @return the current tile size.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto current_tile_size() const noexcept -> int
  {
    return m_tileSize.get();
  }

  /**
   * @brief Returns an iterator to the first layer.
   *
   * @return an iterator to the first layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_layers.begin();
  }

  /**
   * @brief Returns an iterator one-past the last layer.
   *
   * @return an iterator one-past the last layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_layers.end();
  }

 private:
  layer_id m_activeLayer{0};
  std::vector<layer> m_layers;
  tile_size m_tileSize;

  [[nodiscard]] auto current_layer() -> layer&;

  [[nodiscard]] auto current_layer() const -> const layer&;
};

}  // namespace tactile::core
