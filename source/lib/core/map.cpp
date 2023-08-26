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

#include "map.hpp"

#include <utility>  // move, cmp_less

#include <fmt/format.h>

#include "common/util/functional.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/layer_visitor.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_format.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tile_pos.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "tactile/core/debug/error.hpp"

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
  ContextInfo context;  ///< Map context information.
  TileExtent map_size {5, 5};
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
  get_invisible_root().each(visitor);
}

void Map::add_row()
{
  ++mData->map_size.rows;
  each_tile_layer([](TileLayer& layer) { layer.add_row(); });
}

void Map::add_column()
{
  ++mData->map_size.cols;
  each_tile_layer([](TileLayer& layer) { layer.add_column(); });
}

void Map::remove_row()
{
  if (mData->map_size.rows > 1) {
    --mData->map_size.rows;
    each_tile_layer([](TileLayer& layer) { layer.remove_row(); });
  }
  else {
    throw Error {"Invalid row amount!"};
  }
}

void Map::remove_column()
{
  if (mData->map_size.cols > 1) {
    --mData->map_size.cols;
    each_tile_layer([](TileLayer& layer) { layer.remove_column(); });
  }
  else {
    throw Error {"Invalid column amount!"};
  }
}

void Map::resize(const TileExtent extent)
{
  mData->map_size.rows = require_that(extent.rows, [](const usize x) { return x > 0; });
  mData->map_size.cols = require_that(extent.cols, [](const usize x) { return x > 0; });

  each_tile_layer([=](TileLayer& layer) { layer.resize(extent); });
}

auto Map::fix_tiles() -> FixTilesResult
{
  FixTilesResult result;

  each_tile_layer([&, this](TileLayer& layer) {
    HashMap<TilePos, TileID> previous;

    const auto extent = get_extent();
    const auto& tileset_bundle = get_tileset_bundle();

    invoke_mn(extent.rows, extent.cols, [&](const usize row, const usize col) {
      const auto pos = TilePos::from(row, col);
      const auto tile_id = layer.tile_at(pos);

      if (tile_id.has_value() && tile_id != kEmptyTile &&
          !tileset_bundle.is_valid_tile(*tile_id)) {
        previous[pos] = *tile_id;
        layer.set_tile(pos, kEmptyTile);
      }
    });

    result[layer.get_uuid()] = std::move(previous);
  });

  return result;
}

void Map::add_layer(Shared<Layer> layer, const Maybe<UUID>& parent_id)
{
  const auto layer_id = layer->get_uuid();
  auto& root_layer = get_invisible_root();

  if (parent_id) {
    root_layer.add_layer(*parent_id, layer);
  }
  else {
    root_layer.add_layer(std::move(layer));
  }

  // Select the layer if was the first one to be added
  if (root_layer.layer_count() == 1) {
    mData->active_layer = layer_id;
  }
}

auto Map::add_layer(const LayerType type, const Maybe<UUID>& parent_id) -> UUID
{
  switch (type) {
    case LayerType::TileLayer:
      return add_tile_layer(parent_id);

    case LayerType::ObjectLayer:
      return add_object_layer(parent_id);

    case LayerType::GroupLayer:
      return add_group_layer(parent_id);

    default:
      throw Error {"Invalid layer type"};
  }
}

auto Map::add_tile_layer(const Maybe<UUID>& parent_id) -> UUID
{
  auto layer = std::make_shared<TileLayer>(get_extent());

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

void Map::insert_layer(Shared<Layer> layer, const usize local_index)
{
  const auto layer_id = layer->get_uuid();
  const auto parent_id = layer->get_parent();
  add_layer(std::move(layer), parent_id);
  get_invisible_root().set_layer_index(layer_id, local_index);
}

auto Map::remove_layer(const UUID& id) -> Shared<Layer>
{
  if (mData->active_layer == id) {
    mData->active_layer.reset();
  }

  return get_invisible_root().remove_layer(id);
}

auto Map::duplicate_layer(const UUID& id) -> Shared<Layer>
{
  auto new_layer = get_invisible_root().duplicate_layer(id);
  new_layer->set_meta_id(fetch_and_increment_next_layer_id());
  return new_layer;
}

void Map::select_layer(const UUID& id)
{
  if (get_invisible_root().find_layer(id)) {
    mData->active_layer = id;
  }
  else {
    throw Error {"Invalid layer identifier!"};
  }
}

auto Map::is_active_layer(const LayerType type) const -> bool
{
  if (const auto layer_id = get_active_layer_id()) {
    return get_invisible_root().get_layer(*layer_id).get_type() == type;
  }
  else {
    return false;
  }
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
  return pos.row() >= 0 &&  //
         pos.col() >= 0 &&  //
         std::cmp_less(pos.row(), get_extent().rows) &&
         std::cmp_less(pos.col(), get_extent().cols);
}

auto Map::is_stamp_randomizer_possible() const -> bool
{
  const auto& tileset_bundle = get_tileset_bundle();
  if (const auto tileset_id = tileset_bundle.get_active_tileset_id()) {
    const auto& tileset_ref = tileset_bundle.get_tileset_ref(*tileset_id);
    return !tileset_ref.is_single_tile_selected();
  }
  else {
    return false;
  }
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

auto Map::get_active_layer_id() const -> Maybe<UUID>
{
  return mData->active_layer;
}

auto Map::get_extent() const -> TileExtent
{
  return mData->map_size;
}

auto Map::get_tile_size() const -> const Int2&
{
  return mData->tile_size;
}

auto Map::get_tile_format() -> TileFormat&
{
  return mData->tile_format;
}

auto Map::get_tile_format() const -> const TileFormat&
{
  return mData->tile_format;
}

auto Map::get_invisible_root() -> GroupLayer&
{
  return mData->root_layer;
}

auto Map::get_invisible_root() const -> const GroupLayer&
{
  return mData->root_layer;
}

auto Map::get_tileset_bundle() -> TilesetBundle&
{
  return mData->tilesets;
}

auto Map::get_tileset_bundle() const -> const TilesetBundle&
{
  return mData->tilesets;
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

}  // namespace tactile
