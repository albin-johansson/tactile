/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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
#include "common/type/ecs.hpp"
#include "common/type/maybe.hpp"
#include "common/type/vec.hpp"
#include "core/tile_matrix.hpp"
#include "core/tile_pos.hpp"

namespace tactile {

/// Represents all of the supported layer types.
enum class LayerType {
  TileLayer,
  ObjectLayer,
  GroupLayer
};

/// Component featured by all layer entities.
struct Layer final {
  LayerType type {};        ///< The specific layer variant.
  int32 id {};              ///< Human-readable identifier associated with the layer.
  float32 opacity {1.0f};   ///< Opacity of the layer when rendered.
  bool visible : 1 {true};  ///< Determines whether the layer is rendered.
};

/// Component for layer variant consisting of a matrix of tile identifiers.
struct TileLayer final {
  TileMatrix tiles;  ///< 2D matrix of identifiers for all tiles in the layer.

  /// Sets the tile identifier at the specified position.
  ///
  /// \note This function does nothing if the provided position is out-of-bounds.
  /// \param pos the position of the tile to modify.
  /// \param tile_id the new tile identifier.
  void set_tile(TilePos pos, TileID tile_id);

  /// Returns the tile identifier at the specified position, as long as it's valid.
  [[nodiscard]] auto tile_at(TilePos pos) const -> Maybe<TileID>;

  /// Indicates whether a position is a valid tile position in the tile layer.
  [[nodiscard]] auto contains(TilePos pos) const -> bool;
};

/// Component for layer variant consisting of objects such as rectangles and points.
struct ObjectLayer final {
  Vec<Entity> objects;                 ///< Entity identifiers of all contained objects.
  Entity active_object {kNullEntity};  ///< The currently active object, if any.
};

/// Component for layer variant consisting of other layers (may be recursive).
struct GroupLayer final {
  Vec<Entity> children;  ///< Entity identifiers of all contained layers.

  /// Adds a layer to the group, asserting that the layer hasn't already been added.
  void append(const Entity layer_entity);
};

}  // namespace tactile