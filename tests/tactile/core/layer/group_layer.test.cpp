// Copyright (C) 2025 Albin Johansson

#include <gmock/gmock.h>
#include <gtest/gtest.h>

import tactile.core.ext.std;
import tactile.core.layer;
import tactile.core.prelude;

namespace tactile::tests {
namespace {

using testing::Const;
using testing::ContainerEq;

// Root
// |--> Layer 1 (G)
//      |--> Layer 2 (A)
//      \--> Layer 3 (G)
//           \--> Layer 4 (A)
//           \--> Layer 5 (T)
//           \--> Layer 6 (A)
// \--> Layer 7 (A)
struct TestHierarchy final
{
  LayerID layer1_id;
  LayerID layer2_id;
  LayerID layer3_id;
  LayerID layer4_id;
  LayerID layer5_id;
  LayerID layer6_id;
  LayerID layer7_id;
  LayerID invalid_id;
};

struct TestVisitor final : ILayerVisitor
{
  Vector<LayerID> layer_sequence {};
  isize group_layers {0};
  isize tile_layers {0};
  isize annotation_layers {0};

  void on_group_layer(GroupLayer& layer) override
  {
    layer_sequence.push_back(layer.info().id());
    ++group_layers;
  }

  void on_tile_layer(TileLayer& layer) override
  {
    layer_sequence.push_back(layer.info().id());
    ++tile_layers;
  }

  void on_annotation_layer(AnnotationLayer& layer) override
  {
    layer_sequence.push_back(layer.info().id());
    ++annotation_layers;
  }
};

struct TestConstVisitor final : IConstLayerVisitor
{
  Vector<LayerID> layer_sequence {};
  isize group_layers {0};
  isize tile_layers {0};
  isize annotation_layers {0};

  void on_group_layer(const GroupLayer& layer) override
  {
    layer_sequence.push_back(layer.info().id());
    ++group_layers;
  }

  void on_tile_layer(const TileLayer& layer) override
  {
    layer_sequence.push_back(layer.info().id());
    ++tile_layers;
  }

  void on_annotation_layer(const AnnotationLayer& layer) override
  {
    layer_sequence.push_back(layer.info().id());
    ++annotation_layers;
  }
};

class GroupLayerTest : public testing::Test
{
 protected:
  LayerID m_next_layer_id {1};
  LayerID m_group_id {m_next_layer_id++};
  GroupLayer m_root {m_group_id};

  [[nodiscard]]
  auto prepare_test_hierarchy() -> TestHierarchy
  {
    TestHierarchy hierarchy {};
    hierarchy.layer1_id = m_next_layer_id++;
    hierarchy.layer2_id = m_next_layer_id++;
    hierarchy.layer3_id = m_next_layer_id++;
    hierarchy.layer4_id = m_next_layer_id++;
    hierarchy.layer5_id = m_next_layer_id++;
    hierarchy.layer6_id = m_next_layer_id++;
    hierarchy.layer7_id = m_next_layer_id++;
    hierarchy.invalid_id = LayerID {123};

    auto layer1 = make_unique<GroupLayer>(hierarchy.layer1_id);
    auto layer2 = make_unique<AnnotationLayer>(hierarchy.layer2_id);
    auto layer3 = make_unique<GroupLayer>(hierarchy.layer3_id);
    auto layer4 = make_unique<AnnotationLayer>(hierarchy.layer4_id);
    auto layer5 = make_unique<TileLayer>(hierarchy.layer5_id);
    auto layer6 = make_unique<AnnotationLayer>(hierarchy.layer6_id);
    auto layer7 = make_unique<AnnotationLayer>(hierarchy.layer7_id);

    layer3->append_layer(std::move(layer4));
    layer3->append_layer(std::move(layer5));
    layer3->append_layer(std::move(layer6));

    layer1->append_layer(std::move(layer2));
    layer1->append_layer(std::move(layer3));

    m_root.append_layer(std::move(layer1));
    m_root.append_layer(std::move(layer7));

    return hierarchy;
  }
};

TEST_F(GroupLayerTest, Visit)
{
  const auto hierarchy = prepare_test_hierarchy();

  const Vector expected_sequence {
    m_root.info().id(),
    hierarchy.layer1_id,
    hierarchy.layer2_id,
    hierarchy.layer3_id,
    hierarchy.layer4_id,
    hierarchy.layer5_id,
    hierarchy.layer6_id,
    hierarchy.layer7_id,
  };

  {
    TestVisitor visitor {};
    m_root.visit(visitor);

    EXPECT_THAT(visitor.layer_sequence, ContainerEq(expected_sequence));
    EXPECT_EQ(visitor.group_layers, 3z);
    EXPECT_EQ(visitor.tile_layers, 1z);
    EXPECT_EQ(visitor.annotation_layers, 4z);
  }

  {
    TestConstVisitor visitor {};
    Const(m_root).visit(visitor);

    EXPECT_THAT(visitor.layer_sequence, ContainerEq(expected_sequence));
    EXPECT_EQ(visitor.group_layers, 3z);
    EXPECT_EQ(visitor.tile_layers, 1z);
    EXPECT_EQ(visitor.annotation_layers, 4z);
  }
}

TEST_F(GroupLayerTest, AppendLayer)
{
  const auto layer_id = m_next_layer_id++;
  EXPECT_EQ(m_root.layer_count(), 0z);
  EXPECT_EQ(m_root.find_layer(layer_id), nullptr);
  EXPECT_EQ(m_root.find_parent_layer(layer_id), nullptr);

  m_root.append_layer(make_unique<AnnotationLayer>(layer_id));

  EXPECT_EQ(m_root.layer_count(), 1z);
  EXPECT_NE(m_root.find_layer(layer_id), nullptr);
  EXPECT_EQ(m_root.find_parent_layer(layer_id), &m_root);

  EXPECT_THROW(m_root.append_layer(nullptr), std::invalid_argument);

  auto layer_with_duplicated_id = make_unique<AnnotationLayer>(layer_id);
  EXPECT_THROW(m_root.append_layer(std::move(layer_with_duplicated_id)),
               std::invalid_argument);
}

TEST_F(GroupLayerTest, AppendLayerTo)
{
  const auto nested_group_id = m_next_layer_id++;
  const auto nested_layer_id = m_next_layer_id++;

  auto nested_group = make_unique<GroupLayer>(nested_group_id);
  auto nested_layer = make_unique<AnnotationLayer>(nested_layer_id);
  auto layer_with_duplicated_id = make_unique<AnnotationLayer>(nested_layer_id);
  auto unused_layer = make_unique<AnnotationLayer>(m_next_layer_id++);

  const auto* nested_group_ptr = nested_group.get();
  const auto* nested_layer_ptr = nested_layer.get();

  m_root.append_layer(std::move(nested_group));
  m_root.append_layer_to(nested_group_id, std::move(nested_layer));

  EXPECT_EQ(m_root.layer_count(), 2z);
  EXPECT_EQ(m_root.find_layer(nested_group_id), nested_group_ptr);
  EXPECT_EQ(m_root.find_layer(nested_layer_id), nested_layer_ptr);

  EXPECT_THROW(m_root.append_layer_to(nested_group_id, nullptr),
               std::invalid_argument);

  EXPECT_THROW(m_root.append_layer_to(nested_group_id,
                                      std::move(layer_with_duplicated_id)),
               std::invalid_argument);

  EXPECT_THROW(m_root.append_layer_to(LayerID {999}, std::move(unused_layer)),
               std::invalid_argument);
}

TEST_F(GroupLayerTest, RemoveLayer)
{
  const auto hierarchy = prepare_test_hierarchy();
  ASSERT_EQ(m_root.layer_count(), 7z);

  const auto removed_layer = m_root.remove_layer(hierarchy.layer3_id);
  EXPECT_EQ(m_root.layer_count(), 3z);
  ASSERT_NE(removed_layer, nullptr);
  EXPECT_EQ(removed_layer->info().id(), hierarchy.layer3_id);

  const auto* removed_group_layer =
      dynamic_cast<const GroupLayer*>(removed_layer.get());
  ASSERT_NE(removed_group_layer, nullptr);
  EXPECT_EQ(removed_group_layer->layer_count(), 3z);

  EXPECT_EQ(m_root.remove_layer(hierarchy.invalid_id), nullptr);
}

TEST_F(GroupLayerTest, RaiseLayer)
{
  const auto hierarchy = prepare_test_hierarchy();
  ASSERT_EQ(m_root.layer_index_rel(hierarchy.layer6_id), 2z);

  EXPECT_TRUE(m_root.raise_layer(hierarchy.layer6_id).has_value());
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer6_id), 1z);

  EXPECT_TRUE(m_root.raise_layer(hierarchy.layer6_id).has_value());
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer6_id), 0z);

  EXPECT_FALSE(m_root.raise_layer(hierarchy.layer6_id).has_value());
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer6_id), 0z);

  EXPECT_FALSE(m_root.raise_layer(hierarchy.invalid_id).has_value());
}

TEST_F(GroupLayerTest, LowerLayer)
{
  const auto hierarchy = prepare_test_hierarchy();
  ASSERT_EQ(m_root.layer_index_rel(hierarchy.layer4_id), 0z);

  EXPECT_TRUE(m_root.lower_layer(hierarchy.layer4_id).has_value());
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer4_id), 1z);

  EXPECT_TRUE(m_root.lower_layer(hierarchy.layer4_id).has_value());
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer4_id), 2z);

  EXPECT_FALSE(m_root.lower_layer(hierarchy.layer4_id).has_value());
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer4_id), 2z);

  EXPECT_FALSE(m_root.lower_layer(hierarchy.invalid_id).has_value());
}

TEST_F(GroupLayerTest, LayerIndexRel)
{
  const auto hierarchy = prepare_test_hierarchy();

  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer1_id), 0z);
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer2_id), 0z);
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer3_id), 1z);
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer4_id), 0z);
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer5_id), 1z);
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer6_id), 2z);
  EXPECT_EQ(m_root.layer_index_rel(hierarchy.layer7_id), 1z);
}

TEST_F(GroupLayerTest, LayerIndexAbs)
{
  const auto hierarchy = prepare_test_hierarchy();

  EXPECT_EQ(m_root.layer_index_abs(hierarchy.layer1_id), 0z);
  EXPECT_EQ(m_root.layer_index_abs(hierarchy.layer2_id), 1z);
  EXPECT_EQ(m_root.layer_index_abs(hierarchy.layer3_id), 2z);
  EXPECT_EQ(m_root.layer_index_abs(hierarchy.layer4_id), 3z);
  EXPECT_EQ(m_root.layer_index_abs(hierarchy.layer5_id), 4z);
  EXPECT_EQ(m_root.layer_index_abs(hierarchy.layer6_id), 5z);
  EXPECT_EQ(m_root.layer_index_abs(hierarchy.layer7_id), 6z);
}

TEST_F(GroupLayerTest, FindLayer)
{
  const auto hierarchy = prepare_test_hierarchy();

  {
    EXPECT_NE(m_root.find_layer(hierarchy.layer1_id), nullptr);
    EXPECT_NE(m_root.find_layer(hierarchy.layer2_id), nullptr);
    EXPECT_NE(m_root.find_layer(hierarchy.layer4_id), nullptr);
    EXPECT_EQ(m_root.find_layer(hierarchy.invalid_id), nullptr);
  }

  {
    EXPECT_NE(Const(m_root).find_layer(hierarchy.layer1_id), nullptr);
    EXPECT_NE(Const(m_root).find_layer(hierarchy.layer2_id), nullptr);
    EXPECT_NE(Const(m_root).find_layer(hierarchy.layer4_id), nullptr);
    EXPECT_EQ(Const(m_root).find_layer(hierarchy.invalid_id), nullptr);
  }
}

TEST_F(GroupLayerTest, FindParentLayer)
{
  const auto hierarchy = prepare_test_hierarchy();

  const auto* group1 = m_root.find_layer(hierarchy.layer1_id);
  const auto* group2 = m_root.find_layer(hierarchy.layer3_id);
  ASSERT_NE(group1, nullptr);
  ASSERT_NE(group2, nullptr);

  {
    EXPECT_EQ(m_root.find_parent_layer(hierarchy.layer1_id), &m_root);
    EXPECT_EQ(m_root.find_parent_layer(hierarchy.layer2_id), group1);
    EXPECT_EQ(m_root.find_parent_layer(hierarchy.layer4_id), group2);
    EXPECT_EQ(m_root.find_parent_layer(hierarchy.invalid_id), nullptr);
  }

  {
    EXPECT_EQ(Const(m_root).find_parent_layer(hierarchy.layer1_id), &m_root);
    EXPECT_EQ(Const(m_root).find_parent_layer(hierarchy.layer2_id), group1);
    EXPECT_EQ(Const(m_root).find_parent_layer(hierarchy.layer4_id), group2);
    EXPECT_EQ(Const(m_root).find_parent_layer(hierarchy.invalid_id), nullptr);
  }
}

TEST_F(GroupLayerTest, LayerCount)
{
  (void) prepare_test_hierarchy();
  EXPECT_EQ(m_root.layer_count(), 7z);
}

}  // namespace
}  // namespace tactile::tests
