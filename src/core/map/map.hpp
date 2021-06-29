#pragma once

#include <concepts>        // invocable
#include <string_view>     // string_view
#include <vector_map.hpp>  // vector_map

#include "aliases/col.hpp"
#include "aliases/ints.hpp"
#include "aliases/layer_id.hpp"
#include "aliases/maybe.hpp"
#include "aliases/object_id.hpp"
#include "aliases/row.hpp"
#include "aliases/shared.hpp"
#include "aliases/tile_id.hpp"
#include "core/map/layers/layer.hpp"
#include "core/map/layers/object_layer.hpp"
#include "core/map/layers/tile_layer.hpp"

namespace Tactile {

/**
 * \class Map
 *
 * \brief Represents a tilemap.
 *
 * \see `TileLayer`
 * \see `ObjectLayer`
 *
 * \headerfile map.hpp
 */
class Map final
{
 public:
  using layer_storage = rune::vector_map<layer_id, SharedLayer>;
  using layer_pair = typename layer_storage::value_type;
  using const_iterator = typename layer_storage::const_iterator;

  /// Creates an empty tilemap with no layers.
  Map() = default;

  /**
   * \brief Creates a map with a single tile layer.
   *
   * \param nRows the initial number of rows in the map.
   * \param nCols the initial number of columns in the map.
   *
   * \throws TactileError if `nRows` or `nCols` aren't greater than 0.
   */
  Map(row_t nRows, col_t nCols);

  /**
   * \brief Visits each layer in the map.
   *
   * \tparam T the type of the function object.
   *
   * \param callable the callable that will be invoked for each layer.
   */
  template <std::invocable<layer_id, const SharedLayer&> T>
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
   */
  void RemoveOccurrences(tile_id id);

  /**
   * \brief Removes all occurrences of a range of tiles, in all tile layers.
   *
   * \details This function removes tiles in the range [first, last].
   *
   * \param first the first tile in the range that will be removed.
   * \param last the last tile in the range that will be removed.
   */
  void RemoveOccurrences(tile_id first, tile_id last);

  /**
   * \brief Adds an empty tile layer to the map.
   *
   * \note The added layer will *not* be made the active layer.
   */
  auto AddTileLayer() -> layer_id;

  /**
   * \brief Adds an empty object layer to the map.
   *
   * \note The added layer will *not* be made the active layer.
   */
  auto AddObjectLayer() -> layer_id;

  /**
   * \brief Adds a layer to the map.
   *
   * \note This function does *not* modify the next layer ID property.
   *
   * \param id the ID that will be associated with the layer.
   * \param layer the layer that will be added.
   */
  void AddLayer(layer_id id, SharedLayer layer);

  /**
   * \brief Duplicates the layer associated with the specified ID.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \param id the ID of the layer that will be duplicated.
   *
   * \return the new layer ID and the duplicated layer.
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
   * \todo Should another layer be made active instead?
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
   */
  [[nodiscard]] auto TakeLayer(layer_id id) -> SharedLayer;

  /**
   * \brief Selects the tile layer associated with the specified ID.
   *
   * \note This function has no effect if the supplied ID is invalid.
   *
   * \param id the layer ID of the tile layer that will be selected.
   */
  void SelectLayer(layer_id id);

  /**
   * \brief Adds a row to the map.
   *
   * \param id the tile ID that the new tiles will have.
   */
  void AddRow(tile_id id);

  /**
   * \brief Adds a column to the map.
   *
   * \param id the tile ID that the new tiles will have.
   */
  void AddColumn(tile_id id);

  /**
   * \brief Removes a row from the map.
   *
   * \note This function has no effect if the tile map only contains one row.
   */
  void RemoveRow();

  /**
   * \brief Removes a column from the map.
   *
   * \note This function has no effect if the tile map only contains one column.
   */
  void RemoveColumn();

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
   */
  void SetNextLayerId(layer_id id) noexcept;

  /**
   * \brief Sets the next available object ID.
   *
   * \pre There mustn't be an object associated with `id`.
   *
   * \details This function is meant to be used when parsing maps from save
   * files.
   *
   * \param id the ID that will be used by the next object.
   */
  void SetNextObjectId(object_id id) noexcept;

  /**
   * \brief Sets the total number of rows in the map.
   *
   * \param nRows the new number of rows in the map. Clamped to be at
   * least 1.
   */
  void SetRowCount(row_t nRows);

  /**
   * \brief Sets the total number of columns in the map.
   *
   * \param nCols the new number of columns in the map. Clamped to be at
   * least 1.
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
   */
  void SetVisible(layer_id id, bool visible);

  /**
   * \brief Sets the opacity of a layer.
   *
   * \note This function has no effect if the specified layer ID isn't
   * associated with a layer.
   *
   * \param id the ID of the layer that will be modified.
   * \param opacity the new opacity of the layer, in the range [0, 1].
   */
  void SetOpacity(layer_id id, float opacity);

  /**
   * \brief Sets the name of the specified layer.
   *
   * \note This function has no effect if the specified layer ID isn't
   * associated with a layer.
   *
   * \param id the ID of the layer that will be modified.
   * \param name the name that will be associated with the layer.
   */
  void SetName(layer_id id, std::string name);

  /**
   * \brief Moves the specified layer back one step in the render order.
   *
   * \details This function will make the specified layer be rendered later.
   *
   * \note This function has no effect if the specified layer ID isn't
   * associated with a layer.
   *
   * \param id the ID associated with the layer that will be moved.
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
   */
  void MoveLayerForward(layer_id id);

  /**
   * \brief Creates and returns an empty tile layer but doesn't add it to the
   * map.
   *
   * \note This function *does* increment the next layer ID property.
   *
   * \return the created layer.
   */
  [[nodiscard]] auto MakeTileLayer() -> Shared<TileLayer>;

  /**
   * \brief Creates and returns an empty object layer but doesn't add it to the
   * map.
   *
   * \note This function *does* increment the next layer ID property.
   *
   * \return the created layer.
   */
  [[nodiscard]] auto MakeObjectLayer() -> Shared<ObjectLayer>;

  /**
   * \brief Returns the index associated with the specified layer.
   *
   * \param id the ID associated with the layer to obtain the index of.
   *
   * \return the index of the specified layer; `nothing` if the layer wasn't
   * found.
   */
  [[nodiscard]] auto IndexOf(layer_id id) const -> Maybe<usize>;

  /**
   * \brief Returns the name of the specified layer.
   *
   * \param id the ID associated with the layer to get the name of.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \return the name of the specified layer.
   */
  [[nodiscard]] auto GetName(layer_id id) const -> std::string_view;

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
   */
  [[nodiscard]] auto IsVisible(layer_id id) const -> bool;

  /**
   * \brief Returns the opacity of the specified layer.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \param id the ID associated with the layer that will be queried.
   *
   * \return the opacity of the layer, in the range [0, 1].
   */
  [[nodiscard]] auto GetOpacity(layer_id id) const -> float;

  /**
   * \brief Returns the amount of layers present in the map.
   *
   * \details Tilemaps are initially created with 1 tile layer.
   *
   * \return the amount of layers present in the map.
   */
  [[nodiscard]] auto GetLayerCount() const -> int;

  /**
   * \brief Indicates whether or not the specified layer ID is associated
   * with a tile layer.
   *
   * \param id the tile layer ID that will be checked.
   *
   * \return `true` if the supplied layer ID is associated with a tile layer;
   * `false` otherwise.
   */
  [[nodiscard]] auto HasLayer(layer_id id) const -> bool;

  /**
   * \brief Returns the layer associated with the specified ID.
   *
   * \note This function throws if no layer is found.
   *
   * \param id the ID associated with the desired layer.
   *
   * \return a reference to the layer associated with the specified ID.
   */
  [[nodiscard]] auto GetLayer(layer_id id) const -> const SharedLayer&;

  [[nodiscard]] auto GetLayer(layer_id id) -> SharedLayer&;

  [[nodiscard]] auto GetTileLayer(layer_id id) -> TileLayer*;

  [[nodiscard]] auto GetTileLayer(layer_id id) const -> const TileLayer*;

  [[nodiscard]] auto GetObjectLayer(layer_id id) -> ObjectLayer*;

  [[nodiscard]] auto GetObjectLayer(layer_id id) const -> const ObjectLayer*;

  /**
   * \brief Indicates whether or not the supplied position is within the bounds
   * of the map.
   *
   * \param pos the position that will be checked.
   *
   * \return `true` if the supplied positions is in bounds; `false` otherwise.
   */
  [[nodiscard]] auto InBounds(const MapPosition& pos) const -> bool;

  /**
   * \brief Returns the total number of rows in the map.
   *
   * \return the amount of rows in the map.
   */
  [[nodiscard]] auto GetRowCount() const -> row_t
  {
    return mRows;
  }

  /**
   * \brief Returns the total number of columns in the map.
   *
   * \return the amount of columns in the map.
   */
  [[nodiscard]] auto GetColumnCount() const -> col_t
  {
    return mCols;
  }

  /**
   * \brief Returns the ID of the currently active layer.
   *
   * \return the ID of the active layer; `nothing` if there is no active
   * layer.
   */
  [[nodiscard]] auto GetActiveLayerId() const noexcept -> Maybe<layer_id>
  {
    return mActiveLayer;
  }

  /**
   * \brief Returns the ID that will be used by the next layer.
   *
   * \return the ID of the next layer.
   */
  [[nodiscard]] auto GetNextLayerId() const noexcept -> layer_id
  {
    return mNextLayerId;
  }

  [[nodiscard]] auto GetNextObjectId() const noexcept -> object_id
  {
    return mNextObjectId;
  }

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mLayers.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mLayers.end();
  }

 private:
  layer_storage mLayers;
  Maybe<layer_id> mActiveLayer;
  row_t mRows;
  col_t mCols;
  layer_id mNextLayerId{1};
  object_id mNextObjectId{1};

  [[nodiscard]] auto FindLayer(layer_id id) -> ILayer*;

  [[nodiscard]] auto FindLayer(layer_id id) const -> const ILayer*;
};

}  // namespace Tactile
