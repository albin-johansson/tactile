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

#include <functional>  // function

#include "core/common/associative.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/math.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/comp/component_bundle.hpp"
#include "core/ctx/context.hpp"
#include "core/ctx/context_delegate.hpp"
#include "core/ctx/property_bundle.hpp"
#include "core/layer/group_layer.hpp"
#include "core/tile_pos.hpp"
#include "core/tileset/tileset_bundle.hpp"

namespace tactile {

class Map final : public IContext
{
 public:
  using VisitorFunc = std::function<void(const ILayer*)>;

  /// Maps previous invalid tile identifiers in a collection of layers.
  using FixTilesResult = HashMap<UUID, HashMap<TilePos, TileID>>;

  Map();

  void add_row();
  void add_column();

  void remove_row();
  void remove_column();

  void resize(usize rows, usize columns);

  /// Fixes invalid tile identifiers in all tile layers.
  auto fix_tiles() -> FixTilesResult;

  auto add_layer(Shared<ILayer> layer, const Maybe<UUID>& parentId = nothing) -> UUID;

  auto add_tile_layer(const Maybe<UUID>& parentId = nothing) -> UUID;
  auto add_object_layer(const Maybe<UUID>& parentId = nothing) -> UUID;
  auto add_group_layer(const Maybe<UUID>& parentId = nothing) -> UUID;

  auto remove_layer(const UUID& id) -> Shared<ILayer>;

  auto duplicate_layer(const UUID& id) -> Shared<ILayer>;

  void visit_layers(IConstLayerVisitor& visitor) const;
  void visit_layers(const VisitorFunc& visitor) const;

  void move_layer_up(const UUID& id);
  void move_layer_down(const UUID& id);

  void select_layer(const UUID& id);

  void set_layer_index(const UUID& id, usize index);

  [[nodiscard]] auto local_layer_index(const UUID& id) const -> usize;

  [[nodiscard]] auto can_move_layer_up(const UUID& id) const -> bool;
  [[nodiscard]] auto can_move_layer_down(const UUID& id) const -> bool;

  [[nodiscard]] auto is_active_layer(LayerType type) const -> bool;

  [[nodiscard]] auto layer_count() const -> usize;

  [[nodiscard]] auto active_layer_id() const -> Maybe<UUID>;

  [[nodiscard]] auto view_layer(const UUID& id) -> ILayer&;
  [[nodiscard]] auto view_layer(const UUID& id) const -> const ILayer&;

  [[nodiscard]] auto view_tile_layer(const UUID& id) -> TileLayer&;
  [[nodiscard]] auto view_tile_layer(const UUID& id) const -> const TileLayer&;

  [[nodiscard]] auto view_object_layer(const UUID& id) -> ObjectLayer&;
  [[nodiscard]] auto view_object_layer(const UUID& id) const -> const ObjectLayer&;

  [[nodiscard]] auto view_group_layer(const UUID& id) -> GroupLayer&;

  [[nodiscard]] auto find_tile_layer(const UUID& id) -> TileLayer*;
  [[nodiscard]] auto find_tile_layer(const UUID& id) const -> const TileLayer*;

  [[nodiscard]] auto find_object_layer(const UUID& id) const -> const ObjectLayer*;
  [[nodiscard]] auto find_group_layer(const UUID& id) const -> const GroupLayer*;

  [[nodiscard]] auto get_layer(const UUID& id) -> Shared<ILayer>;

  void attach_tileset(Shared<Tileset> tileset, TileID firstTileId, bool embedded);
  void attach_tileset(Shared<Tileset> tileset, bool embedded);
  void detach_tileset(const UUID& id);
  void select_tileset(const UUID& id);

  [[nodiscard]] auto get_tilesets() -> TilesetBundle&;
  [[nodiscard]] auto get_tilesets() const -> const TilesetBundle&;

  void set_tile_size(const Vector2i& size);

  void accept(IContextVisitor& visitor) const override;

  void set_name(std::string name) override;

  [[nodiscard]] auto is_valid_position(const TilePos& pos) const -> bool;

  [[nodiscard]] auto row_count() const -> usize;

  [[nodiscard]] auto column_count() const -> usize;

  [[nodiscard]] auto tile_size() const -> const Vector2i&;

  [[nodiscard]] auto is_stamp_randomizer_possible() const -> bool;

  [[nodiscard]] auto get_props() -> PropertyBundle& override;
  [[nodiscard]] auto get_props() const -> const PropertyBundle& override;

  [[nodiscard]] auto get_comps() -> ComponentBundle& override;
  [[nodiscard]] auto get_comps() const -> const ComponentBundle& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_name() const -> const std::string& override;

  auto fetch_and_increment_next_object_id() -> int32;
  auto fetch_and_increment_next_layer_id() -> int32;

  void set_next_object_id(int32 id);
  void set_next_layer_id(int32 id);

  [[nodiscard]] auto next_object_id() const -> int32;
  [[nodiscard]] auto next_layer_id() const -> int32;

  [[nodiscard]] auto next_tile_layer_suffix() const -> int32;
  [[nodiscard]] auto next_object_layer_suffix() const -> int32;
  [[nodiscard]] auto next_group_layer_suffix() const -> int32;

 private:
  UUID            mId {make_uuid()};
  usize           mRowCount {5};
  usize           mColCount {5};
  Vector2i        mTileSize {32, 32};
  GroupLayer      mRootLayer;
  Maybe<UUID>     mActiveLayer;
  TilesetBundle   mTilesets;
  ContextDelegate mContext;
  int32           mNextObjectId {1};
  int32           mNextLayerId {1};
  int32           mTileLayerSuffix {1};
  int32           mObjectLayerSuffix {1};
  int32           mGroupLayerSuffix {1};
};

}  // namespace tactile
