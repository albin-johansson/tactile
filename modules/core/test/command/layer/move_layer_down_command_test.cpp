// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/move_layer_down_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/dense_tile_layer.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;

class MoveLayerDownCommandTest : public testing::Test {
 protected:
  MoveLayerDownCommandTest()
    : mDocument {make_unique<OrthogonalMap>(MatrixExtent {7, 7}, Int2 {32, 32})},
      mGroupLayer {make_shared<GroupLayer>()},
      mLayer1 {make_shared<DenseTileLayer>(mDocument.map().extent())},
      mLayer2 {make_shared<DenseTileLayer>(mDocument.map().extent())},
      mLayer3 {make_shared<DenseTileLayer>(mDocument.map().extent())}
  {
    auto& root_layer = mDocument.map().root_layer();
    root_layer.append_layer(mGroupLayer);

    const auto group_layer_uuid = mGroupLayer->meta().uuid();
    root_layer.append_layer_to(group_layer_uuid, mLayer1);
    root_layer.append_layer_to(group_layer_uuid, mLayer2);
    root_layer.append_layer_to(group_layer_uuid, mLayer3);
  }

  MapDocument mDocument;
  Shared<GroupLayer> mGroupLayer;
  Shared<TileLayer> mLayer1;
  Shared<TileLayer> mLayer2;
  Shared<TileLayer> mLayer3;
};

/// \trace tactile::MoveLayerDownCommand::redo
/// \trace tactile::MoveLayerDownCommand::undo
TEST_F(MoveLayerDownCommandTest, RedoUndo)
{
  auto& map = mDocument.map();
  auto& root_layer = map.root_layer();

  const auto layer1_uuid = mLayer1->meta().uuid();
  const auto layer2_uuid = mLayer2->meta().uuid();
  const auto layer3_uuid = mLayer3->meta().uuid();

  EXPECT_EQ(root_layer.get_layer_local_index(layer1_uuid), 0);
  EXPECT_EQ(root_layer.get_layer_local_index(layer2_uuid), 1);
  EXPECT_EQ(root_layer.get_layer_local_index(layer3_uuid), 2);

  MoveLayerDownCommand command {&mDocument, layer2_uuid};

  command.redo();

  EXPECT_EQ(root_layer.get_layer_local_index(layer1_uuid), 0);
  EXPECT_EQ(root_layer.get_layer_local_index(layer3_uuid), 1);
  EXPECT_EQ(root_layer.get_layer_local_index(layer2_uuid), 2);

  command.undo();

  EXPECT_EQ(root_layer.get_layer_local_index(layer1_uuid), 0);
  EXPECT_EQ(root_layer.get_layer_local_index(layer2_uuid), 1);
  EXPECT_EQ(root_layer.get_layer_local_index(layer3_uuid), 2);
}
