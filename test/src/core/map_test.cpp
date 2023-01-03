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

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "common/util/assoc.hpp"
#include "common/util/functional.hpp"
#include "core/helpers/map_builder.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/layer_visitor.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tileset.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/tile/tileset_info.hpp"

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

TEST_SUITE("Map")
{
  TEST_CASE("Defaults")
  {
    const Map map;
    const auto& root = map.invisible_root();

    REQUIRE("Map" == map.get_ctx().name());

    REQUIRE(5u == map.row_count());
    REQUIRE(5u == map.column_count());

    REQUIRE(0u == root.layer_count());
    REQUIRE(!map.active_layer_id().has_value());

    REQUIRE(1 == map.next_tile_layer_suffix());
    REQUIRE(1 == map.next_object_layer_suffix());
    REQUIRE(1 == map.next_group_layer_suffix());

    REQUIRE(32 == map.tile_size().x);
    REQUIRE(32 == map.tile_size().y);

    REQUIRE(map.tileset_bundle().empty());
  }

  TEST_CASE("add_row")
  {
    Map map;
    auto& root = map.invisible_root();

    map.add_row();
    REQUIRE(6u == map.row_count());
    REQUIRE(5u == map.column_count());

    const auto id = map.add_tile_layer();
    const auto& layer = root.get_tile_layer(id);

    REQUIRE(6u == layer.row_count());
    REQUIRE(5u == layer.column_count());
  }

  TEST_CASE("add_column")
  {
    Map map;
    auto& root = map.invisible_root();

    const auto id = map.add_tile_layer();
    const auto& layer = root.get_tile_layer(id);

    map.add_column();
    REQUIRE(5u == map.row_count());
    REQUIRE(6u == map.column_count());

    REQUIRE(5u == layer.row_count());
    REQUIRE(6u == layer.column_count());
  }

  TEST_CASE("remove_row")
  {
    Map map;
    map.remove_row();

    REQUIRE(4u == map.row_count());
    REQUIRE(5u == map.column_count());

    map.remove_row();
    map.remove_row();
    map.remove_row();

    REQUIRE(1u == map.row_count());
    REQUIRE(5u == map.column_count());

    REQUIRE_THROWS_AS(map.remove_row(), TactileError);
  }

  TEST_CASE("remove_column")
  {
    Map map;
    map.remove_column();

    REQUIRE(5u == map.row_count());
    REQUIRE(4u == map.column_count());

    map.remove_column();
    map.remove_column();
    map.remove_column();

    REQUIRE(5u == map.row_count());
    REQUIRE(1u == map.column_count());

    REQUIRE_THROWS_AS(map.remove_column(), TactileError);
  }

  TEST_CASE("resize")
  {
    MapLayerPreset preset;

    preset.map.resize(8, 3);

    REQUIRE(8u == preset.map.row_count());
    REQUIRE(3u == preset.map.column_count());

    auto& root = preset.map.invisible_root();
    auto& layer = root.get_tile_layer(preset.f);
    REQUIRE(8u == layer.row_count());
    REQUIRE(3u == layer.column_count());

    preset.map.resize(1, 1);

    REQUIRE(1u == preset.map.row_count());
    REQUIRE(1u == preset.map.column_count());

    REQUIRE(1u == layer.row_count());
    REQUIRE(1u == layer.column_count());

    REQUIRE_THROWS_AS(preset.map.resize(0, 1), TactileError);
    REQUIRE_THROWS_AS(preset.map.resize(1, 0), TactileError);
  }

  TEST_CASE("fix_tiles")
  {
    UUID layer_id;
    UUID tileset_id;

    auto document = test::MapBuilder::build()  //
                        .with_tile_layer(&layer_id)
                        .with_tileset(&tileset_id)
                        .result();
    auto map = document->get_map_ptr();
    auto& root = map->invisible_root();

    const auto& tileset_ref = map->tileset_bundle().get_tileset_ref(tileset_id);
    auto& layer = root.get_tile_layer(layer_id);

    // Valid
    layer.set_tile({0, 1}, tileset_ref.get_first_tile());
    layer.set_tile({4, 2}, 42);
    layer.set_tile({1, 3}, tileset_ref.get_last_tile());

    // Invalid
    layer.set_tile({0, 0}, -1);
    layer.set_tile({2, 1}, tileset_ref.get_last_tile() + 1);
    layer.set_tile({3, 2}, tileset_ref.get_last_tile() + 934);
    layer.set_tile({4, 4}, tileset_ref.get_first_tile() - 32);

    const auto result = map->fix_tiles();
    const auto& previous = lookup_in(result, layer_id);

    REQUIRE(4u == previous.size());
    REQUIRE(lookup_in(previous, TilePos {0, 0}) == -1);
    REQUIRE(lookup_in(previous, TilePos {2, 1}) == tileset_ref.get_last_tile() + 1);
    REQUIRE(lookup_in(previous, TilePos {3, 2}) == tileset_ref.get_last_tile() + 934);
    REQUIRE(lookup_in(previous, TilePos {4, 4}) == tileset_ref.get_first_tile() - 32);

    REQUIRE(42 == layer.tile_at({4, 2}));
    REQUIRE(tileset_ref.get_first_tile() == layer.tile_at({0, 1}));
    REQUIRE(tileset_ref.get_last_tile() == layer.tile_at({1, 3}));

    REQUIRE(empty_tile == layer.tile_at({0, 0}));
    REQUIRE(empty_tile == layer.tile_at({2, 1}));
    REQUIRE(empty_tile == layer.tile_at({3, 2}));
    REQUIRE(empty_tile == layer.tile_at({4, 4}));
  }

  TEST_CASE("set_tile_size")
  {
    Map map;
    map.set_tile_size({28, 45});
    REQUIRE(28 == map.tile_size().x);
    REQUIRE(45 == map.tile_size().y);
  }

  TEST_CASE("is_valid_position")
  {
    const Map map;
    const auto end = TilePos::from(map.row_count(), map.column_count());

    REQUIRE(!map.is_valid_position({-1, 0}));
    REQUIRE(!map.is_valid_position({0, -1}));
    REQUIRE(map.is_valid_position({0, 0}));
    REQUIRE(map.is_valid_position({2, 3}));
    REQUIRE(map.is_valid_position(end - TilePos {1, 1}));
    REQUIRE(!map.is_valid_position(end));
  }

  TEST_CASE("add_layer")
  {
    Map map;
    auto& root = map.invisible_root();

    auto t1 = std::make_shared<TileLayer>();
    auto t2 = std::make_shared<TileLayer>();
    auto o1 = std::make_shared<ObjectLayer>();
    auto g1 = std::make_shared<GroupLayer>();

    map.add_layer(t1);
    REQUIRE(1u == root.layer_count());
    REQUIRE(nullptr != root.find_tile_layer(t1->get_uuid()));
    REQUIRE(nullptr == root.find_object_layer(o1->get_uuid()));
    REQUIRE(!t1->get_parent().has_value());

    map.add_layer(o1);
    map.add_layer(g1);
    map.add_layer(t2, g1->get_uuid());

    REQUIRE(4u == root.layer_count());

    REQUIRE(nullptr != root.find_tile_layer(t1->get_uuid()));
    REQUIRE(nullptr != root.find_tile_layer(t2->get_uuid()));
    REQUIRE(nullptr != root.find_object_layer(o1->get_uuid()));
    REQUIRE(nullptr != root.find_group_layer(g1->get_uuid()));

    REQUIRE(g1->get_uuid() == t2->get_parent());
    REQUIRE(nothing == t1->get_parent());
    REQUIRE(nothing == o1->get_parent());
    REQUIRE(nothing == g1->get_parent());
  }

  TEST_CASE("add_tile_layer")
  {
    Map map;
    auto& root = map.invisible_root();

    const auto id = map.add_tile_layer();
    const auto* layer = root.find_tile_layer(id);

    REQUIRE(nullptr != layer);
    REQUIRE(nullptr == root.find_object_layer(id));
    REQUIRE(nullptr == root.find_group_layer(id));
    REQUIRE(1u == root.layer_count());

    REQUIRE(map.row_count() == layer->row_count());
    REQUIRE(map.column_count() == layer->column_count());
  }

  TEST_CASE("add_object_layer")
  {
    Map map;
    auto& root = map.invisible_root();

    const auto parent = map.add_group_layer();

    const auto id = map.add_object_layer(parent);
    const auto& layer = root.get_object_layer(id);

    REQUIRE(nullptr == root.find_tile_layer(id));
    REQUIRE(nullptr == root.find_group_layer(id));
    REQUIRE(2u == root.layer_count());

    REQUIRE(parent == layer.get_parent());
  }

  TEST_CASE("add_group_layer")
  {
    Map map;
    auto& root = map.invisible_root();

    const auto id = map.add_group_layer();
    const auto& layer = root.get_group_layer(id);

    REQUIRE(nullptr == root.find_tile_layer(id));
    REQUIRE(nullptr == root.find_object_layer(id));
    REQUIRE(1u == root.layer_count());

    REQUIRE(0u == layer.layer_count());
  }

  TEST_CASE("remove_layer")
  {
    MapLayerPreset preset;
    auto& root = preset.map.invisible_root();

    REQUIRE_NOTHROW(preset.map.remove_layer(make_uuid()));

    REQUIRE(nullptr != root.find_object_layer(preset.a));
    REQUIRE(7u == root.layer_count());

    preset.map.remove_layer(preset.a);

    REQUIRE(6u == root.layer_count());
    REQUIRE(nullptr == root.find_object_layer(preset.a));

    auto group = std::dynamic_pointer_cast<GroupLayer>(preset.map.remove_layer(preset.d));

    REQUIRE(3u == root.layer_count());
    REQUIRE(nullptr == root.find_group_layer(preset.d));
    REQUIRE(nullptr == root.find_object_layer(preset.e));
    REQUIRE(nullptr == root.find_tile_layer(preset.f));

    REQUIRE(2u == group->layer_count());
    REQUIRE(nullptr != group->find_object_layer(preset.e));
    REQUIRE(nullptr != group->find_tile_layer(preset.f));

    REQUIRE(nullptr != root.find_tile_layer(preset.c));
  }

  TEST_CASE("duplicate_layer")
  {
    MapLayerPreset preset;
    auto& root = preset.map.invisible_root();

    REQUIRE_THROWS_AS(preset.map.duplicate_layer(make_uuid()), TactileError);

    REQUIRE(7u == root.layer_count());
    REQUIRE(5u == root.find_group_layer(preset.b)->layer_count());
    REQUIRE(2u == root.find_group_layer(preset.d)->layer_count());

    const auto layer = preset.map.duplicate_layer(preset.d);

    REQUIRE(10u == root.layer_count());
    REQUIRE(8u == root.find_group_layer(preset.b)->layer_count());
    REQUIRE(2u == root.find_group_layer(preset.d)->layer_count());
    REQUIRE(2u == root.find_group_layer(layer->get_uuid())->layer_count());
  }

  TEST_CASE("select_layer")
  {
    MapLayerPreset preset;
    REQUIRE(preset.map.active_layer_id().has_value());
    REQUIRE_THROWS_AS(preset.map.select_layer(make_uuid()), TactileError);

    preset.map.select_layer(preset.e);
    REQUIRE(preset.e == preset.map.active_layer_id());
    REQUIRE(preset.map.is_active_layer(LayerType::ObjectLayer));
    REQUIRE(!preset.map.is_active_layer(LayerType::TileLayer));
    REQUIRE(!preset.map.is_active_layer(LayerType::GroupLayer));

    preset.map.select_layer(preset.b);
    REQUIRE(preset.b == preset.map.active_layer_id());
    REQUIRE(preset.map.is_active_layer(LayerType::GroupLayer));
    REQUIRE(!preset.map.is_active_layer(LayerType::TileLayer));
    REQUIRE(!preset.map.is_active_layer(LayerType::ObjectLayer));

    preset.map.select_layer(preset.f);
    REQUIRE(preset.f == preset.map.active_layer_id());
    REQUIRE(preset.map.is_active_layer(LayerType::TileLayer));
    REQUIRE(!preset.map.is_active_layer(LayerType::ObjectLayer));
    REQUIRE(!preset.map.is_active_layer(LayerType::GroupLayer));
  }
}

}  // namespace tactile::test
