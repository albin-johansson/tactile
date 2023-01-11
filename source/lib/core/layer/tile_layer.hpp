/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/numeric.hpp"
#include "common/type/vec.hpp"
#include "core/layer/abstract_layer.hpp"
#include "core/tile/tile_extent.hpp"
#include "core/tile/tile_matrix.hpp"
#include "core/tile/tile_pos.hpp"

namespace tactile {

/// A layer variant consisting of a matrix of tile identifiers.
class TileLayer final : public AbstractLayer {
 public:
  TileLayer();

  explicit TileLayer(TileExtent extent);

  ~TileLayer() noexcept override = default;

  void accept(ContextVisitor& visitor) const override;
  void accept(LayerVisitor& visitor) override;
  void accept(ConstLayerVisitor& visitor) const override;

  /// Runs a flood-fill algorithm in the layer, used by the bucket tool.
  ///
  /// <p>
  /// This will replace all adjacent tiles of the type at the origin position with the
  /// specified tile replacement.
  ///
  /// \param origin the initial position of the flood-fill.
  /// \param replacement the tile ID that will be used when
  /// \param[out] affected pointer to which all modified positions are written to.
  void flood(const TilePos& origin, TileID replacement, Vec<TilePos>* affected = nullptr);

  /// Appends a single row of empty tiles to the bottom of the layer.
  void add_row();

  /// Appends a single column of empty tiles to the end of the layer.
  void add_column();

  /// Removes a single row from the layer.
  /// There must be more than one row in the layer when this function is called.
  void remove_row();

  /// Removes a single column from the layer.
  /// There must be more than one column in the layer when this function is called.
  void remove_column();

  /// Changes the size of the layer.
  /// Note, layers must have at least 1 row and 1 column.
  void resize(TileExtent extent);

  /// Sets the tile identifier at the specified position.
  /// This function throws for invalid positions.
  void set_tile(const TilePos& pos, TileID id);

  /// Returns the tile identifier at a specific position.
  /// This function throws for invalid positions.
  [[nodiscard]] auto tile_at(const TilePos& pos) const -> TileID;

  /// Indicates whether a tile position refers to a valid tile in the layer.
  [[nodiscard]] auto is_valid(const TilePos& pos) const -> bool;

  /// Returns the number of tile rows in the layer.
  [[nodiscard]] auto row_count() const -> usize;

  /// Returns the number of tile columns in the layer.
  [[nodiscard]] auto column_count() const -> usize;

  /// Returns the associated tile matrix.
  [[nodiscard]] auto get_tiles() const -> const TileMatrix&;

  [[nodiscard]] auto clone() const -> Shared<Layer> override;

  [[nodiscard]] auto get_type() const -> LayerType override
  {
    return LayerType::TileLayer;
  }

 private:
  TileMatrix mTiles;
};

}  // namespace tactile
