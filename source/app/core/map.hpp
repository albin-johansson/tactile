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

#include "core/ctx/context.hpp"
#include "core/layer/layer_type.hpp"
#include "core/tile_pos.hpp"
#include "core/type/fn.hpp"
#include "core/type/hash_map.hpp"
#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/type/ptr.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Layer)
TACTILE_FWD_DECLARE_CLASS(GroupLayer)
TACTILE_FWD_DECLARE_CLASS(TilesetBundle)
TACTILE_FWD_DECLARE_CLASS(TileFormat)

/// Represents a tile map.
///
/// Maps a built from multiple layers stacked on top of each other. An invisible group
/// layer (the "root" layer) manages the layers in every map. This root layer is always
/// present, even in empty maps. However, this root layer is never included in save files,
/// it is merely an implementation detail to simplify the map code.
///
/// Each map has a collection of associated tilesets, providing the tiles to be used in
/// tile layers. Tilesets may be shared with other maps in the case of external tilesets.
/// See the Tileset class for more information.
///
/// Every map uses its own tile format specification, which can be used to control aspects
/// such as whether tile layer data is compressed. By default, maps do not use any
/// compression along with plain text encoding.
class Map final : public Context {
 public:
  using VisitorFunc = Fn<void(const Layer*)>;
  using TileLayerVisitorFunc = Fn<void(TileLayer&)>;

  /// Maps previous invalid tile identifiers in a collection of layers.
  using FixTilesResult = HashMap<UUID, HashMap<TilePos, TileID>>;

  TACTILE_DELETE_COPY(Map);
  TACTILE_DEFAULT_MOVE(Map);

  /// Creates an empty map.
  Map();

  ~Map() noexcept;

  void accept(ContextVisitor& visitor) const override;

  /// Adds an empty row to all tile layers.
  void add_row();

  /// Adds an empty column to all tile layers.
  void add_column();

  /// Removes the right-most row from all tile layers.
  /// Raises an exception if there is only a single tile row in the map.
  void remove_row();

  /// Removes a column from the bottom of all tile layers.
  /// Raises an exception if there is only a single tile column in the map.
  void remove_column();

  /// Changes the size of all tile layers.
  /// The dimensions must be larger than zero.
  void resize(usize rows, usize columns);

  /// Fixes invalid tile identifiers in all tile layers.
  auto fix_tiles() -> FixTilesResult;

  /// Adds a layer to the map.
  /// The parent parameter can be used to attach a layer to a group layer.
  void add_layer(Shared<Layer> layer, const Maybe<UUID>& parent_id = nothing);

  /// Adds a new tile layer to the map, returning the layer ID.
  auto add_tile_layer(const Maybe<UUID>& parent_id = nothing) -> UUID;

  /// Adds a new object layer to the map, returning the layer ID.
  auto add_object_layer(const Maybe<UUID>& parent_id = nothing) -> UUID;

  /// Adds a new group layer to the map, returning the layer ID.
  auto add_group_layer(const Maybe<UUID>& parent_id = nothing) -> UUID;

  /// Removes an existing layer from the map and returns it.
  auto remove_layer(const UUID& id) -> Shared<Layer>;

  /// Duplicates an existing layer and inserts it after the source layer.
  auto duplicate_layer(const UUID& id) -> Shared<Layer>;

  /// Makes a layer active.
  void select_layer(const UUID& id);

  /// Sets the logical size of all tiles.
  void set_tile_size(const Int2& size);

  auto fetch_and_increment_next_object_id() -> int32;
  auto fetch_and_increment_next_layer_id() -> int32;

  void set_next_object_id(int32 id);
  void set_next_layer_id(int32 id);

  /// Indicates whether the active layer is of the specified type.
  [[nodiscard]] auto is_active_layer(LayerType type) const -> bool;

  [[nodiscard]] auto active_layer_id() const -> Maybe<UUID>;

  /// Indicates whether the stamp tool randomizer mode is available.
  [[nodiscard]] auto is_stamp_randomizer_possible() const -> bool;

  /// Indicates whether a position is within the map.
  [[nodiscard]] auto is_valid_position(const TilePos& pos) const -> bool;

  [[nodiscard]] auto next_object_id() const -> int32;
  [[nodiscard]] auto next_layer_id() const -> int32;

  [[nodiscard]] auto next_tile_layer_suffix() const -> int32;
  [[nodiscard]] auto next_object_layer_suffix() const -> int32;
  [[nodiscard]] auto next_group_layer_suffix() const -> int32;

  [[nodiscard]] auto row_count() const -> usize;
  [[nodiscard]] auto column_count() const -> usize;

  [[nodiscard]] auto tile_size() const -> const Int2&;

  [[nodiscard]] auto tile_format() -> TileFormat&;
  [[nodiscard]] auto tile_format() const -> const TileFormat&;

  [[nodiscard]] auto invisible_root() -> GroupLayer&;
  [[nodiscard]] auto invisible_root() const -> const GroupLayer&;

  [[nodiscard]] auto tileset_bundle() -> TilesetBundle&;
  [[nodiscard]] auto tileset_bundle() const -> const TilesetBundle&;

  [[nodiscard]] auto ctx() -> ContextInfo& override;
  [[nodiscard]] auto ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto uuid() const -> const UUID& override;

 private:
  struct MapData;
  Unique<MapData> mData;

  void each_tile_layer(const TileLayerVisitorFunc& func);
};

}  // namespace tactile
