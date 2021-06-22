#pragma once

#include <concepts>  // invocable
#include <vector>    // vector

#include "aliases/col.hpp"
#include "aliases/row.hpp"
#include "aliases/tile_id.hpp"
#include "core/map/map_position.hpp"
#include "layer.hpp"
#include "layer_delegate.hpp"

namespace tactile {

using TileRow = std::vector<tile_id>;
using TileMatrix = std::vector<TileRow>;

[[nodiscard]] auto MakeTileRow(col_t nCols, tile_id value) -> TileRow;

[[nodiscard]] auto MakeTileMatrix(row_t nRows, col_t nCols) -> TileMatrix;

/**
 * \class TileLayer
 *
 * \brief Represents a layer consisting of tiles from tilesets.
 *
 * \headerfile tile_layer.hpp
 */
class TileLayer final : public ILayer
{
 public:
  /**
   * \brief Creates a tile layer with the specified dimensions.
   *
   * \details All of the tiles in the created layer are empty by default.
   *
   * \param nRows the initial number of rows in the layer.
   * \param nCols the initial number of columns in the layer.
   *
   * \throws TactileError if `nRows` or `nCols` aren't greater than 0.
   */
  TileLayer(row_t nRows, col_t nCols);

  TileLayer() : TileLayer{5_row, 5_col}
  {}

  /// \name Layer API
  /// \{

  void SetVisible(bool visible) override;

  void SetOpacity(float opacity) override;

  void SetName(std::string name) override;

  [[nodiscard]] auto IsVisible() const -> bool override;

  [[nodiscard]] auto GetOpacity() const noexcept -> float override;

  [[nodiscard]] auto GetType() const -> LayerType override;

  [[nodiscard]] auto Clone() const -> Shared<ILayer> override;

  /// \} End of layer API

  /// \name Tile layer API
  /// \{

  /**
   * \brief Iterates each tile in the layer.
   *
   * \tparam T the type of the function object that will be invoked.
   *
   * \param callable the callable that will be invoked for each tile in the
   * layer.
   *
   * \since 0.1.0
   */
  template <std::invocable<tile_id> T>
  void Each(T&& callable) const
  {
    const auto endRow = GetRowCount();
    const auto endCol = GetColumnCount();
    for (row_t row{0}; row < endRow; ++row)
    {
      for (col_t col{0}; col < endCol; ++col)
      {
        callable(mTiles[row][col]);
      }
    }
  }

  /**
   * \brief Runs a flood fill algorithm in the layer.
   *
   * \param origin the starting position of the flood fill.
   * \param replacement the tile type that will be used as the replacement.
   * \param[out] positions the vector that the affected positions will be added
   * to.
   */
  void Flood(const MapPosition& origin,
             tile_id replacement,
             std::vector<MapPosition>& positions);

  /**
   * \brief Removes all occurrences of the specified ID in the layer.
   *
   * \param id the ID of tiles that will be cleared.
   */
  void RemoveAll(tile_id id);

  /**
   * \brief Removes all occurrences of a range of tiles.
   *
   * \details This function removes tiles in the range [first, last].
   *
   * \param first the first tile in the range that will be removed.
   * \param last the last tile in the range that will be removed.
   */
  void RemoveAll(tile_id first, tile_id last);

  /**
   * \brief Adds a row to the tile layer.
   *
   * \param id the tile ID of all of the new cells in the added row.
   */
  void AddRow(tile_id id);

  /**
   * \brief Adds a column to the tile layer.
   *
   * \param id the tile ID of all of the new cells in the added column.
   */
  void AddColumn(tile_id id);

  /**
   * \brief Removes a row from the tile layer.
   *
   * \note The tile layer will always have at least one row, so this method
   * has no effect if the tile layer has exactly one row.
   */
  void RemoveRow() noexcept;

  /**
   * \brief Removes a column from the tile layer.
   *
   * \note The tile layer will always have at least one column, so this
   * method has no effect if the tile layer has exactly one column.
   */
  void RemoveColumn() noexcept;

  /**
   * \brief Sets the total number of rows in the layer.
   *
   * \pre `nRows` must be greater than or equal to 1.
   *
   * \param nRows the new number of rows in the layer.
   */
  void SetRowCount(row_t nRows);

  /**
   * \brief Sets the total number of columns in the layer.
   *
   * \pre `nCols` must be greater than or equal to 1.
   *
   * \param nCols the new number of columns in the layer.
   */
  void SetColumnCount(col_t nCols);

  /**
   * \brief Sets the tile ID of a tile in the tile layer.
   *
   * \note This function has no effect if the supplied position is
   * out-of-bounds.
   *
   * \param position the position of the tile that will be changed.
   * \param id the new tile ID.
   */
  void SetTile(const MapPosition& position, tile_id id);

  /**
   * \brief Returns the number of rows in the tile layer.
   *
   * \note A tile layer always contains at least one row.
   *
   * \return the number of rows in the tile layer.
   */
  [[nodiscard]] auto GetRowCount() const noexcept -> row_t;

  /**
   * \brief Returns the number of columns in the tile layer.
   *
   * \note A tile layer always contains at least one column.
   *
   * \return the number of columns in the tile layer.
   */
  [[nodiscard]] auto GetColumnCount() const noexcept -> col_t;

  /**
   * \brief Returns the total amount of tiles in the layer.
   *
   * \return the number of tiles in the layer.
   */
  [[nodiscard]] auto GetTileCount() const noexcept -> int;

  /**
   * \brief Returns the ID of the tile at the specified position.
   *
   * \param position the position to obtain the tile ID of.
   *
   * \return the ID of the tile at the specified position; `nothing` if the
   * position is out-of-bounds.
   */
  [[nodiscard]] auto GetTile(const MapPosition& position) const
      -> Maybe<tile_id>;

  /**
   * \brief Indicates whether or not the specified position is in bounds of the
   * tile layer.
   *
   * \param position the position that will be checked.
   *
   * \return `true` if the position is in bounds; `false` otherwise.
   */
  [[nodiscard]] auto InBounds(const MapPosition& position) const -> bool;

  /// \} End of tile layer API

  /// \name Property API
  /// \{

  void AddProperty(const std::string& name, PropertyType type) override;

  void AddProperty(const std::string& name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName,
                      const std::string& newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string_view name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> std::string_view override;

  /// \} End of property API

 private:
  TileMatrix mTiles;
  LayerDelegate mDelegate;
};

}  // namespace tactile
