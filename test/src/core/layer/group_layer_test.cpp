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

#include <fmt/ostream.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "core/layer/layer_visitor.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(GroupLayer, Defaults)
{
  GroupLayer root;
  ASSERT_EQ(0u, root.size());

  struct Counter final : ConstLayerVisitor {
    usize count {};

    void visit(const TileLayer&) override { ++count; }
    void visit(const ObjectLayer&) override { ++count; }
    void visit(const GroupLayer&) override { ++count; }
  };

  Counter counter;
  root.each(counter);
  ASSERT_EQ(0u, counter.count);
}

TEST(GroupLayer, SimpleEach)
{
  GroupLayer root;

  auto t1 = std::make_shared<TileLayer>();
  auto g1 = std::make_shared<GroupLayer>();
  auto o1 = std::make_shared<ObjectLayer>();

  root.add(t1);
  root.add(g1);
  root.add(g1->uuid(), o1);

  ASSERT_EQ(3u, root.size());

  ASSERT_EQ(nothing, t1->parent());
  ASSERT_EQ(nothing, g1->parent());
  ASSERT_EQ(g1->uuid(), o1->parent());

  usize count = 0;
  root.each([&](const Layer* layer) {
    ASSERT_TRUE(layer != nullptr);
    ++count;

    switch (layer->type()) {
      case LayerType::TileLayer:
        ASSERT_FALSE(layer->parent().has_value());
        ASSERT_EQ(0u, root.global_index(layer->uuid()));
        break;

      case LayerType::ObjectLayer:
        ASSERT_EQ(g1->uuid(), layer->parent());
        ASSERT_EQ(2u, root.global_index(layer->uuid()));
        break;

      case LayerType::GroupLayer:
        ASSERT_FALSE(layer->parent().has_value());
        ASSERT_EQ(1u, root.global_index(layer->uuid()));
        break;
    }
  });

  ASSERT_EQ(3u, count);
}

TEST(GroupLayer, Add)
{
  GroupLayer root;
  ASSERT_THROW(root.add(nullptr), TactileError);

  auto t1 = std::make_shared<TileLayer>();
  root.add(t1);

  ASSERT_EQ(1u, root.size());
  ASSERT_EQ(0u, root.local_index(t1->uuid()));
  ASSERT_EQ(0u, root.global_index(t1->uuid()));
  ASSERT_EQ(nothing, t1->parent());

  auto t2 = std::make_shared<TileLayer>();
  root.add(t2);

  ASSERT_EQ(2u, root.size());
  ASSERT_EQ(0u, root.local_index(t1->uuid()));
  ASSERT_EQ(1u, root.local_index(t2->uuid()));
  ASSERT_EQ(0u, root.global_index(t1->uuid()));
  ASSERT_EQ(1u, root.global_index(t2->uuid()));
  ASSERT_EQ(nothing, t2->parent());

  auto g1 = std::make_shared<GroupLayer>();
  auto t3 = std::make_shared<TileLayer>();

  root.add(g1);
  root.add(g1->uuid(), t3);

  ASSERT_EQ(4u, root.size());

  ASSERT_EQ(0u, root.local_index(t1->uuid()));
  ASSERT_EQ(1u, root.local_index(t2->uuid()));
  ASSERT_EQ(2u, root.local_index(g1->uuid()));
  ASSERT_EQ(0u, root.local_index(t3->uuid()));

  ASSERT_EQ(0u, root.global_index(t1->uuid()));
  ASSERT_EQ(1u, root.global_index(t2->uuid()));
  ASSERT_EQ(2u, root.global_index(g1->uuid()));
  ASSERT_EQ(3u, root.global_index(t3->uuid()));

  ASSERT_EQ(nothing, g1->parent());
  ASSERT_EQ(g1->uuid(), t3->parent());
}

TEST(GroupLayer, AddToParent)
{
  GroupLayer root;
  ASSERT_THROW(root.add(root.uuid(), std::make_shared<TileLayer>()), TactileError);
}

TEST(GroupLayer, Remove)
{
  GroupLayer root;
  ASSERT_THROW(root.remove(root.uuid()), TactileError);

  auto t1 = std::make_shared<TileLayer>();
  auto t2 = std::make_shared<TileLayer>();
  auto g1 = std::make_shared<GroupLayer>();

  root.add(g1);
  root.add(g1->uuid(), t1);
  root.add(t2);

  ASSERT_EQ(t1, root.remove(t1->uuid()));
  ASSERT_EQ(g1, root.remove(g1->uuid()));
  ASSERT_EQ(t2, root.remove(t2->uuid()));

  ASSERT_EQ(0u, root.size());
}

TEST(GroupLayer, MoveUp)
{
  GroupLayer root;
  ASSERT_THROW(root.move_up(root.uuid()), TactileError);

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
  root.add(g1);
  root.add(g1->uuid(), g2);
  root.add(g2->uuid(), t1);
  root.add(g2->uuid(), t2);
  root.add(g1->uuid(), t3);
  root.add(o1);

  ASSERT_EQ(0u, root.global_index(g1->uuid()));
  ASSERT_EQ(1u, root.global_index(g2->uuid()));
  ASSERT_EQ(2u, root.global_index(t1->uuid()));
  ASSERT_EQ(3u, root.global_index(t2->uuid()));
  ASSERT_EQ(4u, root.global_index(t3->uuid()));
  ASSERT_EQ(5u, root.global_index(o1->uuid()));

  ASSERT_EQ(0u, root.local_index(g1->uuid()));
  ASSERT_EQ(0u, root.local_index(g2->uuid()));
  ASSERT_EQ(0u, root.local_index(t1->uuid()));
  ASSERT_EQ(1u, root.local_index(t2->uuid()));
  ASSERT_EQ(1u, root.local_index(t3->uuid()));
  ASSERT_EQ(1u, root.local_index(o1->uuid()));

  // root
  // -- g1
  // ---- g2
  // ------ t2
  // ------ t1
  // ---- t3
  // -- o1
  root.move_up(t2->uuid());

  ASSERT_EQ(0u, root.global_index(g1->uuid()));
  ASSERT_EQ(1u, root.global_index(g2->uuid()));
  ASSERT_EQ(2u, root.global_index(t2->uuid()));
  ASSERT_EQ(3u, root.global_index(t1->uuid()));
  ASSERT_EQ(4u, root.global_index(t3->uuid()));

  ASSERT_EQ(0u, root.local_index(g1->uuid()));
  ASSERT_EQ(0u, root.local_index(g2->uuid()));
  ASSERT_EQ(0u, root.local_index(t2->uuid()));
  ASSERT_EQ(1u, root.local_index(t1->uuid()));
  ASSERT_EQ(1u, root.local_index(t3->uuid()));

  // root
  // -- g1
  // ---- t3
  // ---- g2
  // ------ t2
  // ------ t1
  // -- o1
  root.move_up(t3->uuid());

  ASSERT_EQ(0u, root.global_index(g1->uuid()));
  ASSERT_EQ(1u, root.global_index(t3->uuid()));
  ASSERT_EQ(2u, root.global_index(g2->uuid()));
  ASSERT_EQ(3u, root.global_index(t2->uuid()));
  ASSERT_EQ(4u, root.global_index(t1->uuid()));

  ASSERT_EQ(0u, root.local_index(g1->uuid()));
  ASSERT_EQ(0u, root.local_index(t3->uuid()));
  ASSERT_EQ(1u, root.local_index(g2->uuid()));
  ASSERT_EQ(0u, root.local_index(t2->uuid()));
  ASSERT_EQ(1u, root.local_index(t1->uuid()));

  // root
  // -- o1
  // -- g1
  // ---- t3
  // ---- g2
  // ------ t2
  // ------ t1
  root.move_up(o1->uuid());

  ASSERT_EQ(0u, root.global_index(o1->uuid()));
  ASSERT_EQ(1u, root.global_index(g1->uuid()));
  ASSERT_EQ(2u, root.global_index(t3->uuid()));
  ASSERT_EQ(3u, root.global_index(g2->uuid()));
  ASSERT_EQ(4u, root.global_index(t2->uuid()));
  ASSERT_EQ(5u, root.global_index(t1->uuid()));

  ASSERT_EQ(0u, root.local_index(o1->uuid()));
  ASSERT_EQ(1u, root.local_index(g1->uuid()));
  ASSERT_EQ(0u, root.local_index(t3->uuid()));
  ASSERT_EQ(1u, root.local_index(g2->uuid()));
  ASSERT_EQ(0u, root.local_index(t2->uuid()));
  ASSERT_EQ(1u, root.local_index(t1->uuid()));
}

TEST(GroupLayer, MoveDown)
{
  GroupLayer root;
  ASSERT_THROW(root.move_down(root.uuid()), TactileError);

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
  root.add(g1);
  root.add(g1->uuid(), t1);
  root.add(g1->uuid(), g2);
  root.add(g2->uuid(), t2);
  root.add(g1->uuid(), t3);
  root.add(t4);

  ASSERT_EQ(0u, root.global_index(g1->uuid()));
  ASSERT_EQ(1u, root.global_index(t1->uuid()));
  ASSERT_EQ(2u, root.global_index(g2->uuid()));
  ASSERT_EQ(3u, root.global_index(t2->uuid()));
  ASSERT_EQ(4u, root.global_index(t3->uuid()));
  ASSERT_EQ(5u, root.global_index(t4->uuid()));

  ASSERT_EQ(0u, root.local_index(g1->uuid()));
  ASSERT_EQ(0u, root.local_index(t1->uuid()));
  ASSERT_EQ(1u, root.local_index(g2->uuid()));
  ASSERT_EQ(0u, root.local_index(t2->uuid()));
  ASSERT_EQ(2u, root.local_index(t3->uuid()));
  ASSERT_EQ(1u, root.local_index(t4->uuid()));

  // root
  // -- g1
  // ---- t1
  // ---- t3
  // ---- g2
  // ------ t2
  // -- t4
  root.move_down(g2->uuid());

  ASSERT_EQ(0u, root.global_index(g1->uuid()));
  ASSERT_EQ(1u, root.global_index(t1->uuid()));
  ASSERT_EQ(2u, root.global_index(t3->uuid()));
  ASSERT_EQ(3u, root.global_index(g2->uuid()));
  ASSERT_EQ(4u, root.global_index(t2->uuid()));
  ASSERT_EQ(5u, root.global_index(t4->uuid()));

  ASSERT_EQ(0u, root.local_index(g1->uuid()));
  ASSERT_EQ(0u, root.local_index(t1->uuid()));
  ASSERT_EQ(1u, root.local_index(t3->uuid()));
  ASSERT_EQ(2u, root.local_index(g2->uuid()));
  ASSERT_EQ(0u, root.local_index(t2->uuid()));
  ASSERT_EQ(1u, root.local_index(t4->uuid()));

  // root
  // -- t4
  // -- g1
  // ---- t1
  // ---- t3
  // ---- g2
  // ------ t2
  root.move_down(g1->uuid());

  ASSERT_EQ(0u, root.global_index(t4->uuid()));
  ASSERT_EQ(1u, root.global_index(g1->uuid()));
  ASSERT_EQ(2u, root.global_index(t1->uuid()));
  ASSERT_EQ(3u, root.global_index(t3->uuid()));
  ASSERT_EQ(4u, root.global_index(g2->uuid()));
  ASSERT_EQ(5u, root.global_index(t2->uuid()));

  ASSERT_EQ(0u, root.local_index(t4->uuid()));
  ASSERT_EQ(1u, root.local_index(g1->uuid()));
  ASSERT_EQ(0u, root.local_index(t1->uuid()));
  ASSERT_EQ(1u, root.local_index(t3->uuid()));
  ASSERT_EQ(2u, root.local_index(g2->uuid()));
  ASSERT_EQ(0u, root.local_index(t2->uuid()));
}

TEST(GroupLayer, SiblingCount)
{
  GroupLayer root;
  ASSERT_THROW(root.sibling_count(root.uuid()), TactileError);

  auto t1 = std::make_shared<TileLayer>();
  auto t2 = std::make_shared<TileLayer>();
  auto t3 = std::make_shared<TileLayer>();
  auto g1 = std::make_shared<GroupLayer>();
  auto g2 = std::make_shared<GroupLayer>();
  auto o1 = std::make_shared<ObjectLayer>();

  root.add(g1);
  root.add(g1->uuid(), t1);
  root.add(g1->uuid(), t2);
  root.add(g2);
  root.add(g2->uuid(), o1);
  root.add(t3);

  ASSERT_EQ(2u, root.sibling_count(g1->uuid()));
  ASSERT_EQ(2u, root.sibling_count(g2->uuid()));
  ASSERT_EQ(2u, root.sibling_count(t3->uuid()));

  ASSERT_EQ(1u, root.sibling_count(t1->uuid()));
  ASSERT_EQ(1u, root.sibling_count(t2->uuid()));

  ASSERT_EQ(0u, root.sibling_count(o1->uuid()));
}

TEST(GroupLayer, LocalIndex)
{
  GroupLayer root;
  ASSERT_THROW(root.local_index(root.uuid()), TactileError);

  auto layer1 = std::make_shared<TileLayer>();
  auto layer2 = std::make_shared<TileLayer>();
  auto layer3 = std::make_shared<TileLayer>();

  root.add(layer1);
  ASSERT_EQ(0u, root.local_index(layer1->uuid()));

  root.add(layer2);
  root.add(layer3);

  ASSERT_EQ(0u, root.local_index(layer1->uuid()));
  ASSERT_EQ(1u, root.local_index(layer2->uuid()));
  ASSERT_EQ(2u, root.local_index(layer3->uuid()));
}

TEST(GroupLayer, GlobalIndex)
{
  GroupLayer root;
  ASSERT_THROW(root.global_index(root.uuid()), TactileError);

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
  root.add(t1);
  root.add(g1);
  root.add(g1->uuid(), g2);
  root.add(g2->uuid(), t2);
  root.add(g1->uuid(), t3);

  ASSERT_EQ(0u, root.global_index(t1->uuid()));
  ASSERT_EQ(1u, root.global_index(g1->uuid()));
  ASSERT_EQ(2u, root.global_index(g2->uuid()));
  ASSERT_EQ(3u, root.global_index(t2->uuid()));
  ASSERT_EQ(4u, root.global_index(t3->uuid()));
}

TEST(GroupLayer, CanMoveUp)
{
  GroupLayer root;
  ASSERT_THROW(root.can_move_up(root.uuid()), TactileError);

  auto t1 = std::make_shared<TileLayer>();
  auto t2 = std::make_shared<TileLayer>();
  auto t3 = std::make_shared<TileLayer>();
  auto g1 = std::make_shared<GroupLayer>();

  root.add(t1);
  root.add(g1);
  root.add(g1->uuid(), t2);
  root.add(t3);

  ASSERT_FALSE(root.can_move_up(t1->uuid()));
  ASSERT_TRUE(root.can_move_up(g1->uuid()));
  ASSERT_FALSE(root.can_move_up(t2->uuid()));
  ASSERT_TRUE(root.can_move_up(t3->uuid()));
}

TEST(GroupLayer, CanMoveDown)
{
  GroupLayer root;
  ASSERT_THROW(root.can_move_down(root.uuid()), TactileError);

  auto t1 = std::make_shared<TileLayer>();
  auto t2 = std::make_shared<TileLayer>();
  auto o1 = std::make_shared<ObjectLayer>();
  auto g1 = std::make_shared<GroupLayer>();

  root.add(g1);
  root.add(g1->uuid(), t1);
  root.add(g1->uuid(), o1);
  root.add(t2);

  ASSERT_TRUE(root.can_move_down(g1->uuid()));
  ASSERT_TRUE(root.can_move_down(t1->uuid()));
  ASSERT_FALSE(root.can_move_down(o1->uuid()));
  ASSERT_FALSE(root.can_move_down(t2->uuid()));
}

}  // namespace tactile::test
