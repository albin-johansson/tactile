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

#include "core/layer/group_layer.hpp"
#include "core/layer/layer_visitor.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_format.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tile_pos.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/util/functional.hpp"
#include "debug/panic.hpp"

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

struct Map::MapData final {
  ContextInfo context;      ///< Map context information.
  usize row_count {5};      ///< Amount of rows.
  usize col_count {5};      ///< Amount of columns.
  Int2 tile_size {32, 32};  ///< Logical size of all tiles.

  GroupLayer root_layer;     ///< Invisible root layer.
  Maybe<UUID> active_layer;  ///< The selected layer.

  TilesetBundle tilesets;  ///< The associated tilesets.

  int32 next_object_id {1};  ///< Next available object identifier.
  int32 next_layer_id {1};   ///< Next available layer identifier.

  int32 tile_layer_suffix {1};    ///< Next tile layer suffix for naming purposes.
  int32 object_layer_suffix {1};  ///< Next object layer suffix for naming purposes.
  int32 group_layer_suffix {1};   ///< Next group layer suffix for naming purposes.

  TileFormat tile_format;  ///< The tile format used by the map in save files.
};

Map::Map()
    : mData {std::make_unique<MapData>()}
{
  get_ctx().set_name("Map");
}

Map::~Map() noexcept = default;

void Map::each_tile_layer(const TileLayerVisitorFunc& func)
{
  TileLayerVisitor visitor {func};
  invisible_root().each(visitor);
}

void Map::add_row()
{
  ++mData->row_count;
  each_tile_layer([](TileLayer& layer) { layer.add_row(); });
}

void Map::add_column()
{
  ++mData->col_count;
  each_tile_layer([](TileLayer& layer) { layer.add_column(); });
}

void Map::remove_row()
{
  if (mData->row_count > 1) {
    --mData->row_count;
    each_tile_layer([](TileLayer& layer) { layer.remove_row(); });
  }
  else {
    throw TactileError {"Invalid row amount!"};
  }
}

void Map::remove_column()
{
  if (mData->col_count > 1) {
    --mData->col_count;
    each_tile_layer([](TileLayer& layer) { layer.remove_column(); });
  }
  else {
    throw TactileError {"Invalid column amount!"};
  }
}

void Map::resize(const usize rows, const usize columns)
{
  mData->row_count = require_that(rows, [](const usize x) { return x > 0; });
  mData->col_count = require_that(columns, [](const usize x) { return x > 0; });

  each_tile_layer([=](TileLayer& layer) { layer.resize(rows, columns); });
}

auto Map::fix_tiles() -> FixTilesResult
{
  FixTilesResult result;

  each_tile_layer([&, this](TileLayer& layer) {
    HashMap<TilePos, TileID> previous;
    const auto& tilesets = tileset_bundle();

    invoke_mn(row_count(), column_count(), [&](const usize row, const usize col) {
      const auto pos = TilePos::from(row, col);
      const auto tile_id = layer.tile_at(pos);

      if (tile_id != empty_tile && !tilesets.is_valid_tile(tile_id)) {
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
    invisible_root().add(*parent_id, layer);
  }
  else {
    invisible_root().add(std::move(layer));
  }

  // Select the layer if it's the first one to be added
  if (invisible_root().size() == 1) {
    mData->active_layer = id;
  }
}

auto Map::add_tile_layer(const Maybe<UUID>& parent_id) -> UUID
{
  auto layer = std::make_shared<TileLayer>(row_count(), column_count());

  layer->set_meta_id(fetch_and_increment_next_layer_id());
  layer->get_ctx().set_name(fmt::format("Tile Layer {}", mData->tile_layer_suffix));
  ++mData->tile_layer_suffix;

  const auto id = layer->get_ctx().get_uuid();
  add_layer(std::move(layer), parent_id);

  return id;
}

auto Map::add_object_layer(const Maybe<UUID>& parent_id) -> UUID
{
  auto layer = std::make_shared<ObjectLayer>();

  layer->set_meta_id(fetch_and_increment_next_layer_id());
  layer->get_ctx().set_name(fmt::format("Object Layer {}", mData->object_layer_suffix));
  ++mData->object_layer_suffix;

  const auto id = layer->get_ctx().get_uuid();
  add_layer(std::move(layer), parent_id);

  return id;
}

auto Map::add_group_layer(const Maybe<UUID>& parent_id) -> UUID
{
  auto layer = std::make_shared<GroupLayer>();

  layer->set_meta_id(fetch_and_increment_next_layer_id());
  layer->get_ctx().set_name(fmt::format("Group Layer {}", mData->group_layer_suffix));
  ++mData->group_layer_suffix;

  const auto id = layer->get_ctx().get_uuid();
  add_layer(std::move(layer), parent_id);

  return id;
}

auto Map::remove_layer(const UUID& id) -> Shared<Layer>
{
  if (mData->active_layer == id) {
    mData->active_layer.reset();
  }

  return invisible_root().remove(id);
}

auto Map::duplicate_layer(const UUID& id) -> Shared<Layer>
{
  auto layer = invisible_root().duplicate(id);
  layer->set_meta_id(fetch_and_increment_next_layer_id());
  return layer;
}

void Map::select_layer(const UUID& id)
{
  if (invisible_root().find_layer(id)) {
    mData->active_layer = id;
  }
  else {
    throw TactileError {"Invalid layer identifier!"};
  }
}

auto Map::is_active_layer(const LayerType type) const -> bool
{
  if (const auto id = active_layer_id()) {
    return invisible_root().get_layer(*id).type() == type;
  }
  else {
    return false;
  }
}

auto Map::active_layer_id() const -> Maybe<UUID>
{
  return mData->active_layer;
}

auto Map::invisible_root() -> GroupLayer&
{
  return mData->root_layer;
}

auto Map::invisible_root() const -> const GroupLayer&
{
  return mData->root_layer;
}

auto Map::tileset_bundle() -> TilesetBundle&
{
  return mData->tilesets;
}

auto Map::tileset_bundle() const -> const TilesetBundle&
{
  return mData->tilesets;
}

void Map::set_tile_size(const Int2& size)
{
  mData->tile_size = size;
}

void Map::accept(ContextVisitor& visitor) const
{
  visitor.visit(*this);
}

auto Map::is_valid_position(const TilePos& pos) const -> bool
{
  return pos.row() >= 0 &&            //
         pos.col() >= 0 &&            //
         pos.urow() < row_count() &&  //
         pos.ucol() < column_count();
}

auto Map::row_count() const -> usize
{
  return mData->row_count;
}

auto Map::column_count() const -> usize
{
  return mData->col_count;
}

auto Map::tile_size() const -> const Int2&
{
  return mData->tile_size;
}

auto Map::is_stamp_randomizer_possible() const -> bool
{
  const auto& tilesets = tileset_bundle();
  if (const auto tileset_id = tilesets.active_tileset_id()) {
    const auto& ref = tilesets.get_ref(*tileset_id);
    return !ref.is_single_tile_selected();
  }
  else {
    return false;
  }
}

auto Map::get_ctx() -> ContextInfo&
{
  return mData->context;
}

auto Map::get_ctx() const -> const ContextInfo&
{
  return mData->context;
}

auto Map::get_uuid() const -> const UUID&
{
  return get_ctx().get_uuid();
}

auto Map::fetch_and_increment_next_object_id() -> int32
{
  return mData->next_object_id++;
}

auto Map::fetch_and_increment_next_layer_id() -> int32
{
  return mData->next_layer_id++;
}

void Map::set_next_object_id(const int32 id)
{
  mData->next_object_id = id;
}

void Map::set_next_layer_id(const int32 id)
{
  mData->next_layer_id = id;
}

auto Map::next_object_id() const -> int32
{
  return mData->next_object_id;
}

auto Map::next_layer_id() const -> int32
{
  return mData->next_layer_id;
}

auto Map::next_tile_layer_suffix() const -> int32
{
  return mData->tile_layer_suffix;
}

auto Map::next_object_layer_suffix() const -> int32
{
  return mData->object_layer_suffix;
}

auto Map::next_group_layer_suffix() const -> int32
{
  return mData->group_layer_suffix;
}

auto Map::tile_format() -> TileFormat&
{
  return mData->tile_format;
}

auto Map::tile_format() const -> const TileFormat&
{
  return mData->tile_format;
}

}  // namespace tactile
