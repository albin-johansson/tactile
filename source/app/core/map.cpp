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

#include "core/layer/layer_visitor.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile_pos.hpp"
#include "core/util/fmt.hpp"
#include "core/util/functional.hpp"
#include "core/util/string.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

using TileLayerVisitorFunc = Map::TileLayerVisitorFunc;

class TileLayerVisitor final : public LayerVisitor {
 public:
  explicit TileLayerVisitor(const TileLayerVisitorFunc& func)
      : mCallable {&func}
  {
  }

  void visit(TileLayer& layer) override { (*mCallable)(layer); }

 private:
  const TileLayerVisitorFunc* mCallable {};
};

}  // namespace

Map::Map()
{
  ctx().set_name("Map");
}

void Map::each_tile_layer(const std::function<void(TileLayer&)>& func)
{
  TileLayerVisitor visitor {func};
  mRootLayer.each(visitor);
}

void Map::add_row()
{
  ++mRowCount;
  each_tile_layer([](TileLayer& layer) { layer.add_row(); });
}

void Map::add_column()
{
  ++mColCount;
  each_tile_layer([](TileLayer& layer) { layer.add_column(); });
}

void Map::remove_row()
{
  if (mRowCount > 1) {
    --mRowCount;
    each_tile_layer([](TileLayer& layer) { layer.remove_row(); });
  }
  else {
    throw TactileError {"Invalid row amount!"};
  }
}

void Map::remove_column()
{
  if (mColCount > 1) {
    --mColCount;
    each_tile_layer([](TileLayer& layer) { layer.remove_column(); });
  }
  else {
    throw TactileError {"Invalid column amount!"};
  }
}

void Map::resize(const usize rows, const usize columns)
{
  mRowCount = require_that(rows, [](const usize x) { return x > 0; });
  mColCount = require_that(columns, [](const usize x) { return x > 0; });

  each_tile_layer([=](TileLayer& layer) { layer.resize(rows, columns); });
}

auto Map::fix_tiles() -> FixTilesResult
{
  FixTilesResult result;

  each_tile_layer([&, this](TileLayer& layer) {
    HashMap<TilePos, TileID> previous;

    invoke_mn(row_count(), column_count(), [&](const usize row, const usize col) {
      const auto pos = TilePos::from(row, col);
      const auto tile_id = layer.tile_at(pos);

      if (tile_id != empty_tile && !mTilesets.is_valid_tile(tile_id)) {
        previous[pos] = tile_id;
        layer.set_tile(pos, empty_tile);
      }
    });

    result[layer.get_uuid()] = std::move(previous);
  });

  return result;
}

void Map::add_layer(Shared<Layer> layer, const Maybe<UUID>& parent_id)
{
  const auto id = layer->get_uuid();

  if (parent_id) {
    mRootLayer.add(*parent_id, layer);
  }
  else {
    mRootLayer.add(std::move(layer));
  }

  // Select the layer if it's the first one to be added
  if (mRootLayer.size() == 1) {
    mActiveLayer = id;
  }
}

auto Map::add_tile_layer(const Maybe<UUID>& parent_id) -> UUID
{
  auto layer = std::make_shared<TileLayer>(mRowCount, mColCount);

  layer->set_meta_id(fetch_and_increment_next_layer_id());
  layer->ctx().set_name(format_str("Tile Layer {}", mTileLayerSuffix));
  ++mTileLayerSuffix;

  const auto id = layer->ctx().uuid();
  add_layer(std::move(layer), parent_id);

  return id;
}

auto Map::add_object_layer(const Maybe<UUID>& parent_id) -> UUID
{
  auto layer = std::make_shared<ObjectLayer>();

  layer->set_meta_id(fetch_and_increment_next_layer_id());
  layer->ctx().set_name(format_str("Object Layer {}", mObjectLayerSuffix));
  ++mObjectLayerSuffix;

  const auto id = layer->ctx().uuid();
  add_layer(std::move(layer), parent_id);

  return id;
}

auto Map::add_group_layer(const Maybe<UUID>& parent_id) -> UUID
{
  auto layer = std::make_shared<GroupLayer>();

  layer->set_meta_id(fetch_and_increment_next_layer_id());
  layer->ctx().set_name(format_str("Group Layer {}", mGroupLayerSuffix));
  ++mGroupLayerSuffix;

  const auto id = layer->ctx().uuid();
  add_layer(std::move(layer), parent_id);

  return id;
}

auto Map::remove_layer(const UUID& id) -> Shared<Layer>
{
  if (mActiveLayer == id) {
    mActiveLayer.reset();
  }
  return mRootLayer.remove(id);
}

auto Map::duplicate_layer(const UUID& id) -> Shared<Layer>
{
  auto layer = mRootLayer.duplicate(id);
  layer->set_meta_id(fetch_and_increment_next_layer_id());
  return layer;
}

void Map::select_layer(const UUID& id)
{
  if (mRootLayer.find(id)) {
    mActiveLayer = id;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto Map::is_active_layer(const LayerType type) const -> bool
{
  if (mActiveLayer) {
    return mRootLayer.at(*mActiveLayer).type() == type;
  }
  else {
    return false;
  }
}

auto Map::active_layer_id() const -> Maybe<UUID>
{
  return mActiveLayer;
}

auto Map::invisible_root() -> GroupLayer&
{
  return mRootLayer;
}

auto Map::invisible_root() const -> const GroupLayer&
{
  return mRootLayer;
}

auto Map::tileset_bundle() -> TilesetBundle&
{
  return mTilesets;
}

auto Map::tileset_bundle() const -> const TilesetBundle&
{
  return mTilesets;
}

void Map::set_tile_size(const Int2& size)
{
  mTileSize = size;
}

void Map::accept(ContextVisitor& visitor) const
{
  visitor.visit(*this);
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

auto Map::tile_size() const -> const Int2&
{
  return mTileSize;
}

auto Map::is_stamp_randomizer_possible() const -> bool
{
  if (const auto tileset_id = mTilesets.active_tileset_id()) {
    const auto& ref = mTilesets.get_ref(*tileset_id);
    return !ref.is_single_tile_selected();
  }
  else {
    return false;
  }
}

auto Map::ctx() -> ContextInfo&
{
  return mContext;
}

auto Map::ctx() const -> const ContextInfo&
{
  return mContext;
}

auto Map::get_uuid() const -> const UUID&
{
  return mContext.uuid();
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

auto Map::tile_format() -> TileFormat&
{
  return mTileFormat;
}

auto Map::tile_format() const -> const TileFormat&
{
  return mTileFormat;
}

}  // namespace tactile
