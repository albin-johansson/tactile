// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/move_layer_up_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;

class MoveLayerUpCommandTest : public testing::Test {
 protected:
  MoveLayerUpCommandTest()
    : mDocument {make_unique<OrthogonalMap>(MatrixExtent {8, 5}, Int2 {32, 32})},
      mLayer1 {make_shared<ObjectLayer>()},
      mLayer2 {make_shared<ObjectLayer>()},
      mLayer3 {make_shared<ObjectLayer>()}
  {
    auto& root_layer = mDocument.map().root_layer();
    root_layer.append_layer(mLayer1);
    root_layer.append_layer(mLayer2);
    root_layer.append_layer(mLayer3);
  }

  MapDocument mDocument;
  Shared<ObjectLayer> mLayer1;
  Shared<ObjectLayer> mLayer2;
  Shared<ObjectLayer> mLayer3;
};

/// \trace tactile::MoveLayerUpCommand::redo
/// \trace tactile::MoveLayerUpCommand::undo
TEST_F(MoveLayerUpCommandTest, RedoUndo)
{
  auto& map = mDocument.map();
  auto& root_layer = map.root_layer();

  const auto layer1_uuid = mLayer1->meta().uuid();
  const auto layer2_uuid = mLayer2->meta().uuid();
  const auto layer3_uuid = mLayer3->meta().uuid();

  EXPECT_EQ(root_layer.get_layer_local_index(layer1_uuid), 0);
  EXPECT_EQ(root_layer.get_layer_local_index(layer2_uuid), 1);
  EXPECT_EQ(root_layer.get_layer_local_index(layer3_uuid), 2);

  MoveLayerUpCommand command {&mDocument, layer2_uuid};

  command.redo();

  EXPECT_EQ(root_layer.get_layer_local_index(layer2_uuid), 0);
  EXPECT_EQ(root_layer.get_layer_local_index(layer1_uuid), 1);
  EXPECT_EQ(root_layer.get_layer_local_index(layer3_uuid), 2);

  command.undo();

  EXPECT_EQ(root_layer.get_layer_local_index(layer1_uuid), 0);
  EXPECT_EQ(root_layer.get_layer_local_index(layer2_uuid), 1);
  EXPECT_EQ(root_layer.get_layer_local_index(layer3_uuid), 2);
}
