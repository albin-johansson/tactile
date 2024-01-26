// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

#include "tactile/core/command/layer/update_layer_visibility_command.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/dense_tile_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;

class UpdateLayerVisibilityCommandTest : public testing::Test {
 protected:
  UpdateLayerVisibilityCommandTest()
    : mDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})},
      mLayer {make_shared<DenseTileLayer>(mDocument.map().extent())}
  {
    auto& map = mDocument.map();
    map.root_layer().append_layer(mLayer);
  }

  MapDocument mDocument;
  Shared<TileLayer> mLayer;
};

/// \tests tactile::UpdateLayerVisibilityCommand::redo
/// \tests tactile::UpdateLayerVisibilityCommand::undo
TEST_F(UpdateLayerVisibilityCommandTest, RedoUndo)
{
  ASSERT_TRUE(mLayer->is_visible());

  UpdateLayerVisibilityCommand command {&mDocument, mLayer->meta().uuid(), false};

  command.redo();
  EXPECT_FALSE(mLayer->is_visible());

  command.undo();
  EXPECT_TRUE(mLayer->is_visible());
}
