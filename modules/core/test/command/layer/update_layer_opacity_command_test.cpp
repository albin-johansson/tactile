// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/update_layer_opacity_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;
using namespace tactile::core;

class UpdateLayerOpacityCommandTest : public testing::Test {
 public:
  UpdateLayerOpacityCommandTest()
    : mDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})},
      mLayer {make_shared<ObjectLayer>()}
  {
    mDocument.map().root_layer().append_layer(mLayer);
  }

  MapDocument mDocument;
  Shared<ObjectLayer> mLayer;
};

/// \tests tactile::UpdateLayerOpacityCommand::redo
/// \tests tactile::UpdateLayerOpacityCommand::undo
TEST_F(UpdateLayerOpacityCommandTest, RedoUndo)
{
  ASSERT_EQ(mLayer->get_opacity(), 1.0f);

  UpdateLayerOpacityCommand command {&mDocument, mLayer->meta().uuid(), 0.5f};

  command.redo();
  EXPECT_EQ(mLayer->get_opacity(), 0.5f);

  command.undo();
  EXPECT_EQ(mLayer->get_opacity(), 1.0f);
}

/// \tests tactile::UpdateLayerOpacityCommand::merge_with
TEST_F(UpdateLayerOpacityCommandTest, MergeWith)
{
  UpdateLayerOpacityCommand command1 {&mDocument, mLayer->meta().uuid(), 0.2f};
  const UpdateLayerOpacityCommand command2 {&mDocument, mLayer->meta().uuid(), 0.3f};
  const UpdateLayerOpacityCommand command3 {&mDocument, mLayer->meta().uuid(), 0.4f};
  const UpdateLayerOpacityCommand command4 {&mDocument, UUID::generate(), 0.5f};

  EXPECT_TRUE(command1.merge_with(&command2));
  EXPECT_TRUE(command1.merge_with(&command3));
  EXPECT_FALSE(command1.merge_with(&command4));

  command1.redo();
  EXPECT_EQ(mLayer->get_opacity(), 0.4f);

  command1.undo();
  EXPECT_EQ(mLayer->get_opacity(), 1.0f);
}
