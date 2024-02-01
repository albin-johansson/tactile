// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/create_layer_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;
using namespace tactile::core;

class CreateLayerCommandTest : public testing::Test {
 protected:
  CreateLayerCommandTest()
    : mDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})},
      mGroupLayer {make_shared<GroupLayer>()},
      mObjectLayer {make_shared<ObjectLayer>()}
  {
    auto& root_layer = mDocument.map().root_layer();
    root_layer.append_layer(mGroupLayer);
    root_layer.append_layer(mObjectLayer);
  }

  MapDocument mDocument;
  Shared<GroupLayer> mGroupLayer;
  Shared<ObjectLayer> mObjectLayer;
  const ssize mInitialLayerCount {2};
};

/// \trace tactile::CreateLayerCommand::redo
/// \trace tactile::CreateLayerCommand::undo
TEST_F(CreateLayerCommandTest, RedoUndoWithNoActiveLayer)
{
  auto& map = mDocument.map();
  auto& root_layer = map.root_layer();

  map.set_active_layer(kNothing);
  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount);

  CreateLayerCommand command {&mDocument, LayerType::kTileLayer};

  command.redo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount + 1);
  EXPECT_EQ(mGroupLayer->layer_count(), 0);

  command.undo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount);
  EXPECT_EQ(mGroupLayer->layer_count(), 0);
}

/// \trace tactile::CreateLayerCommand::redo
/// \trace tactile::CreateLayerCommand::undo
TEST_F(CreateLayerCommandTest, RedoUndoWithActiveGroupLayer)
{
  auto& map = mDocument.map();
  auto& root_layer = map.root_layer();

  const auto group_layer_uuid = mGroupLayer->meta().uuid();

  map.set_active_layer(group_layer_uuid);
  EXPECT_EQ(map.active_layer_uuid(), group_layer_uuid);

  CreateLayerCommand command {&mDocument, LayerType::kObjectLayer};

  command.redo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount + 1);
  EXPECT_EQ(mGroupLayer->layer_count(), 1);

  command.undo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount);
  EXPECT_EQ(mGroupLayer->layer_count(), 0);

  // This tests the branch where the cached parent layer UUID is used.
  map.set_active_layer(kNothing);
  command.redo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount + 1);
  EXPECT_EQ(mGroupLayer->layer_count(), 1);
}

/// \trace tactile::CreateLayerCommand::redo
/// \trace tactile::CreateLayerCommand::undo
TEST_F(CreateLayerCommandTest, RedoUndoWithActiveLayer)
{
  auto& map = mDocument.map();
  auto& root_layer = map.root_layer();

  map.set_active_layer(mObjectLayer->meta().uuid());
  EXPECT_EQ(map.active_layer_uuid(), mObjectLayer->meta().uuid());

  CreateLayerCommand command {&mDocument, LayerType::kGroupLayer};

  command.redo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount + 1);
  EXPECT_EQ(mGroupLayer->layer_count(), 0);

  command.undo();

  EXPECT_EQ(root_layer.layer_count(), mInitialLayerCount);
  EXPECT_EQ(mGroupLayer->layer_count(), 0);
}