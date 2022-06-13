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

#include "core/layers/group_layer.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <fmt/ostream.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "core/layers/layer_visitor.hpp"
#include "core/layers/object_layer.hpp"
#include "core/layers/tile_layer.hpp"
#include "misc/panic.hpp"

using namespace tactile;
using namespace core;

TEST(GroupLayer, Defaults)
{
  GroupLayer root;
  ASSERT_EQ(0u, root.layer_count());

  struct Counter final : core::IConstLayerVisitor
  {
    usize count{};

    void visit(const TileLayer&) override { ++count; }
    void visit(const ObjectLayer&) override { ++count; }
    void visit(const GroupLayer&) override { ++count; }
  };

  Counter counter;
  root.each(counter);
  ASSERT_EQ(0u, counter.count);
}

TEST(GroupLayer, AddLayer)
{
  GroupLayer root;
  ASSERT_THROW(root.add_layer(nullptr), TactileError);

  auto t1 = TileLayer::make();
  root.add_layer(t1);

  ASSERT_EQ(1u, root.layer_count());
  ASSERT_EQ(0u, root.get_local_index(t1->get_uuid()));
  ASSERT_EQ(0u, root.get_global_index(t1->get_uuid()));

  auto t2 = TileLayer::make();
  root.add_layer(t2);

  ASSERT_EQ(2u, root.layer_count());
  ASSERT_EQ(0u, root.get_local_index(t1->get_uuid()));
  ASSERT_EQ(1u, root.get_local_index(t2->get_uuid()));
  ASSERT_EQ(0u, root.get_global_index(t1->get_uuid()));
  ASSERT_EQ(1u, root.get_global_index(t2->get_uuid()));

  auto g1 = GroupLayer::make();
  auto t3 = TileLayer::make();

  root.add_layer(g1);
  g1->add_layer(t3);

  ASSERT_EQ(4u, root.layer_count());

  ASSERT_EQ(0u, root.get_local_index(t1->get_uuid()));
  ASSERT_EQ(1u, root.get_local_index(t2->get_uuid()));
  ASSERT_EQ(2u, root.get_local_index(g1->get_uuid()));
  ASSERT_EQ(0u, root.get_local_index(t3->get_uuid()));

  ASSERT_EQ(0u, root.get_global_index(t1->get_uuid()));
  ASSERT_EQ(1u, root.get_global_index(t2->get_uuid()));
  ASSERT_EQ(2u, root.get_global_index(g1->get_uuid()));
  ASSERT_EQ(3u, root.get_global_index(t3->get_uuid()));
}

TEST(GroupLayer, AddLayerToParent)
{
  GroupLayer root;
  ASSERT_THROW(root.add_layer(root.get_uuid(), TileLayer::make()), TactileError);
}

TEST(GroupLayer, RemoveLayer)
{
  GroupLayer root;
  ASSERT_THROW(root.remove_layer(root.get_uuid()), TactileError);

  auto t1 = TileLayer::make();
  auto t2 = TileLayer::make();
  auto g1 = GroupLayer::make();

  root.add_layer(g1);
  root.add_layer(g1->get_uuid(), t1);
  root.add_layer(t2);

  ASSERT_EQ(t1, root.remove_layer(t1->get_uuid()));
  ASSERT_EQ(g1, root.remove_layer(g1->get_uuid()));
  ASSERT_EQ(t2, root.remove_layer(t2->get_uuid()));

  ASSERT_EQ(0, root.layer_count());
}

TEST(GroupLayer, MoveLayerUp)
{
  GroupLayer root;
  ASSERT_THROW(root.move_layer_up(root.get_uuid()), TactileError);

  auto t1 = TileLayer::make();
  auto t2 = TileLayer::make();
  auto t3 = TileLayer::make();
  auto o1 = ObjectLayer::make();
  auto g1 = GroupLayer::make();
  auto g2 = GroupLayer::make();

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

  ASSERT_EQ(0, root.get_global_index(g1->get_uuid()));
  ASSERT_EQ(1, root.get_global_index(g2->get_uuid()));
  ASSERT_EQ(2, root.get_global_index(t1->get_uuid()));
  ASSERT_EQ(3, root.get_global_index(t2->get_uuid()));
  ASSERT_EQ(4, root.get_global_index(t3->get_uuid()));
  ASSERT_EQ(5, root.get_global_index(o1->get_uuid()));

  ASSERT_EQ(0, root.get_local_index(g1->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(g2->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t1->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t2->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t3->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(o1->get_uuid()));

  // root
  // -- g1
  // ---- g2
  // ------ t2
  // ------ t1
  // ---- t3
  // -- o1
  root.move_layer_up(t2->get_uuid());

  ASSERT_EQ(0, root.get_global_index(g1->get_uuid()));
  ASSERT_EQ(1, root.get_global_index(g2->get_uuid()));
  ASSERT_EQ(2, root.get_global_index(t2->get_uuid()));
  ASSERT_EQ(3, root.get_global_index(t1->get_uuid()));
  ASSERT_EQ(4, root.get_global_index(t3->get_uuid()));

  ASSERT_EQ(0, root.get_local_index(g1->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(g2->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t2->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t1->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t3->get_uuid()));

  // root
  // -- g1
  // ---- t3
  // ---- g2
  // ------ t2
  // ------ t1
  // -- o1
  root.move_layer_up(t3->get_uuid());

  ASSERT_EQ(0, root.get_global_index(g1->get_uuid()));
  ASSERT_EQ(1, root.get_global_index(t3->get_uuid()));
  ASSERT_EQ(2, root.get_global_index(g2->get_uuid()));
  ASSERT_EQ(3, root.get_global_index(t2->get_uuid()));
  ASSERT_EQ(4, root.get_global_index(t1->get_uuid()));

  ASSERT_EQ(0, root.get_local_index(g1->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t3->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(g2->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t2->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t1->get_uuid()));

  // root
  // -- o1
  // -- g1
  // ---- t3
  // ---- g2
  // ------ t2
  // ------ t1
  root.move_layer_up(o1->get_uuid());

  ASSERT_EQ(0, root.get_global_index(o1->get_uuid()));
  ASSERT_EQ(1, root.get_global_index(g1->get_uuid()));
  ASSERT_EQ(2, root.get_global_index(t3->get_uuid()));
  ASSERT_EQ(3, root.get_global_index(g2->get_uuid()));
  ASSERT_EQ(4, root.get_global_index(t2->get_uuid()));
  ASSERT_EQ(5, root.get_global_index(t1->get_uuid()));

  ASSERT_EQ(0, root.get_local_index(o1->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(g1->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t3->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(g2->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t2->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t1->get_uuid()));
}

TEST(GroupLayer, MoveLayerDown)
{
  GroupLayer root;
  ASSERT_THROW(root.move_layer_down(root.get_uuid()), TactileError);

  auto t1 = TileLayer::make();
  auto t2 = TileLayer::make();
  auto t3 = TileLayer::make();
  auto t4 = TileLayer::make();
  auto g1 = GroupLayer::make();
  auto g2 = GroupLayer::make();

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

  ASSERT_EQ(0, root.get_global_index(g1->get_uuid()));
  ASSERT_EQ(1, root.get_global_index(t1->get_uuid()));
  ASSERT_EQ(2, root.get_global_index(g2->get_uuid()));
  ASSERT_EQ(3, root.get_global_index(t2->get_uuid()));
  ASSERT_EQ(4, root.get_global_index(t3->get_uuid()));
  ASSERT_EQ(5, root.get_global_index(t4->get_uuid()));

  ASSERT_EQ(0, root.get_local_index(g1->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t1->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(g2->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t2->get_uuid()));
  ASSERT_EQ(2, root.get_local_index(t3->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t4->get_uuid()));

  // root
  // -- g1
  // ---- t1
  // ---- t3
  // ---- g2
  // ------ t2
  // -- t4
  root.move_layer_down(g2->get_uuid());

  ASSERT_EQ(0, root.get_global_index(g1->get_uuid()));
  ASSERT_EQ(1, root.get_global_index(t1->get_uuid()));
  ASSERT_EQ(2, root.get_global_index(t3->get_uuid()));
  ASSERT_EQ(3, root.get_global_index(g2->get_uuid()));
  ASSERT_EQ(4, root.get_global_index(t2->get_uuid()));
  ASSERT_EQ(5, root.get_global_index(t4->get_uuid()));

  ASSERT_EQ(0, root.get_local_index(g1->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t1->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t3->get_uuid()));
  ASSERT_EQ(2, root.get_local_index(g2->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t2->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t4->get_uuid()));

  // root
  // -- t4
  // -- g1
  // ---- t1
  // ---- t3
  // ---- g2
  // ------ t2
  root.move_layer_down(g1->get_uuid());

  ASSERT_EQ(0, root.get_global_index(t4->get_uuid()));
  ASSERT_EQ(1, root.get_global_index(g1->get_uuid()));
  ASSERT_EQ(2, root.get_global_index(t1->get_uuid()));
  ASSERT_EQ(3, root.get_global_index(t3->get_uuid()));
  ASSERT_EQ(4, root.get_global_index(g2->get_uuid()));
  ASSERT_EQ(5, root.get_global_index(t2->get_uuid()));

  ASSERT_EQ(0, root.get_local_index(t4->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(g1->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t1->get_uuid()));
  ASSERT_EQ(1, root.get_local_index(t3->get_uuid()));
  ASSERT_EQ(2, root.get_local_index(g2->get_uuid()));
  ASSERT_EQ(0, root.get_local_index(t2->get_uuid()));
}

TEST(GroupLayer, SiblingCount)
{
  GroupLayer root;
  ASSERT_THROW(root.sibling_count(root.get_uuid()), TactileError);

  auto t1 = TileLayer::make();
  auto t2 = TileLayer::make();
  auto t3 = TileLayer::make();
  auto g1 = GroupLayer::make();
  auto g2 = GroupLayer::make();
  auto o1 = ObjectLayer::make();

  root.add_layer(g1);
  root.add_layer(g1->get_uuid(), t1);
  root.add_layer(g1->get_uuid(), t2);
  root.add_layer(g2);
  root.add_layer(g2->get_uuid(), o1);
  root.add_layer(t3);

  ASSERT_EQ(2, root.sibling_count(g1->get_uuid()));
  ASSERT_EQ(2, root.sibling_count(g2->get_uuid()));
  ASSERT_EQ(2, root.sibling_count(t3->get_uuid()));

  ASSERT_EQ(1, root.sibling_count(t1->get_uuid()));
  ASSERT_EQ(1, root.sibling_count(t2->get_uuid()));

  ASSERT_EQ(0, root.sibling_count(o1->get_uuid()));
}

TEST(GroupLayer, GetLocalIndex)
{
  GroupLayer root;
  ASSERT_THROW(root.get_local_index(root.get_uuid()), TactileError);

  auto layer1 = TileLayer::make();
  auto layer2 = TileLayer::make();
  auto layer3 = TileLayer::make();

  root.add_layer(layer1);
  ASSERT_EQ(0u, root.get_local_index(layer1->get_uuid()));

  root.add_layer(layer2);
  root.add_layer(layer3);

  ASSERT_EQ(0u, root.get_local_index(layer1->get_uuid()));
  ASSERT_EQ(1u, root.get_local_index(layer2->get_uuid()));
  ASSERT_EQ(2u, root.get_local_index(layer3->get_uuid()));
}

TEST(GroupLayer, GetGlobalIndex)
{
  GroupLayer root;
  ASSERT_THROW(root.get_global_index(root.get_uuid()), TactileError);

  auto t1 = TileLayer::make();
  auto t2 = TileLayer::make();
  auto t3 = TileLayer::make();
  auto g1 = GroupLayer::make();
  auto g2 = GroupLayer::make();

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

  ASSERT_EQ(0u, root.get_global_index(t1->get_uuid()));
  ASSERT_EQ(1u, root.get_global_index(g1->get_uuid()));
  ASSERT_EQ(2u, root.get_global_index(g2->get_uuid()));
  ASSERT_EQ(3u, root.get_global_index(t2->get_uuid()));
  ASSERT_EQ(4u, root.get_global_index(t3->get_uuid()));
}

TEST(GroupLayer, CanMoveLayerUp)
{
  GroupLayer root;
  ASSERT_THROW(root.can_move_layer_up(root.get_uuid()), TactileError);

  auto t1 = TileLayer::make();
  auto t2 = TileLayer::make();
  auto t3 = TileLayer::make();
  auto g1 = GroupLayer::make();

  root.add_layer(t1);
  root.add_layer(g1);
  root.add_layer(g1->get_uuid(), t2);
  root.add_layer(t3);

  ASSERT_FALSE(root.can_move_layer_up(t1->get_uuid()));
  ASSERT_TRUE(root.can_move_layer_up(g1->get_uuid()));
  ASSERT_FALSE(root.can_move_layer_up(t2->get_uuid()));
  ASSERT_TRUE(root.can_move_layer_up(t3->get_uuid()));
}

TEST(GroupLayer, CanMoveLayerDown)
{
  GroupLayer root;
  ASSERT_THROW(root.can_move_layer_down(root.get_uuid()), TactileError);

  auto t1 = TileLayer::make();
  auto t2 = TileLayer::make();
  auto o1 = ObjectLayer::make();
  auto g1 = GroupLayer::make();

  root.add_layer(g1);
  root.add_layer(g1->get_uuid(), t1);
  root.add_layer(g1->get_uuid(), o1);
  root.add_layer(t2);

  ASSERT_TRUE(root.can_move_layer_down(g1->get_uuid()));
  ASSERT_TRUE(root.can_move_layer_down(t1->get_uuid()));
  ASSERT_FALSE(root.can_move_layer_down(o1->get_uuid()));
  ASSERT_FALSE(root.can_move_layer_down(t2->get_uuid()));
}
