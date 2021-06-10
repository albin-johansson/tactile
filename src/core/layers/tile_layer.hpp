#pragma once

#include <QString>   // QString
#include <concepts>  // invocable
#include <vector>    // vector

#include "layer.hpp"
#include "layer_delegate.hpp"
#include "maybe.hpp"
#include "position.hpp"
#include "tile_id.hpp"

namespace tactile::core {

using tile_row = std::vector<tile_id>;
using tile_matrix = std::vector<tile_row>;

/**
 * \class TileLayer
 *
 * \brief Represents a layer of tiles in a map.
 *
 * \since 0.1.0
 *
 * \see `map`
 *
 * \headerfile tile_layer.hpp
 */
class TileLayer final : public ILayer
{
 public:
  /**
   * \brief Creates a tile layer with the specified dimensions.
   *
   * \details All of the tiles in the created layer are initialized with the
   * value of the `empty` tile ID.
   *
   * \param nRows the initial number of rows in the layer.
   * \param nCols the initial number of columns in the layer.
   *
   * \throws tactile_error if `nRows` or `nCols` aren't greater than 0.
   *
   * \since 0.1.0
   */
  TileLayer(row_t nRows, col_t nCols);

  /**
   * \brief Creates a tile layer.
   *
   * \since 0.1.0
   */
  TileLayer() : TileLayer{5_row, 5_col}
  {}

  /// \name Layer API
  /// \{

  void SetOpacity(double opacity) override;

  void SetName(QString name) override;

  void SetVisible(bool visible) noexcept override;

  [[nodiscard]] auto Type() const -> LayerType override;

  [[nodiscard]] auto Name() const -> const QString& override;

  [[nodiscard]] auto Opacity() const noexcept -> double override;

  [[nodiscard]] auto IsVisible() const noexcept -> bool override;

  [[nodiscard]] auto Clone() const -> shared<ILayer> override;

  /// \} End of layer API

  /// \name Property API
  /// \{

  void AddProperty(const QString& name, PropertyType type) override;

  void AddProperty(const QString& name, const Property& property) override;

  void RemoveProperty(const QString& name) override;

  void RenameProperty(const QString& oldName, const QString& newName) override;

  void SetProperty(const QString& name, const Property& property) override;

  void ChangePropertyType(const QString& name,
                          core::PropertyType type) override;

  [[nodiscard]] auto GetProperty(const QString& name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperty(const QString& name) -> Property& override;

  [[nodiscard]] auto HasProperty(const QString& name) const -> bool override;

  [[nodiscard]] auto PropertyCount() const noexcept -> int override;

  [[nodiscard]] auto GetProperties() const -> const property_map& override;

  /// \} End of property API

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
    const auto endRow = RowCount();
    const auto endCol = ColumnCount();
    for (row_t row{0}; row < endRow; ++row)
    {
      for (col_t col{0}; col < endCol; ++col)
      {
        callable(mTiles[row][col]);
      }
    }
  }

  /**
   * \brief Runs a flood fill in the layer.
   *
   * \param origin the starting position of the flood fill.
   * \param replacement the tile type that will be used as the replacement.
   * \param[out] positions the vector that the affected positions will be added
   * to.
   *
   * \since 0.1.0
   */
  void Flood(const Position& origin,
             tile_id replacement,
             std::vector<Position>& positions);

  /**
   * \brief Removes all occurrences of the specified ID in the layer.
   *
   * \param id the ID that will be replaced with `empty`.
   *
   * \since 0.1.0
   */
  void RemoveAll(tile_id id);

  /**
   * \brief Removes all occurrences of a range of tiles. This function removes
   * tiles in the range [first, last].
   *
   * \param first the first tile in the range that will be removed.
   * \param last the last tile in the range that will be removed.
   *
   * \since 0.2.0
   */
  void RemoveAll(tile_id first, tile_id last);

  /**
   * \brief Adds a row to the tile layer.
   *
   * \param id the tile ID of all of the new cells in the added row.
   *
   * \since 0.1.0
   */
  void AddRow(tile_id id);

  /**
   * \brief Adds a column to the tile layer.
   *
   * \param id the tile ID of all of the new cells in the added column.
   *
   * \since 0.1.0
   */
  void AddColumn(tile_id id);

  /**
   * \brief Removes a row from the tile layer.
   *
   * \note The tile layer will always have at least one row, so this method
   * has no effect if the tile layer has exactly one row.
   *
   * \since 0.1.0
   */
  void RemoveRow() noexcept;

  /**
   * \brief Removes a column from the tile layer.
   *
   * \note The tile layer will always have at least one column, so this
   * method has no effect if the tile layer has exactly one column.
   *
   * \since 0.1.0
   */
  void RemoveColumn() noexcept;

  /**
   * \brief Sets the total number of rows in the layer.
   *
   * \pre `nRows` must be greater than or equal to 1.
   *
   * \param nRows the new number of rows in the layer.
   *
   * \since 0.1.0
   */
  void SetRows(row_t nRows);

  /**
   * \brief Sets the total number of columns in the layer.
   *
   * \pre `nCols` must be greater than or equal to 1.
   *
   * \param nCols the new number of columns in the layer.
   *
   * \since 0.1.0
   */
  void SetColumns(col_t nCols);

  /**
   * \brief Sets the tile ID of a tile in the tile layer.
   *
   * \note This method has no effect if the supplied position is out-of-bounds.
   *
   * \param pos the position of the tile that will be changed.
   * \param id the new tile ID.
   *
   * \since 0.1.0
   */
  void SetTile(const Position& pos, tile_id id) noexcept;

  /**
   * \brief Returns the number of rows in the tile layer.
   *
   * \note A tile layer always contains at least one row.
   *
   * \return the number of rows in the tile layer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto RowCount() const noexcept -> row_t;

  /**
   * \brief Returns the number of columns in the tile layer.
   *
   * \note A tile layer always contains at least one column.
   *
   * \return the number of columns in the tile layer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto ColumnCount() const noexcept -> col_t;

  /**
   * \brief Returns the total amount of tiles in the layer.
   *
   * \return the number of tiles in the layer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto TileCount() const noexcept -> int;

  /**
   * \brief Returns the ID of the tile at the specified position.
   *
   * \note This method returns `std::nullopt` if the supplied position
   * is out-of-bounds.
   *
   * \param pos the position to obtain the tile ID of.
   *
   * \return the ID of the tile at the specified position; `std::nullopt` if the
   * position is out-of-bounds.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto TileAt(const Position& pos) const -> maybe<tile_id>;

  /**
   * \brief Indicates whether or not the specified position is in bounds of the
   * tile layer.
   *
   * \param pos the position that will be checked.
   *
   * \return `true` if the position is in bounds; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto InBounds(const Position& pos) const noexcept -> bool;

  /// \} End of tile layer API

 private:
  tile_matrix mTiles;
  LayerDelegate mDelegate;
};

[[nodiscard]] auto MakeTileRow(col_t nCols, tile_id value = empty) -> tile_row;

[[nodiscard]] auto MakeTileMatrix(row_t nRows, col_t nCols) -> tile_matrix;

}  // namespace tactile::core
