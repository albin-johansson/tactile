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

#include "core/map.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "core/layer/layer_visitor.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tileset.hpp"
#include "core/tile/tileset_info.hpp"
#include "core/util/assoc.hpp"
#include "core/util/functional.hpp"
#include "misc/panic.hpp"

namespace tactile::test {
namespace {

// - a      (object layer)
// - b      (group layer)
//   - c    (tile layer)
//   - d    (group layer)
//     - e  (object layer)
//     - f  (tile layer)
//   - g    (tile layer)
struct MapLayerPreset final {
  Map map;
  UUID a;
  UUID b;
  UUID c;
  UUID d;
  UUID e;
  UUID f;
  UUID g;

  MapLayerPreset()
      : a {map.add_object_layer()},
        b {map.add_group_layer()},
        c {map.add_tile_layer(b)},
        d {map.add_group_layer(b)},
        e {map.add_object_layer(d)},
        f {map.add_tile_layer(d)},
        g {map.add_tile_layer(b)}
  {
  }
};

}  // namespace

TEST(Map, Defaults)
{
  const Map map;
  const auto& root = map.invisible_root();

  ASSERT_EQ("Map", map.ctx().name());

  ASSERT_EQ(5, map.row_count());
  ASSERT_EQ(5, map.column_count());

  ASSERT_EQ(0, root.size());
  ASSERT_FALSE(map.active_layer_id().has_value());

  ASSERT_EQ(1, map.next_tile_layer_suffix());
  ASSERT_EQ(1, map.next_object_layer_suffix());
  ASSERT_EQ(1, map.next_group_layer_suffix());

  ASSERT_EQ(32, map.tile_size().x);
  ASSERT_EQ(32, map.tile_size().y);

  ASSERT_TRUE(map.tileset_bundle().empty());
}

TEST(Map, AddRow)
{
  Map map;
  auto& root = map.invisible_root();

  map.add_row();
  ASSERT_EQ(6, map.row_count());
  ASSERT_EQ(5, map.column_count());

  const auto id = map.add_tile_layer();
  const auto& layer = root.tile_layer(id);

  ASSERT_EQ(6, layer.row_count());
  ASSERT_EQ(5, layer.column_count());
}

TEST(Map, AddColumn)
{
  Map map;
  auto& root = map.invisible_root();

  const auto id = map.add_tile_layer();
  const auto& layer = root.tile_layer(id);

  map.add_column();
  ASSERT_EQ(5, map.row_count());
  ASSERT_EQ(6, map.column_count());

  ASSERT_EQ(5, layer.row_count());
  ASSERT_EQ(6, layer.column_count());
}

TEST(Map, RemoveRow)
{
  Map map;
  map.remove_row();

  ASSERT_EQ(4, map.row_count());
  ASSERT_EQ(5, map.column_count());

  map.remove_row();
  map.remove_row();
  map.remove_row();

  ASSERT_EQ(1, map.row_count());
  ASSERT_EQ(5, map.column_count());

  ASSERT_THROW(map.remove_row(), TactileError);
}

TEST(Map, RemoveColumn)
{
  Map map;
  map.remove_column();

  ASSERT_EQ(5, map.row_count());
  ASSERT_EQ(4, map.column_count());

  map.remove_column();
  map.remove_column();
  map.remove_column();

  ASSERT_EQ(5, map.row_count());
  ASSERT_EQ(1, map.column_count());

  ASSERT_THROW(map.remove_column(), TactileError);
}

TEST(Map, Resize)
{
  MapLayerPreset preset;

  preset.map.resize(8, 3);

  ASSERT_EQ(8, preset.map.row_count());
  ASSERT_EQ(3, preset.map.column_count());

  auto& root = preset.map.invisible_root();
  auto& layer = root.tile_layer(preset.f);
  ASSERT_EQ(8, layer.row_count());
  ASSERT_EQ(3, layer.column_count());

  preset.map.resize(1, 1);

  ASSERT_EQ(1, preset.map.row_count());
  ASSERT_EQ(1, preset.map.column_count());

  ASSERT_EQ(1, layer.row_count());
  ASSERT_EQ(1, layer.column_count());

  ASSERT_THROW(preset.map.resize(0, 1), TactileError);
  ASSERT_THROW(preset.map.resize(1, 0), TactileError);
}

TEST(Map, FixTiles)
{
  UUID layer_id;
  UUID tileset_id;

  auto document = test::MapBuilder::build()  //
                      .with_tile_layer(&layer_id)
                      .with_tileset(&tileset_id)
                      .result();
  auto map = document->get_map_ptr();
  auto& root = map->invisible_root();

  const auto& tileset_ref = map->tileset_bundle().get_ref(tileset_id);
  auto& layer = root.tile_layer(layer_id);

  // Valid
  layer.set_tile({0, 1}, tileset_ref.first_tile());
  layer.set_tile({4, 2}, 42);
  layer.set_tile({1, 3}, tileset_ref.last_tile());

  // Invalid
  layer.set_tile({0, 0}, -1);
  layer.set_tile({2, 1}, tileset_ref.last_tile() + 1);
  layer.set_tile({3, 2}, tileset_ref.last_tile() + 934);
  layer.set_tile({4, 4}, tileset_ref.first_tile() - 32);

  const auto result = map->fix_tiles();
  const auto& previous = lookup_in(result, layer_id);

  ASSERT_EQ(4, previous.size());
  ASSERT_EQ(lookup_in(previous, TilePos {0, 0}), -1);
  ASSERT_EQ(lookup_in(previous, TilePos {2, 1}), tileset_ref.last_tile() + 1);
  ASSERT_EQ(lookup_in(previous, TilePos {3, 2}), tileset_ref.last_tile() + 934);
  ASSERT_EQ(lookup_in(previous, TilePos {4, 4}), tileset_ref.first_tile() - 32);

  ASSERT_EQ(tileset_ref.first_tile(), layer.tile_at({0, 1}));
  ASSERT_EQ(42, layer.tile_at({4, 2}));
  ASSERT_EQ(tileset_ref.last_tile(), layer.tile_at({1, 3}));

  ASSERT_EQ(empty_tile, layer.tile_at({0, 0}));
  ASSERT_EQ(empty_tile, layer.tile_at({2, 1}));
  ASSERT_EQ(empty_tile, layer.tile_at({3, 2}));
  ASSERT_EQ(empty_tile, layer.tile_at({4, 4}));
}

TEST(Map, SetTileSize)
{
  Map map;
  map.set_tile_size({28, 45});
  ASSERT_EQ(28, map.tile_size().x);
  ASSERT_EQ(45, map.tile_size().y);
}

TEST(Map, IsValidPosition)
{
  const Map map;
  const auto end = TilePos::from(map.row_count(), map.column_count());

  ASSERT_FALSE(map.is_valid_position({-1, 0}));
  ASSERT_FALSE(map.is_valid_position({0, -1}));
  ASSERT_TRUE(map.is_valid_position({0, 0}));
  ASSERT_TRUE(map.is_valid_position({2, 3}));
  ASSERT_TRUE(map.is_valid_position(end - TilePos {1, 1}));
  ASSERT_FALSE(map.is_valid_position(end));
}

TEST(Map, AddLayer)
{
  Map map;
  auto& root = map.invisible_root();

  auto t1 = std::make_shared<TileLayer>();
  auto t2 = std::make_shared<TileLayer>();
  auto o1 = std::make_shared<ObjectLayer>();
  auto g1 = std::make_shared<GroupLayer>();

  map.add_layer(t1);
  ASSERT_EQ(1, root.size());
  ASSERT_NE(nullptr, root.as_tile_layer(t1->uuid()));
  ASSERT_EQ(nullptr, root.as_object_layer(o1->uuid()));
  ASSERT_FALSE(t1->parent().has_value());

  map.add_layer(o1);
  map.add_layer(g1);
  map.add_layer(t2, g1->uuid());

  ASSERT_EQ(4, root.size());

  ASSERT_NE(nullptr, root.as_tile_layer(t1->uuid()));
  ASSERT_NE(nullptr, root.as_tile_layer(t2->uuid()));
  ASSERT_NE(nullptr, root.as_object_layer(o1->uuid()));
  ASSERT_NE(nullptr, root.as_group_layer(g1->uuid()));

  ASSERT_EQ(g1->uuid(), t2->parent());
  ASSERT_EQ(nothing, t1->parent());
  ASSERT_EQ(nothing, o1->parent());
  ASSERT_EQ(nothing, g1->parent());
}

TEST(Map, AddTileLayer)
{
  Map map;
  auto& root = map.invisible_root();

  const auto id = map.add_tile_layer();
  const auto* layer = root.as_tile_layer(id);

  ASSERT_NE(nullptr, layer);
  ASSERT_EQ(nullptr, root.as_object_layer(id));
  ASSERT_EQ(nullptr, root.as_group_layer(id));
  ASSERT_EQ(1, root.size());

  ASSERT_EQ(map.row_count(), layer->row_count());
  ASSERT_EQ(map.column_count(), layer->column_count());
}

TEST(Map, AddObjectLayer)
{
  Map map;
  auto& root = map.invisible_root();

  const auto parent = map.add_group_layer();

  const auto id = map.add_object_layer(parent);
  const auto& layer = root.object_layer(id);

  ASSERT_EQ(nullptr, root.as_tile_layer(id));
  ASSERT_EQ(nullptr, root.as_group_layer(id));
  ASSERT_EQ(2, root.size());

  ASSERT_EQ(parent, layer.parent());
}

TEST(Map, AddGroupLayer)
{
  Map map;
  auto& root = map.invisible_root();

  const auto id = map.add_group_layer();
  const auto& layer = root.group_layer(id);

  ASSERT_EQ(nullptr, root.as_tile_layer(id));
  ASSERT_EQ(nullptr, root.as_object_layer(id));
  ASSERT_EQ(1, root.size());

  ASSERT_EQ(0, layer.size());
}

TEST(Map, RemoveLayer)
{
  MapLayerPreset preset;
  auto& root = preset.map.invisible_root();

  ASSERT_THROW(preset.map.remove_layer(make_uuid()), TactileError);

  ASSERT_NE(nullptr, root.as_object_layer(preset.a));
  ASSERT_EQ(7, root.size());

  preset.map.remove_layer(preset.a);

  ASSERT_EQ(6, root.size());
  ASSERT_EQ(nullptr, root.as_object_layer(preset.a));

  auto group = std::dynamic_pointer_cast<GroupLayer>(preset.map.remove_layer(preset.d));

  ASSERT_EQ(3, root.size());
  ASSERT_EQ(nullptr, root.as_group_layer(preset.d));
  ASSERT_EQ(nullptr, root.as_object_layer(preset.e));
  ASSERT_EQ(nullptr, root.as_tile_layer(preset.f));

  ASSERT_EQ(2, group->size());
  ASSERT_NE(nullptr, group->as_object_layer(preset.e));
  ASSERT_NE(nullptr, group->as_tile_layer(preset.f));

  ASSERT_NE(nullptr, root.as_tile_layer(preset.c));
}

TEST(Map, DuplicateLayer)
{
  MapLayerPreset preset;
  auto& root = preset.map.invisible_root();

  ASSERT_THROW(preset.map.duplicate_layer(make_uuid()), TactileError);

  ASSERT_EQ(7, root.size());
  ASSERT_EQ(5, root.as_group_layer(preset.b)->size());
  ASSERT_EQ(2, root.as_group_layer(preset.d)->size());

  const auto layer = preset.map.duplicate_layer(preset.d);

  ASSERT_EQ(10, root.size());
  ASSERT_EQ(8, root.as_group_layer(preset.b)->size());
  ASSERT_EQ(2, root.as_group_layer(preset.d)->size());
  ASSERT_EQ(2, root.as_group_layer(layer->uuid())->size());
}

TEST(Map, SelectLayer)
{
  MapLayerPreset preset;
  ASSERT_TRUE(preset.map.active_layer_id().has_value());
  ASSERT_THROW(preset.map.select_layer(make_uuid()), TactileError);

  preset.map.select_layer(preset.e);
  ASSERT_EQ(preset.e, preset.map.active_layer_id());
  ASSERT_TRUE(preset.map.is_active_layer(LayerType::ObjectLayer));
  ASSERT_FALSE(preset.map.is_active_layer(LayerType::TileLayer));
  ASSERT_FALSE(preset.map.is_active_layer(LayerType::GroupLayer));

  preset.map.select_layer(preset.b);
  ASSERT_EQ(preset.b, preset.map.active_layer_id());
  ASSERT_TRUE(preset.map.is_active_layer(LayerType::GroupLayer));
  ASSERT_FALSE(preset.map.is_active_layer(LayerType::TileLayer));
  ASSERT_FALSE(preset.map.is_active_layer(LayerType::ObjectLayer));

  preset.map.select_layer(preset.f);
  ASSERT_EQ(preset.f, preset.map.active_layer_id());
  ASSERT_TRUE(preset.map.is_active_layer(LayerType::TileLayer));
  ASSERT_FALSE(preset.map.is_active_layer(LayerType::ObjectLayer));
  ASSERT_FALSE(preset.map.is_active_layer(LayerType::GroupLayer));
}

}  // namespace tactile::test
