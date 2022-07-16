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

#include "map.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "core/common/functional.hpp"
#include "core/ctx/context_visitor.hpp"
#include "core/layer/layer_visitor.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile_pos.hpp"
#include "misc/panic.hpp"

namespace tactile {

Map::Map()
{
  set_name("Map");
}

void Map::add_row()
{
  struct AddRowVisitor final : ILayerVisitor
  {
    void visit(TileLayer& layer) override
    {
      layer.add_row();
    }
  };

  ++mRowCount;

  AddRowVisitor visitor;
  mRootLayer.each(visitor);
}

void Map::add_column()
{
  struct AddColumnVisitor final : ILayerVisitor
  {
    void visit(TileLayer& layer) override
    {
      layer.add_column();
    }
  };

  ++mColCount;

  AddColumnVisitor visitor;
  mRootLayer.each(visitor);
}

void Map::remove_row()
{
  struct RemoveRowVisitor final : ILayerVisitor
  {
    void visit(TileLayer& layer) override
    {
      layer.remove_row();
    }
  };

  if (mRowCount > 1) {
    --mRowCount;

    RemoveRowVisitor visitor;
    mRootLayer.each(visitor);
  }
  else {
    throw TactileError {"Invalid row amount!"};
  }
}

void Map::remove_column()
{
  struct RemoveColumnVisitor final : ILayerVisitor
  {
    void visit(TileLayer& layer) override
    {
      layer.remove_column();
    }
  };

  if (mColCount > 1) {
    --mColCount;

    RemoveColumnVisitor visitor;
    mRootLayer.each(visitor);
  }
  else {
    throw TactileError {"Invalid column amount!"};
  }
}

void Map::resize(const usize rows, const usize columns)
{
  struct ResizeLayerVisitor final : ILayerVisitor
  {
    usize new_rows {};
    usize new_cols {};

    void visit(TileLayer& layer) override
    {
      layer.resize(new_rows, new_cols);
    }
  };

  mRowCount = require_that(rows, [](usize x) { return x > 0; });
  mColCount = require_that(columns, [](usize x) { return x > 0; });

  ResizeLayerVisitor visitor;
  visitor.new_rows = rows;
  visitor.new_cols = columns;

  mRootLayer.each(visitor);
}

auto Map::fix_tiles() -> FixTilesResult
{
  struct FixTilesVisitor final : ILayerVisitor
  {
    Map*           self {};
    FixTilesResult result;

    void visit(TileLayer& layer) override
    {
      HashMap<TilePos, TileID> previous;

      invoke_mn(self->row_count(),
                self->column_count(),
                [&](const usize row, const usize col) {
                  const auto pos = TilePos::from(row, col);
                  const auto tileId = layer.tile_at(pos);
                  if (tileId != empty_tile && !self->mTilesets.is_valid_tile(tileId)) {
                    previous[pos] = tileId;
                    layer.set_tile(pos, empty_tile);
                  }
                });

      result[layer.get_uuid()] = std::move(previous);
    }
  };

  FixTilesVisitor visitor;
  visitor.self = this;
  mRootLayer.each(visitor);

  return visitor.result;
}

auto Map::add_layer(Shared<ILayer> layer, const Maybe<UUID>& parentId)

    -> UUID
{
  const auto id = layer->get_uuid();

  if (parentId) {
    mRootLayer.add_layer(*parentId, std::move(layer));
  }
  else {
    mRootLayer.add_layer(std::move(layer));
  }

  /* Select the layer if it's the first one to be added */
  if (layer_count() == 1) {
    mActiveLayer = id;
  }

  return id;
}

auto Map::add_tile_layer(const Maybe<UUID>& parentId) -> UUID
{
  auto layer = TileLayer::make(mRowCount, mColCount);

  layer->set_meta_id(fetch_and_increment_next_layer_id());
  layer->set_name(fmt::format("Tile Layer {}", mTileLayerSuffix));
  ++mTileLayerSuffix;

  return add_layer(std::move(layer), parentId);
}

auto Map::add_object_layer(const Maybe<UUID>& parentId) -> UUID
{
  auto layer = ObjectLayer::make();

  layer->set_meta_id(fetch_and_increment_next_layer_id());
  layer->set_name(fmt::format("Object Layer {}", mObjectLayerSuffix));
  ++mObjectLayerSuffix;

  return add_layer(std::move(layer), parentId);
}

auto Map::add_group_layer(const Maybe<UUID>& parentId) -> UUID
{
  auto layer = GroupLayer::make();

  layer->set_meta_id(fetch_and_increment_next_layer_id());
  layer->set_name(fmt::format("Group Layer {}", mGroupLayerSuffix));
  ++mGroupLayerSuffix;

  return add_layer(std::move(layer), parentId);
}

void Map::visit_layers(IConstLayerVisitor& visitor) const
{
  mRootLayer.each(visitor);
}

void Map::visit_layers(const VisitorFunc& visitor) const
{
  mRootLayer.each(visitor);
}

auto Map::remove_layer(const UUID& id) -> Shared<ILayer>
{
  if (mActiveLayer == id) {
    mActiveLayer.reset();
  }
  return mRootLayer.remove_layer(id);
}

auto Map::duplicate_layer(const UUID& id) -> Shared<ILayer>
{
  auto layer = mRootLayer.duplicate_layer(id);
  layer->set_meta_id(fetch_and_increment_next_layer_id());
  return layer;
}

void Map::move_layer_up(const UUID& id)
{
  mRootLayer.move_layer_up(id);
}

void Map::move_layer_down(const UUID& id)
{
  mRootLayer.move_layer_down(id);
}

void Map::select_layer(const UUID& id)
{
  if (mRootLayer.find_layer(id)) {
    mActiveLayer = id;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

void Map::set_layer_index(const UUID& id, const usize index)
{
  mRootLayer.set_layer_index(id, index);
}

auto Map::local_layer_index(const UUID& id) const -> usize
{
  return mRootLayer.get_local_index(id);
}

auto Map::can_move_layer_up(const UUID& id) const -> bool
{
  return mRootLayer.can_move_layer_up(id);
}

auto Map::can_move_layer_down(const UUID& id) const -> bool
{
  return mRootLayer.can_move_layer_down(id);
}

auto Map::is_active_layer(const LayerType type) const -> bool
{
  if (mActiveLayer) {
    return mRootLayer.view_layer(*mActiveLayer).get_type() == type;
  }
  else {
    return false;
  }
}

auto Map::layer_count() const -> usize
{
  return mRootLayer.layer_count();
}

auto Map::active_layer_id() const -> Maybe<UUID>
{
  return mActiveLayer;
}

auto Map::view_layer(const UUID& id) -> ILayer&
{
  return mRootLayer.view_layer(id);
}

auto Map::view_layer(const UUID& id) const -> const ILayer&
{
  return mRootLayer.view_layer(id);
}

auto Map::view_tile_layer(const UUID& id) -> TileLayer&
{
  return mRootLayer.view_tile_layer(id);
}

auto Map::view_tile_layer(const UUID& id) const -> const TileLayer&
{
  return mRootLayer.view_tile_layer(id);
}

auto Map::view_object_layer(const UUID& id) -> ObjectLayer&
{
  return mRootLayer.view_object_layer(id);
}

auto Map::view_object_layer(const UUID& id) const -> const ObjectLayer&
{
  return mRootLayer.view_object_layer(id);
}

auto Map::view_group_layer(const UUID& id) -> GroupLayer&
{
  return mRootLayer.view_group_layer(id);
}

auto Map::find_tile_layer(const UUID& id) -> TileLayer*
{
  return mRootLayer.find_tile_layer(id);
}

auto Map::find_tile_layer(const UUID& id) const -> const TileLayer*
{
  return mRootLayer.find_tile_layer(id);
}

auto Map::find_object_layer(const UUID& id) const -> const ObjectLayer*
{
  return mRootLayer.find_object_layer(id);
}

auto Map::find_group_layer(const UUID& id) const -> const GroupLayer*
{
  return mRootLayer.find_group_layer(id);
}

auto Map::get_layer(const UUID& id) -> Shared<ILayer>
{
  return mRootLayer.get_layer(id);
}

void Map::attach_tileset(Shared<Tileset> tileset,
                         const TileID    firstTileId,
                         const bool      embedded)
{
  mTilesets.attach_tileset(std::move(tileset), firstTileId, embedded);
}

void Map::attach_tileset(Shared<Tileset> tileset, const bool embedded)
{
  mTilesets.attach_tileset(std::move(tileset), embedded);
}

void Map::detach_tileset(const UUID& id)
{
  mTilesets.detach_tileset(id);
}

void Map::select_tileset(const UUID& id)
{
  mTilesets.select_tileset(id);
}

auto Map::get_tilesets() -> TilesetBundle&
{
  return mTilesets;
}

auto Map::get_tilesets() const -> const TilesetBundle&
{
  return mTilesets;
}

void Map::set_tile_size(const Vector2i& size)
{
  mTileSize = size;
}

void Map::accept(IContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void Map::set_name(std::string name)
{
  mContext.set_name(std::move(name));
}

auto Map::is_valid_position(const TilePos& pos) const -> bool
{
  return pos.row() >= 0 &&          //
         pos.col() >= 0 &&          //
         pos.urow() < mRowCount &&  //
         pos.ucol() < mColCount;
}

auto Map::row_count() const -> usize
{
  return mRowCount;
}

auto Map::column_count() const -> usize
{
  return mColCount;
}

auto Map::tile_size() const -> const Vector2i&
{
  return mTileSize;
}

auto Map::is_stamp_randomizer_possible() const -> bool
{
  if (const auto tilesetId = mTilesets.active_tileset_id()) {
    const auto& ref = mTilesets.get_ref(*tilesetId);
    return !ref.is_single_tile_selected();
  }
  else {
    return false;
  }
}

auto Map::get_props() -> PropertyBundle&
{
  return mContext.get_props();
}

auto Map::get_props() const -> const PropertyBundle&
{
  return mContext.get_props();
}

auto Map::get_comps() -> ComponentBundle&
{
  return mContext.get_comps();
}

auto Map::get_comps() const -> const ComponentBundle&
{
  return mContext.get_comps();
}

auto Map::get_uuid() const -> const UUID&
{
  return mId;
}

auto Map::get_name() const -> const std::string&
{
  return mContext.get_name();
}

auto Map::fetch_and_increment_next_object_id() -> int32
{
  return mNextObjectId++;
}

auto Map::fetch_and_increment_next_layer_id() -> int32
{
  return mNextLayerId++;
}

void Map::set_next_object_id(const int32 id)
{
  mNextObjectId = id;
}

void Map::set_next_layer_id(const int32 id)
{
  mNextLayerId = id;
}

auto Map::next_object_id() const -> int32
{
  return mNextObjectId;
}

auto Map::next_layer_id() const -> int32
{
  return mNextLayerId;
}

auto Map::next_tile_layer_suffix() const -> int32
{
  return mTileLayerSuffix;
}

auto Map::next_object_layer_suffix() const -> int32
{
  return mObjectLayerSuffix;
}

auto Map::next_group_layer_suffix() const -> int32
{
  return mGroupLayerSuffix;
}

}  // namespace tactile
