#pragma once

#include <QString>   // QString
#include <concepts>  // invocable
#include <utility>   // pair, move

#include "layer.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "object_id.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"
#include "tile_id.hpp"
#include "tile_size.hpp"
#include "vector_map.hpp"

namespace tactile::core {

class TileLayer;
class ObjectLayer;

/**
 * \class Map
 *
 * \brief Represents the main tilemaps in the application.
 *
 * \details Tilemaps are composed by multiple tile layers.
 *
 * \see `TileLayer`
 * \see `ObjectLayer`
 *
 * \since 0.1.0
 *
 * \headerfile map.hpp
 */
class Map final
{
  using layer_pair = std::pair<layer_id, Shared<ILayer>>;
  using layer_map = vector_map<layer_id, Shared<ILayer>>;

 public:
  using const_iterator = layer_map::const_iterator;

  /**
   * \brief Creates an empty map with no layers.
   *
   * \since 0.1.0
   */
  Map() = default;

  /**
   * \brief Creates a map with one tile layer.
   *
   * \param nRows the initial number of rows in the map.
   * \param nCols the initial number of columns in the map.
   *
   * \throws TactileError if `nRows` or `nCols` aren't greater than 0.
   *
   * \since 0.1.0
   */
  Map(row_t nRows, col_t nCols);

  /**
   * \brief Visits each layer in the map.
   *
   * \tparam T the type of the function object.
   *
   * \param callable the callable that will be invoked for each layer.
   *
   * \since 0.1.0
   */
  template <std::invocable<layer_id, const Shared<ILayer>&> T>
  void EachLayer(T&& callable) const
  {
    for (const auto& [key, layer] : mLayers)
    {
      callable(key, layer);
    }
  }

  /**
   * \brief Removes all occurrences of the specified ID in all layers.
   *
   * \param id the ID that will be replaced with `empty`.
   *
   * \since 0.1.0
   */
  void RemoveOccurrences(tile_id id);

  /**
   * \brief Removes all occurrences of a range of tiles, in all tile layers.
   * This function removes tiles in the range [first, last].
   *
   * \param first the first tile in the range that will be removed.
   * \param last the last tile in the range that will be removed.
   *
   * \since 0.2.0
   */
  void RemoveOccurrences(tile_id first, tile_id last);

  /**
   * \brief Adds an empty tile layer to the map.
   *
   * \note The added layer will *not* be made the active layer.
   *
   * \since 0.1.0
   */
  auto AddTileLayer() -> layer_id;

  /**
   * \brief Adds an empty object layer to the map.
   *
   * \note The added layer will *not* be made the active layer.
   *
   * \since 0.2.0
   */
  auto AddObjectLayer() -> layer_id;

  /**
   * \brief Adds a layer to the map.
   *
   * \note This function does *not* modify the next layer ID property.
   *
   * \param id the ID that will be associated with the layer.
   * \param layer the layer that will be added.
   *
   * \since 0.1.0
   */
  void AddLayer(layer_id id, Shared<ILayer> layer);

  /**
   * \brief Duplicates the layer associated with the specified ID.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \param id the ID of the layer that will be duplicated.
   *
   * \return the new layer ID and the duplicated layer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto DuplicateLayer(layer_id id) -> layer_pair&;

  /**
   * \brief Removes the specified layer from the map.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \warning If the removed layer was the active layer, then no other layer
   * will be made active.
   *
   * \since 0.1.0
   */
  void RemoveLayer(layer_id id);

  /**
   * \brief Removes a layer from the map and returns it.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \param id the ID associated with the layer that will be removed.
   *
   * \return the layer that was removed.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto TakeLayer(layer_id id) -> Shared<ILayer>;

  /**
   * \brief Selects the tile layer associated with the specified ID.
   *
   * \note This method has no effect if the supplied ID is invalid.
   *
   * \param id the layer ID of the tile layer that will be selected.
   *
   * \since 0.1.0
   */
  void SelectLayer(layer_id id);

  /**
   * \brief Adds a row to the map.
   *
   * \param id the tile ID that the new tiles will have.
   *
   * \since 0.1.0
   */
  void AddRow(tile_id id);

  /**
   * \brief Adds a column to the map.
   *
   * \param id the tile ID that the new tiles will have.
   *
   * \since 0.1.0
   */
  void AddColumn(tile_id id);

  /**
   * \brief Removes a row from the map.
   *
   * \note This method has no effect if the tile map only contains one row.
   *
   * \since 0.1.0
   */
  void RemoveRow();

  /**
   * \brief Removes a column from the map.
   *
   * \note This method has no effect if the tile map only contains one column.
   *
   * \since 0.1.0
   */
  void RemoveColumn();

  void IncreaseTileSize();

  void DecreaseTileSize() noexcept;

  void ResetTileSize() noexcept;

  /**
   * \brief Sets the next available layer ID.
   *
   * \pre There mustn't be a layer associated with `id`.
   *
   * \details This property is automatically incremented when calling
   * `AddTileLayer()`. This function is meant to be used when parsing maps
   * from save files.
   *
   * \param id the ID that will be used by the next layer.
   *
   * \since 0.1.0
   */
  void SetNextLayerId(layer_id id) noexcept;

  void SetNextObjectId(object_id id) noexcept;

  /**
   * \brief Sets the total number of rows in the map.
   *
   * \param nRows the new number of rows in the map. Clamped to be at
   * least 1.
   *
   * \since 0.1.0
   */
  void SetRowCount(row_t nRows);

  /**
   * \brief Sets the total number of columns in the map.
   *
   * \param nCols the new number of columns in the map. Clamped to be at
   * least 1.
   *
   * \since 0.1.0
   */
  void SetColumnCount(col_t nCols);

  /**
   * \brief Sets the visibility of a tile layer.
   *
   * \note This function has no effect if the specified layer ID isn't
   * associated with a tile layer.
   *
   * \param id the ID of the layer that will have its visibility changed.
   * \param visible `true` if the layer should be visible; `false` otherwise.
   *
   * \since 0.1.0
   */
  void SetVisibility(layer_id id, bool visible);

  /**
   * \brief Sets the opacity of a layer.
   *
   * \note This function has no effect if the specified layer ID isn't
   * associated with a layer.
   *
   * \param id the ID of the layer that will be modified.
   * \param opacity the new opacity of the layer, in the range [0, 1].
   *
   * \since 0.1.0
   */
  void SetOpacity(layer_id id, double opacity);

  /**
   * \brief Sets the name of the specified layer.
   *
   * \note This function has no effect if the specified layer ID isn't
   * associated with a layer.
   *
   * \param id the ID of the layer that will be modified.
   * \param name the name that will be associated with the layer.
   *
   * \since 0.1.0
   */
  void SetName(layer_id id, const QString& name);

  /**
   * \brief Moves the specified layer back one step in the render order.
   *
   * \details This function will make the specified layer be rendered later.
   *
   * \note This function has no effect if the specified layer ID isn't
   * associated with a layer.
   *
   * \param id the ID associated with the layer that will be moved.
   *
   * \since 0.1.0
   */
  void MoveLayerBack(layer_id id);

  /**
   * \brief Moves the specified layer forwards one step in the render order.
   *
   * \details This function will make the specified layer be rendered earlier.
   *
   * \note This function has no effect if the specified layer ID isn't
   * associated with a layer.
   *
   * \param id the ID associated with the layer that will be moved.
   *
   * \since 0.1.0
   */
  void MoveLayerForward(layer_id id);

  /**
   * \brief Creates and returns an empty tile layer but doesn't add it to the
   * map.
   *
   * \note This function *does* increment the next layer ID property.
   *
   * \return the created layer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto MakeTileLayer() -> Shared<TileLayer>;

  /**
   * \brief Creates and returns an empty object layer but doesn't add it to the
   * map.
   *
   * \note This function *does* increment the next layer ID property.
   *
   * \return the created layer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto MakeObjectLayer() -> Shared<ObjectLayer>;

  /**
   * \brief Returns the index associated with the specified layer.
   *
   * \param id the ID associated with the layer to obtain the index of.
   *
   * \return the index of the specified layer; `nothing` if the layer
   * wasn't found.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto IndexOf(layer_id id) const -> Maybe<std::size_t>;

  /**
   * \brief Returns the name of the specified layer.
   *
   * \param id the ID associated with the layer to get the name of.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \return the name of the specified layer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto Name(layer_id id) const -> QString;

  /**
   * \brief Indicates whether or not the layer associated with the specified
   * ID is visible.
   *
   * \details This method returns false if the supplied ID isn't associated with
   * a tile layer. Tile layers are visible by default.
   *
   * \param id the ID of the tile layer that will be checked.
   *
   * \return `true` if the layer is visible; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto IsVisible(layer_id id) const -> bool;

  /**
   * \brief Returns the amount of layers present in the map.
   *
   * \details Tilemaps are initially created with 1 tile layer.
   *
   * \return the amount of layers present in the map.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto LayerCount() const noexcept -> int;

  /**
   * \brief Indicates whether or not the specified layer ID is associated
   * with a tile layer.
   *
   * \param id the tile layer ID that will be checked.
   *
   * \return `true` if the supplied layer ID is associated with a tile layer;
   * `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto HasLayer(layer_id id) const -> bool;

  /**
   * \brief Indicates whether or not the supplied position is within the bounds
   * of the map.
   *
   * \param pos the position that will be checked.
   *
   * \return `true` if the supplied positions is in bounds; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto InBounds(const Position& pos) const -> bool;

  /**
   * \brief Returns the total number of rows in the map.
   *
   * \return the amount of rows in the map.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto RowCount() const -> row_t;

  /**
   * \brief Returns the total number of columns in the map.
   *
   * \return the amount of columns in the map.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto ColumnCount() const -> col_t;

  /**
   * \brief Returns the pixel width of the map.
   *
   * \return the pixel width of the map.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto Width() const -> int;

  /**
   * \brief Returns the pixel height of the map.
   *
   * \return the pixel height of the map.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto Height() const -> int;

  /**
   * \brief Returns the ID of the currently active layer.
   *
   * \return the ID of the active layer; `nothing` if there is no active
   * layer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto ActiveLayerId() const noexcept -> Maybe<layer_id>;

  /**
   * \brief Returns the ID that will be used by the next layer.
   *
   * \return the ID of the next layer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto NextLayerId() const noexcept -> layer_id;

  /**
   * \brief Returns the current tile size value.
   *
   * \return the current tile size.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto CurrentTileSize() const noexcept -> int;

  /**
   * \brief Returns the layer associated with the specified ID.
   *
   * \note This function throws if no layer is found.
   *
   * \param id the ID associated with the desired layer.
   *
   * \return a reference to the layer associated with the specified ID.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto GetLayer(layer_id id) const -> const Shared<ILayer>&;

  [[nodiscard]] auto GetLayer(layer_id id) -> Shared<ILayer>&;

  [[nodiscard]] auto GetTileLayer(layer_id id) -> TileLayer*;

  [[nodiscard]] auto GetTileLayer(layer_id id) const -> const TileLayer*;

  [[nodiscard]] auto GetObjectLayer(layer_id id) -> ObjectLayer*;

  [[nodiscard]] auto GetObjectLayer(layer_id id) const -> const ObjectLayer*;

  /**
   * \brief Returns an iterator to the first layer.
   *
   * \return an iterator to the first layer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mLayers.begin();
  }

  /**
   * \brief Returns an iterator one-past the last layer.
   *
   * \return an iterator one-past the last layer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mLayers.end();
  }

 private:
  layer_map mLayers;
  Maybe<layer_id> mActiveLayer;
  TileSize mTileSize;
  row_t mRows;
  col_t mCols;
  layer_id mNextLayer{1};
  object_id mNextObject{1};

  [[nodiscard]] auto FindLayer(layer_id id) -> ILayer*;

  [[nodiscard]] auto FindLayer(layer_id id) const -> const ILayer*;
};

}  // namespace tactile::core
