// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/group_layer.hpp"

#include <gtest/gtest.h>

#include "tactile/core/layer/dense_tile_layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/sparse_tile_layer.hpp"

namespace {

using namespace tactile;
using namespace tactile::core;

using tactile::int_literals::operator""_z;

namespace {

struct GroupLayerTestConfig final {
  Shared<GroupLayer> root;

  Shared<GroupLayer> g1;
  Shared<GroupLayer> g2;
  Shared<GroupLayer> g3;
  Shared<GroupLayer> g4;

  Shared<TileLayer> t1;
  Shared<TileLayer> t2;
  Shared<TileLayer> t3;
  Shared<TileLayer> t4;

  Shared<ObjectLayer> o1;
  Shared<ObjectLayer> o2;
  Shared<ObjectLayer> o3;
  Shared<ObjectLayer> o4;
};

/**
 * Creates a layer hierarchy for testing.
 *
 * \details
 *    The hierarchy is illustrated in the following diagram.
 *    \code
 *    G (root)
 *    |--> T (t1)
 *    |--> G (g1)
 *    |    |--> G (g2)
 *    |    |    |--> O (o1)
 *    |    |    |--> G (g3)
 *    |    |    |--> O (o2)
 *    |    |--> T (t2)
 *    |    |--> G (g4)
 *    |    |    |--> O (o3)
 *    |    |--> T (t3)
 *    |--> O (o4)
 *    |--> T (t4)
 *    \endcode
 */
[[nodiscard]]
auto make_test_layer_hierarchy() -> GroupLayerTestConfig
{
  const MatrixExtent map_extent {5, 5};

  GroupLayerTestConfig cfg {};

  cfg.root = make_shared<GroupLayer>();

  cfg.g1 = make_shared<GroupLayer>();
  cfg.g2 = make_shared<GroupLayer>();
  cfg.g3 = make_shared<GroupLayer>();
  cfg.g4 = make_shared<GroupLayer>();

  cfg.t1 = make_shared<DenseTileLayer>(map_extent);
  cfg.t2 = make_shared<DenseTileLayer>(map_extent);
  cfg.t3 = make_shared<SparseTileLayer>(map_extent);
  cfg.t4 = make_shared<SparseTileLayer>(map_extent);

  cfg.o1 = make_shared<ObjectLayer>();
  cfg.o2 = make_shared<ObjectLayer>();
  cfg.o3 = make_shared<ObjectLayer>();
  cfg.o4 = make_shared<ObjectLayer>();

  // clang-format off
  cfg.root->append_layer(cfg.t1);
  cfg.root->append_layer(cfg.g1);

      cfg.g1->append_layer(cfg.g2);

          cfg.g2->append_layer(cfg.o1);
          cfg.g2->append_layer(cfg.g3);
          cfg.g2->append_layer(cfg.o2);

      cfg.g1->append_layer(cfg.t2);
      cfg.g1->append_layer(cfg.g4);

          cfg.g4->append_layer(cfg.o3);

      cfg.g1->append_layer(cfg.t3);

  cfg.root->append_layer(cfg.o4);
  cfg.root->append_layer(cfg.t4);
  // clang-format on

  return cfg;
}

}  // namespace

/// \tests tactile::GroupLayer::append_layer
TEST(GroupLayer, AppendLayer)
{
  GroupLayer layer {};
  EXPECT_EQ(layer.layer_count(), 0_z);

  layer.append_layer(make_shared<DenseTileLayer>(5, 5));
  EXPECT_EQ(layer.layer_count(), 1_z);

  layer.append_layer(make_shared<ObjectLayer>());
  EXPECT_EQ(layer.layer_count(), 2_z);

  layer.append_layer(make_shared<GroupLayer>());
  EXPECT_EQ(layer.layer_count(), 3_z);
}

/// \tests tactile::GroupLayer::append_layer_to
TEST(GroupLayer, AppendLayerTo)
{
  /*
   G (root)
   |- G (group1)
   |  |- G (group2)
   |  |  |- O
   |  |  |- G
   |  |  |- T
   |  |- O
   |  |- T
   |- T
   */
  GroupLayer root {};

  auto group1 = make_shared<GroupLayer>();
  auto group2 = make_shared<GroupLayer>();

  const auto group1_uuid = group1->meta().uuid();
  const auto group2_uuid = group2->meta().uuid();

  root.append_layer(group1);
  EXPECT_EQ(root.layer_count(), 1_z);
  EXPECT_EQ(group1->layer_count(), 0_z);
  EXPECT_EQ(group2->layer_count(), 0_z);

  EXPECT_TRUE(root.append_layer_to(group1_uuid, group2));
  EXPECT_EQ(root.layer_count(), 2_z);
  EXPECT_EQ(group1->layer_count(), 1_z);
  EXPECT_EQ(group2->layer_count(), 0_z);

  EXPECT_TRUE(root.append_layer_to(group2_uuid, make_shared<ObjectLayer>()));
  EXPECT_TRUE(root.append_layer_to(group2_uuid, make_shared<GroupLayer>()));
  EXPECT_TRUE(root.append_layer_to(group2_uuid, make_shared<DenseTileLayer>(5, 5)));
  EXPECT_EQ(root.layer_count(), 5_z);
  EXPECT_EQ(group1->layer_count(), 4_z);
  EXPECT_EQ(group2->layer_count(), 3_z);

  EXPECT_TRUE(root.append_layer_to(group1_uuid, make_shared<GroupLayer>()));
  EXPECT_TRUE(root.append_layer_to(group1_uuid, make_shared<DenseTileLayer>(5, 5)));
  EXPECT_EQ(root.layer_count(), 7_z);
  EXPECT_EQ(group1->layer_count(), 6_z);
  EXPECT_EQ(group2->layer_count(), 3_z);

  root.append_layer(make_shared<DenseTileLayer>(5, 5));
  EXPECT_EQ(root.layer_count(), 8_z);
  EXPECT_EQ(group1->layer_count(), 6_z);
  EXPECT_EQ(group2->layer_count(), 3_z);
}

/// \tests tactile::GroupLayer::insert_layer
TEST(GroupLayer, InsertLayer)
{
  auto hierarchy = make_test_layer_hierarchy();

  auto& root = hierarchy.root;
  ASSERT_EQ(root->layer_count(), 12);

  const auto new_layer = make_shared<ObjectLayer>();
  const auto new_layer_uuid = new_layer->meta().uuid();

  const ssize target_index = 2;

  EXPECT_EQ(root->insert_layer(new_layer, target_index), kOK);
  EXPECT_EQ(root->layer_count(), 13);
  EXPECT_EQ(root->get_layer_local_index(new_layer_uuid), target_index);
  EXPECT_EQ(root->get_layer_global_index(new_layer_uuid), 10);
}

/// \tests tactile::GroupLayer::insert_layer
TEST(GroupLayer, InsertLayerAtFront)
{
  auto hierarchy = make_test_layer_hierarchy();

  auto& root = hierarchy.root;
  ASSERT_EQ(root->layer_count(), 12);

  const auto new_layer = make_shared<ObjectLayer>();
  const auto new_layer_uuid = new_layer->meta().uuid();

  EXPECT_EQ(root->insert_layer(new_layer, 0), kOK);
  EXPECT_EQ(root->layer_count(), 13);
  EXPECT_EQ(root->get_layer_local_index(new_layer_uuid), 0);
  EXPECT_EQ(root->get_layer_global_index(new_layer_uuid), 0);
}

/// \tests tactile::GroupLayer::insert_layer
TEST(GroupLayer, InsertLayerAtBack)
{
  auto hierarchy = make_test_layer_hierarchy();

  auto& root = hierarchy.root;
  ASSERT_EQ(root->layer_count(), 12);

  const auto new_layer = make_shared<ObjectLayer>();
  const auto new_layer_uuid = new_layer->meta().uuid();

  const auto target_index = root->top_level_layer_count();

  EXPECT_EQ(root->insert_layer(new_layer, target_index), kOK);
  EXPECT_EQ(root->layer_count(), 13);
  EXPECT_EQ(root->get_layer_local_index(new_layer_uuid), target_index);
  EXPECT_EQ(root->get_layer_global_index(new_layer_uuid), root->layer_count() - 1);
}

/// \tests tactile::GroupLayer::insert_layer_to
TEST(GroupLayer, InsertLayerTo)
{
  auto hierarchy = make_test_layer_hierarchy();

  auto& root = hierarchy.root;
  ASSERT_EQ(root->layer_count(), 12);

  const auto new_layer = make_shared<ObjectLayer>();
  const auto new_layer_uuid = new_layer->meta().uuid();

  const auto parent_layer_uuid = hierarchy.g2->meta().uuid();
  const ssize target_index = 1;

  EXPECT_EQ(root->insert_layer_to(parent_layer_uuid, new_layer, target_index), kOK);
  EXPECT_EQ(root->layer_count(), 13);
  EXPECT_EQ(root->get_layer_local_index(new_layer_uuid), target_index);
  EXPECT_EQ(root->get_layer_global_index(new_layer_uuid), 4);
}

/// \tests tactile::GroupLayer::remove_layer
TEST(GroupLayer, RemoveLayer)
{
  auto hierarchy = make_test_layer_hierarchy();
  auto& root = hierarchy.root;

  ASSERT_EQ(root->layer_count(), 12_z);
  ASSERT_EQ(root->remove_layer(root->meta().uuid()), nullptr);

  EXPECT_EQ(root->remove_layer(hierarchy.g2->meta().uuid()), hierarchy.g2);
  EXPECT_EQ(root->remove_layer(hierarchy.g2->meta().uuid()), nullptr);
  EXPECT_EQ(root->layer_count(), 8_z);

  EXPECT_EQ(root->remove_layer(hierarchy.t3->meta().uuid()), hierarchy.t3);
  EXPECT_EQ(root->remove_layer(hierarchy.t3->meta().uuid()), nullptr);
  EXPECT_EQ(root->layer_count(), 7_z);

  EXPECT_EQ(root->remove_layer(hierarchy.g1->meta().uuid()), hierarchy.g1);
  EXPECT_EQ(root->remove_layer(hierarchy.g1->meta().uuid()), nullptr);
  EXPECT_EQ(root->layer_count(), 3_z);

  EXPECT_EQ(root->remove_layer(hierarchy.t1->meta().uuid()), hierarchy.t1);
  EXPECT_EQ(root->remove_layer(hierarchy.o4->meta().uuid()), hierarchy.o4);
  EXPECT_EQ(root->remove_layer(hierarchy.t4->meta().uuid()), hierarchy.t4);
  EXPECT_EQ(root->layer_count(), 0_z);
}

/// \tests tactile::GroupLayer::duplicate_layer
TEST(GroupLayer, DuplicateLayer)
{
  auto hierarchy = make_test_layer_hierarchy();
  auto& root = hierarchy.root;

  EXPECT_EQ(root->layer_count(), 12_z);
  EXPECT_EQ(root->duplicate_layer(hierarchy.root->meta().uuid()), nullptr);
  EXPECT_EQ(root->layer_count(), 12_z);

  const auto duplicated_g2 = std::dynamic_pointer_cast<GroupLayer>(
      root->duplicate_layer(hierarchy.g2->meta().uuid()));
  EXPECT_EQ(root->layer_count(), 16_z);
  ASSERT_NE(duplicated_g2, nullptr);

  EXPECT_NE(duplicated_g2, hierarchy.g2);
  EXPECT_EQ(duplicated_g2->layer_count(), hierarchy.g2->layer_count());
}

/// \tests tactile::GroupLayer::move_layer_up
TEST(GroupLayer, MoveLayerUp)
{
  auto hierarchy = make_test_layer_hierarchy();
  auto& root = hierarchy.root;

  EXPECT_FALSE(root->move_layer_up(hierarchy.t1->meta().uuid()));
  EXPECT_FALSE(root->move_layer_up(hierarchy.o3->meta().uuid()));

  EXPECT_TRUE(root->move_layer_up(hierarchy.g1->meta().uuid()));

  // clang-format off
  EXPECT_EQ(root->get_layer_global_index(hierarchy.g1->meta().uuid()), 0_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.g2->meta().uuid()), 1_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o1->meta().uuid()), 2_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.g3->meta().uuid()), 3_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.o2->meta().uuid()), 4_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.t2->meta().uuid()), 5_z);
    EXPECT_EQ(root->get_layer_global_index(hierarchy.g4->meta().uuid()), 6_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o3->meta().uuid()), 7_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.t3->meta().uuid()), 8_z);

  EXPECT_EQ(root->get_layer_global_index(hierarchy.t1->meta().uuid()), 9_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.o4->meta().uuid()), 10_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.t4->meta().uuid()), 11_z);
  // clang-format on

  EXPECT_TRUE(root->move_layer_up(hierarchy.g4->meta().uuid()));

  // clang-format off
  EXPECT_EQ(root->get_layer_global_index(hierarchy.g1->meta().uuid()), 0_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.g2->meta().uuid()), 1_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o1->meta().uuid()), 2_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.g3->meta().uuid()), 3_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.o2->meta().uuid()), 4_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.g4->meta().uuid()), 5_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o3->meta().uuid()), 6_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.t2->meta().uuid()), 7_z);
    EXPECT_EQ(root->get_layer_global_index(hierarchy.t3->meta().uuid()), 8_z);

  EXPECT_EQ(root->get_layer_global_index(hierarchy.t1->meta().uuid()), 9_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.o4->meta().uuid()), 10_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.t4->meta().uuid()), 11_z);
  // clang-format on
}

/// \tests tactile::GroupLayer::move_layer_down
TEST(GroupLayer, MoveLayerDown)
{
  auto hierarchy = make_test_layer_hierarchy();
  auto& root = hierarchy.root;

  EXPECT_FALSE(root->move_layer_down(hierarchy.t4->meta().uuid()));
  EXPECT_FALSE(root->move_layer_down(hierarchy.o2->meta().uuid()));

  EXPECT_TRUE(root->move_layer_down(hierarchy.g2->meta().uuid()));

  // clang-format off
  EXPECT_EQ(root->get_layer_global_index(hierarchy.t1->meta().uuid()), 0_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.g1->meta().uuid()), 1_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.t2->meta().uuid()), 2_z);
    EXPECT_EQ(root->get_layer_global_index(hierarchy.g2->meta().uuid()), 3_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o1->meta().uuid()), 4_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.g3->meta().uuid()), 5_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.o2->meta().uuid()), 6_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.g4->meta().uuid()), 7_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o3->meta().uuid()), 8_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.t3->meta().uuid()), 9_z);

  EXPECT_EQ(root->get_layer_global_index(hierarchy.o4->meta().uuid()), 10_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.t4->meta().uuid()), 11_z);
  // clang-format on

  EXPECT_TRUE(root->move_layer_down(hierarchy.g3->meta().uuid()));

  // clang-format off
  EXPECT_EQ(root->get_layer_global_index(hierarchy.t1->meta().uuid()), 0_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.g1->meta().uuid()), 1_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.t2->meta().uuid()), 2_z);
    EXPECT_EQ(root->get_layer_global_index(hierarchy.g2->meta().uuid()), 3_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o1->meta().uuid()), 4_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.o2->meta().uuid()), 5_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.g3->meta().uuid()), 6_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.g4->meta().uuid()), 7_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o3->meta().uuid()), 8_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.t3->meta().uuid()), 9_z);

  EXPECT_EQ(root->get_layer_global_index(hierarchy.o4->meta().uuid()), 10_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.t4->meta().uuid()), 11_z);
  // clang-format on
}

/// \tests tactile::GroupLayer::can_move_layer_up
TEST(GroupLayer, CanMoveLayerUp)
{
  auto hierarchy = make_test_layer_hierarchy();
  auto& root = hierarchy.root;

  // clang-format off
  EXPECT_FALSE(root->can_move_layer_up(hierarchy.t1->meta().uuid()));
  EXPECT_TRUE(root->can_move_layer_up(hierarchy.g1->meta().uuid()));

    EXPECT_FALSE(root->can_move_layer_up(hierarchy.g2->meta().uuid()));

      EXPECT_FALSE(root->can_move_layer_up(hierarchy.o1->meta().uuid()));
      EXPECT_TRUE(root->can_move_layer_up(hierarchy.g3->meta().uuid()));
      EXPECT_TRUE(root->can_move_layer_up(hierarchy.o2->meta().uuid()));

    EXPECT_TRUE(root->can_move_layer_up(hierarchy.t2->meta().uuid()));
    EXPECT_TRUE(root->can_move_layer_up(hierarchy.g4->meta().uuid()));

      EXPECT_FALSE(root->can_move_layer_up(hierarchy.o3->meta().uuid()));

    EXPECT_TRUE(root->can_move_layer_up(hierarchy.t3->meta().uuid()));

  EXPECT_TRUE(root->can_move_layer_up(hierarchy.o4->meta().uuid()));
  EXPECT_TRUE(root->can_move_layer_up(hierarchy.t4->meta().uuid()));
  // clang-format on
}

/// \tests tactile::GroupLayer::can_move_layer_down
TEST(GroupLayer, CanMoveLayerDown)
{
  auto hierarchy = make_test_layer_hierarchy();
  auto& root = hierarchy.root;

  // clang-format off
  EXPECT_TRUE(root->can_move_layer_down(hierarchy.t1->meta().uuid()));
  EXPECT_TRUE(root->can_move_layer_down(hierarchy.g1->meta().uuid()));

    EXPECT_TRUE(root->can_move_layer_down(hierarchy.g2->meta().uuid()));

      EXPECT_TRUE(root->can_move_layer_down(hierarchy.o1->meta().uuid()));
      EXPECT_TRUE(root->can_move_layer_down(hierarchy.g3->meta().uuid()));
      EXPECT_FALSE(root->can_move_layer_down(hierarchy.o2->meta().uuid()));

    EXPECT_TRUE(root->can_move_layer_down(hierarchy.t2->meta().uuid()));
    EXPECT_TRUE(root->can_move_layer_down(hierarchy.g4->meta().uuid()));

      EXPECT_FALSE(root->can_move_layer_down(hierarchy.o3->meta().uuid()));

    EXPECT_FALSE(root->can_move_layer_down(hierarchy.t3->meta().uuid()));

  EXPECT_TRUE(root->can_move_layer_down(hierarchy.o4->meta().uuid()));
  EXPECT_FALSE(root->can_move_layer_down(hierarchy.t4->meta().uuid()));
  // clang-format on
}

/// \tests tactile::GroupLayer::get_local_layer_index
TEST(GroupLayer, GetLocalLayerIndex)
{
  auto hierarchy = make_test_layer_hierarchy();
  auto& root = hierarchy.root;

  // clang-format off
  EXPECT_EQ(root->get_layer_local_index(hierarchy.t1->meta().uuid()), 0_z);
  EXPECT_EQ(root->get_layer_local_index(hierarchy.g1->meta().uuid()), 1_z);

    EXPECT_EQ(root->get_layer_local_index(hierarchy.g2->meta().uuid()), 0_z);

      EXPECT_EQ(root->get_layer_local_index(hierarchy.o1->meta().uuid()), 0_z);
      EXPECT_EQ(root->get_layer_local_index(hierarchy.g3->meta().uuid()), 1_z);
      EXPECT_EQ(root->get_layer_local_index(hierarchy.o2->meta().uuid()), 2_z);

    EXPECT_EQ(root->get_layer_local_index(hierarchy.t2->meta().uuid()), 1_z);
    EXPECT_EQ(root->get_layer_local_index(hierarchy.g4->meta().uuid()), 2_z);

      EXPECT_EQ(root->get_layer_local_index(hierarchy.o3->meta().uuid()), 0_z);

    EXPECT_EQ(root->get_layer_local_index(hierarchy.t3->meta().uuid()), 3_z);

  EXPECT_EQ(root->get_layer_local_index(hierarchy.o4->meta().uuid()), 2_z);
  EXPECT_EQ(root->get_layer_local_index(hierarchy.t4->meta().uuid()), 3_z);
  // clang-format on
}

/// \tests tactile::GroupLayer::get_global_layer_index
TEST(GroupLayer, GetGlobalLayerIndex)
{
  auto hierarchy = make_test_layer_hierarchy();
  auto& root = hierarchy.root;

  // clang-format off
  EXPECT_EQ(root->get_layer_global_index(hierarchy.t1->meta().uuid()), 0_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.g1->meta().uuid()), 1_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.g2->meta().uuid()), 2_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o1->meta().uuid()), 3_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.g3->meta().uuid()), 4_z);
      EXPECT_EQ(root->get_layer_global_index(hierarchy.o2->meta().uuid()), 5_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.t2->meta().uuid()), 6_z);
    EXPECT_EQ(root->get_layer_global_index(hierarchy.g4->meta().uuid()), 7_z);

      EXPECT_EQ(root->get_layer_global_index(hierarchy.o3->meta().uuid()), 8_z);

    EXPECT_EQ(root->get_layer_global_index(hierarchy.t3->meta().uuid()), 9_z);

  EXPECT_EQ(root->get_layer_global_index(hierarchy.o4->meta().uuid()), 10_z);
  EXPECT_EQ(root->get_layer_global_index(hierarchy.t4->meta().uuid()), 11_z);
  // clang-format on
}

/// \tests tactile::GroupLayer::begin
/// \tests tactile::GroupLayer::end
TEST(GroupLayer, NonConstIteration)
{
  auto hierarchy = make_test_layer_hierarchy();
  auto& root = *hierarchy.root;

  int count = 0;
  for (auto& layer : root) {
    EXPECT_NE(root.find_layer(layer.meta().uuid()), nullptr);
    ++count;
  }

  EXPECT_EQ(count, 12);
}

/// \tests tactile::GroupLayer::begin
/// \tests tactile::GroupLayer::end
TEST(GroupLayer, ConstIteration)
{
  const auto hierarchy = make_test_layer_hierarchy();
  const auto& root = *hierarchy.root;

  int count = 0;
  for (const auto& layer : root) {
    EXPECT_NE(root.find_layer(layer.meta().uuid()), nullptr);
    ++count;
  }

  EXPECT_EQ(count, 12);
}

}  // namespace
