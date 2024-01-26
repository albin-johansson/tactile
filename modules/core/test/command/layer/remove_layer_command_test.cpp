// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/remove_layer_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/sparse_tile_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;

class RemoveLayerCommandTest : public testing::Test {
 protected:
  RemoveLayerCommandTest()
    : mDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})},
      mGroupLayer {make_shared<GroupLayer>()},
      mNestedTileLayer {make_shared<SparseTileLayer>(mDocument.map().extent())},
      mObjectLayer {make_shared<ObjectLayer>()}
  {
    auto& root_layer = mDocument.map().root_layer();
    root_layer.append_layer(mGroupLayer);
    root_layer.append_layer_to(mGroupLayer->meta().uuid(), mNestedTileLayer);
    root_layer.append_layer(mObjectLayer);
  }

  MapDocument mDocument;
  Shared<GroupLayer> mGroupLayer;
  Shared<TileLayer> mNestedTileLayer;
  Shared<ObjectLayer> mObjectLayer;
  const ssize mInitialLayerCount {3};
};

/// \trace tactile::RemoveLayerCommand::redo
/// \trace tactile::RemoveLayerCommand::undo
TEST_F(RemoveLayerCommandTest, RedoUndo)
{
  auto& map = mDocument.map();
  auto& root_layer = map.root_layer();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount);

  const auto target_layer_uuid = mObjectLayer->meta().uuid();
  const auto target_layer_index = root_layer.get_layer_local_index(target_layer_uuid);

  RemoveLayerCommand command {&mDocument, target_layer_uuid};

  command.redo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount - 1);
  EXPECT_EQ(root_layer.find_object_layer(target_layer_uuid), nullptr);

  command.undo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount);
  EXPECT_EQ(root_layer.find_object_layer(target_layer_uuid), mObjectLayer.get());
  EXPECT_EQ(root_layer.get_layer_local_index(target_layer_uuid), target_layer_index);
}

/// \trace tactile::RemoveLayerCommand::redo
/// \trace tactile::RemoveLayerCommand::undo
TEST_F(RemoveLayerCommandTest, RedoUndoWithNestedTargetLayer)
{
  auto& map = mDocument.map();
  auto& root_layer = map.root_layer();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount);

  const auto target_layer_uuid = mNestedTileLayer->meta().uuid();
  const auto target_layer_index = root_layer.get_layer_local_index(target_layer_uuid);

  RemoveLayerCommand command {&mDocument, target_layer_uuid};

  command.redo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount - 1);
  EXPECT_EQ(root_layer.find_tile_layer(target_layer_uuid), nullptr);
  EXPECT_EQ(mGroupLayer->find_tile_layer(target_layer_uuid), nullptr);

  command.undo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount);
  EXPECT_EQ(root_layer.find_tile_layer(target_layer_uuid), mNestedTileLayer.get());
  EXPECT_EQ(mGroupLayer->find_tile_layer(target_layer_uuid), mNestedTileLayer.get());
  EXPECT_EQ(root_layer.get_layer_local_index(target_layer_uuid), target_layer_index);
}
