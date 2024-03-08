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

#include "core/layer/group_layer.hpp"

#include <doctest/doctest.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include "core/layer/layer_visitor.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("GroupLayer")
{
  TEST_CASE("Defaults")
  {
    const GroupLayer root;
    REQUIRE(0u == root.layer_count());
  }

  TEST_CASE("each (simple)")
  {
    GroupLayer root;

    auto t1 = std::make_shared<TileLayer>();
    auto g1 = std::make_shared<GroupLayer>();
    auto o1 = std::make_shared<ObjectLayer>();

    root.add_layer(t1);
    root.add_layer(g1);
    root.add_layer(g1->get_uuid(), o1);

    REQUIRE(3u == root.layer_count());

    REQUIRE(nothing == t1->get_parent());
    REQUIRE(nothing == g1->get_parent());
    REQUIRE(g1->get_uuid() == o1->get_parent());

    usize count = 0;
    root.each([&](const Layer& layer) {
      ++count;

      switch (layer.get_type()) {
        case LayerType::TileLayer:
          REQUIRE(!layer.get_parent().has_value());
          REQUIRE(0u == root.global_layer_index(layer.get_uuid()));
          break;

        case LayerType::ObjectLayer:
          REQUIRE(g1->get_uuid() == layer.get_parent());
          REQUIRE(2u == root.global_layer_index(layer.get_uuid()));
          break;

        case LayerType::GroupLayer:
          REQUIRE(!layer.get_parent().has_value());
          REQUIRE(1u == root.global_layer_index(layer.get_uuid()));
          break;
      }
    });

    REQUIRE(3u == count);
  }

  TEST_CASE("add_layer")
  {
    GroupLayer root;

    auto t1 = std::make_shared<TileLayer>();
    root.add_layer(t1);

    REQUIRE(1u == root.layer_count());
    REQUIRE(0u == root.local_layer_index(t1->get_uuid()));
    REQUIRE(0u == root.global_layer_index(t1->get_uuid()));
    REQUIRE(nothing == t1->get_parent());

    auto t2 = std::make_shared<TileLayer>();
    root.add_layer(t2);

    REQUIRE(2u == root.layer_count());
    REQUIRE(0u == root.local_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t2->get_uuid()));
    REQUIRE(0u == root.global_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.global_layer_index(t2->get_uuid()));
    REQUIRE(nothing == t2->get_parent());

    auto g1 = std::make_shared<GroupLayer>();
    auto t3 = std::make_shared<TileLayer>();

    root.add_layer(g1);
    root.add_layer(g1->get_uuid(), t3);

    REQUIRE(4u == root.layer_count());

    REQUIRE(0u == root.local_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t2->get_uuid()));
    REQUIRE(2u == root.local_layer_index(g1->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t3->get_uuid()));

    REQUIRE(0u == root.global_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.global_layer_index(t2->get_uuid()));
    REQUIRE(2u == root.global_layer_index(g1->get_uuid()));
    REQUIRE(3u == root.global_layer_index(t3->get_uuid()));

    REQUIRE(nothing == g1->get_parent());
    REQUIRE(g1->get_uuid() == t3->get_parent());
  }

  TEST_CASE("add_layer (to parent)")
  {
    GroupLayer root;

    auto group = std::make_shared<GroupLayer>();
    root.add_layer(group);
    root.add_layer(group->get_uuid(), std::make_shared<TileLayer>());
    REQUIRE(2u == root.layer_count());

    REQUIRE_NOTHROW(root.add_layer(UUID::generate(), std::make_shared<ObjectLayer>()));
    REQUIRE(2u == root.layer_count());
  }

  TEST_CASE("remove_layer")
  {
    GroupLayer root;
    REQUIRE(nullptr == root.remove_layer(root.get_uuid()));

    auto t1 = std::make_shared<TileLayer>();
    auto t2 = std::make_shared<TileLayer>();
    auto g1 = std::make_shared<GroupLayer>();

    root.add_layer(g1);
    root.add_layer(g1->get_uuid(), t1);
    root.add_layer(t2);

    REQUIRE(t1 == root.remove_layer(t1->get_uuid()));
    REQUIRE(g1 == root.remove_layer(g1->get_uuid()));
    REQUIRE(t2 == root.remove_layer(t2->get_uuid()));

    REQUIRE(0u == root.layer_count());
  }

  TEST_CASE("move_layer_up")
  {
    GroupLayer root;
    REQUIRE_NOTHROW(root.move_layer_up(root.get_uuid()));

    auto t1 = std::make_shared<TileLayer>();
    auto t2 = std::make_shared<TileLayer>();
    auto t3 = std::make_shared<TileLayer>();
    auto o1 = std::make_shared<ObjectLayer>();
    auto g1 = std::make_shared<GroupLayer>();
    auto g2 = std::make_shared<GroupLayer>();

    // root
    // -- g1
    // ---- g2
    // ------ t1
    // ------ t2
    // ---- t3
    // -- o1
    root.add_layer(g1);
    root.add_layer(g1->get_uuid(), g2);
    root.add_layer(g2->get_uuid(), t1);
    root.add_layer(g2->get_uuid(), t2);
    root.add_layer(g1->get_uuid(), t3);
    root.add_layer(o1);

    REQUIRE(0u == root.global_layer_index(g1->get_uuid()));
    REQUIRE(1u == root.global_layer_index(g2->get_uuid()));
    REQUIRE(2u == root.global_layer_index(t1->get_uuid()));
    REQUIRE(3u == root.global_layer_index(t2->get_uuid()));
    REQUIRE(4u == root.global_layer_index(t3->get_uuid()));
    REQUIRE(5u == root.global_layer_index(o1->get_uuid()));

    REQUIRE(0u == root.local_layer_index(g1->get_uuid()));
    REQUIRE(0u == root.local_layer_index(g2->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t2->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t3->get_uuid()));
    REQUIRE(1u == root.local_layer_index(o1->get_uuid()));

    // root
    // -- g1
    // ---- g2
    // ------ t2
    // ------ t1
    // ---- t3
    // -- o1
    root.move_layer_up(t2->get_uuid());

    REQUIRE(0u == root.global_layer_index(g1->get_uuid()));
    REQUIRE(1u == root.global_layer_index(g2->get_uuid()));
    REQUIRE(2u == root.global_layer_index(t2->get_uuid()));
    REQUIRE(3u == root.global_layer_index(t1->get_uuid()));
    REQUIRE(4u == root.global_layer_index(t3->get_uuid()));

    REQUIRE(0u == root.local_layer_index(g1->get_uuid()));
    REQUIRE(0u == root.local_layer_index(g2->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t2->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t3->get_uuid()));

    // root
    // -- g1
    // ---- t3
    // ---- g2
    // ------ t2
    // ------ t1
    // -- o1
    root.move_layer_up(t3->get_uuid());

    REQUIRE(0u == root.global_layer_index(g1->get_uuid()));
    REQUIRE(1u == root.global_layer_index(t3->get_uuid()));
    REQUIRE(2u == root.global_layer_index(g2->get_uuid()));
    REQUIRE(3u == root.global_layer_index(t2->get_uuid()));
    REQUIRE(4u == root.global_layer_index(t1->get_uuid()));

    REQUIRE(0u == root.local_layer_index(g1->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t3->get_uuid()));
    REQUIRE(1u == root.local_layer_index(g2->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t2->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t1->get_uuid()));

    // root
    // -- o1
    // -- g1
    // ---- t3
    // ---- g2
    // ------ t2
    // ------ t1
    root.move_layer_up(o1->get_uuid());

    REQUIRE(0u == root.global_layer_index(o1->get_uuid()));
    REQUIRE(1u == root.global_layer_index(g1->get_uuid()));
    REQUIRE(2u == root.global_layer_index(t3->get_uuid()));
    REQUIRE(3u == root.global_layer_index(g2->get_uuid()));
    REQUIRE(4u == root.global_layer_index(t2->get_uuid()));
    REQUIRE(5u == root.global_layer_index(t1->get_uuid()));

    REQUIRE(0u == root.local_layer_index(o1->get_uuid()));
    REQUIRE(1u == root.local_layer_index(g1->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t3->get_uuid()));
    REQUIRE(1u == root.local_layer_index(g2->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t2->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t1->get_uuid()));
  }

  TEST_CASE("move_layer_down")
  {
    GroupLayer root;
    REQUIRE_NOTHROW(root.move_layer_down(root.get_uuid()));

    auto t1 = std::make_shared<TileLayer>();
    auto t2 = std::make_shared<TileLayer>();
    auto t3 = std::make_shared<TileLayer>();
    auto t4 = std::make_shared<TileLayer>();
    auto g1 = std::make_shared<GroupLayer>();
    auto g2 = std::make_shared<GroupLayer>();

    // root
    // -- g1
    // ---- t1
    // ---- g2
    // ------ t2
    // ---- t3
    // -- t4
    root.add_layer(g1);
    root.add_layer(g1->get_uuid(), t1);
    root.add_layer(g1->get_uuid(), g2);
    root.add_layer(g2->get_uuid(), t2);
    root.add_layer(g1->get_uuid(), t3);
    root.add_layer(t4);

    REQUIRE(0u == root.global_layer_index(g1->get_uuid()));
    REQUIRE(1u == root.global_layer_index(t1->get_uuid()));
    REQUIRE(2u == root.global_layer_index(g2->get_uuid()));
    REQUIRE(3u == root.global_layer_index(t2->get_uuid()));
    REQUIRE(4u == root.global_layer_index(t3->get_uuid()));
    REQUIRE(5u == root.global_layer_index(t4->get_uuid()));

    REQUIRE(0u == root.local_layer_index(g1->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.local_layer_index(g2->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t2->get_uuid()));
    REQUIRE(2u == root.local_layer_index(t3->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t4->get_uuid()));

    // root
    // -- g1
    // ---- t1
    // ---- t3
    // ---- g2
    // ------ t2
    // -- t4
    root.move_layer_down(g2->get_uuid());

    REQUIRE(0u == root.global_layer_index(g1->get_uuid()));
    REQUIRE(1u == root.global_layer_index(t1->get_uuid()));
    REQUIRE(2u == root.global_layer_index(t3->get_uuid()));
    REQUIRE(3u == root.global_layer_index(g2->get_uuid()));
    REQUIRE(4u == root.global_layer_index(t2->get_uuid()));
    REQUIRE(5u == root.global_layer_index(t4->get_uuid()));

    REQUIRE(0u == root.local_layer_index(g1->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t3->get_uuid()));
    REQUIRE(2u == root.local_layer_index(g2->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t2->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t4->get_uuid()));

    // root
    // -- t4
    // -- g1
    // ---- t1
    // ---- t3
    // ---- g2
    // ------ t2
    root.move_layer_down(g1->get_uuid());

    REQUIRE(0u == root.global_layer_index(t4->get_uuid()));
    REQUIRE(1u == root.global_layer_index(g1->get_uuid()));
    REQUIRE(2u == root.global_layer_index(t1->get_uuid()));
    REQUIRE(3u == root.global_layer_index(t3->get_uuid()));
    REQUIRE(4u == root.global_layer_index(g2->get_uuid()));
    REQUIRE(5u == root.global_layer_index(t2->get_uuid()));

    REQUIRE(0u == root.local_layer_index(t4->get_uuid()));
    REQUIRE(1u == root.local_layer_index(g1->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.local_layer_index(t3->get_uuid()));
    REQUIRE(2u == root.local_layer_index(g2->get_uuid()));
    REQUIRE(0u == root.local_layer_index(t2->get_uuid()));
  }

  TEST_CASE("layer_sibling_count")
  {
    GroupLayer root;
    REQUIRE_THROWS_AS(root.layer_sibling_count(root.get_uuid()), Exception);

    auto t1 = std::make_shared<TileLayer>();
    auto t2 = std::make_shared<TileLayer>();
    auto t3 = std::make_shared<TileLayer>();
    auto g1 = std::make_shared<GroupLayer>();
    auto g2 = std::make_shared<GroupLayer>();
    auto o1 = std::make_shared<ObjectLayer>();

    root.add_layer(g1);
    root.add_layer(g1->get_uuid(), t1);
    root.add_layer(g1->get_uuid(), t2);
    root.add_layer(g2);
    root.add_layer(g2->get_uuid(), o1);
    root.add_layer(t3);

    REQUIRE(2u == root.layer_sibling_count(g1->get_uuid()));
    REQUIRE(2u == root.layer_sibling_count(g2->get_uuid()));
    REQUIRE(2u == root.layer_sibling_count(t3->get_uuid()));

    REQUIRE(1u == root.layer_sibling_count(t1->get_uuid()));
    REQUIRE(1u == root.layer_sibling_count(t2->get_uuid()));

    REQUIRE(0u == root.layer_sibling_count(o1->get_uuid()));
  }

  TEST_CASE("local_layer_index")
  {
    GroupLayer root;
    REQUIRE_THROWS_AS(root.local_layer_index(root.get_uuid()), Exception);

    auto layer1 = std::make_shared<TileLayer>();
    auto layer2 = std::make_shared<TileLayer>();
    auto layer3 = std::make_shared<TileLayer>();

    root.add_layer(layer1);
    REQUIRE(0u == root.local_layer_index(layer1->get_uuid()));

    root.add_layer(layer2);
    root.add_layer(layer3);

    REQUIRE(0u == root.local_layer_index(layer1->get_uuid()));
    REQUIRE(1u == root.local_layer_index(layer2->get_uuid()));
    REQUIRE(2u == root.local_layer_index(layer3->get_uuid()));
  }

  TEST_CASE("global_layer_index")
  {
    GroupLayer root;
    REQUIRE_THROWS_AS(root.global_layer_index(root.get_uuid()), Exception);

    auto t1 = std::make_shared<TileLayer>();
    auto t2 = std::make_shared<TileLayer>();
    auto t3 = std::make_shared<TileLayer>();
    auto g1 = std::make_shared<GroupLayer>();
    auto g2 = std::make_shared<GroupLayer>();

    // root
    // -- t1
    // -- g1
    // ---- g2
    // ------ t2
    // ---- t3
    root.add_layer(t1);
    root.add_layer(g1);
    root.add_layer(g1->get_uuid(), g2);
    root.add_layer(g2->get_uuid(), t2);
    root.add_layer(g1->get_uuid(), t3);

    REQUIRE(0u == root.global_layer_index(t1->get_uuid()));
    REQUIRE(1u == root.global_layer_index(g1->get_uuid()));
    REQUIRE(2u == root.global_layer_index(g2->get_uuid()));
    REQUIRE(3u == root.global_layer_index(t2->get_uuid()));
    REQUIRE(4u == root.global_layer_index(t3->get_uuid()));
  }

  TEST_CASE("can_move_layer_up")
  {
    GroupLayer root;
    REQUIRE_THROWS_AS(root.can_move_layer_up(root.get_uuid()), Exception);

    auto t1 = std::make_shared<TileLayer>();
    auto t2 = std::make_shared<TileLayer>();
    auto t3 = std::make_shared<TileLayer>();
    auto g1 = std::make_shared<GroupLayer>();

    root.add_layer(t1);
    root.add_layer(g1);
    root.add_layer(g1->get_uuid(), t2);
    root.add_layer(t3);

    REQUIRE(!root.can_move_layer_up(t1->get_uuid()));
    REQUIRE(root.can_move_layer_up(g1->get_uuid()));
    REQUIRE(!root.can_move_layer_up(t2->get_uuid()));
    REQUIRE(root.can_move_layer_up(t3->get_uuid()));
  }

  TEST_CASE("can_move_layer_down")
  {
    GroupLayer root;
    REQUIRE_THROWS_AS(root.can_move_layer_down(root.get_uuid()), Exception);

    auto t1 = std::make_shared<TileLayer>();
    auto t2 = std::make_shared<TileLayer>();
    auto o1 = std::make_shared<ObjectLayer>();
    auto g1 = std::make_shared<GroupLayer>();

    root.add_layer(g1);
    root.add_layer(g1->get_uuid(), t1);
    root.add_layer(g1->get_uuid(), o1);
    root.add_layer(t2);

    REQUIRE(root.can_move_layer_down(g1->get_uuid()));
    REQUIRE(root.can_move_layer_down(t1->get_uuid()));
    REQUIRE(!root.can_move_layer_down(o1->get_uuid()));
    REQUIRE(!root.can_move_layer_down(t2->get_uuid()));
  }
}

}  // namespace tactile::test
