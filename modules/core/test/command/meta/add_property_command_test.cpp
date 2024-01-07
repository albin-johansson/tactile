// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/add_property_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/map/layer/tile_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;

class AddPropertyCommandTest : public testing::Test {
 protected:
  AddPropertyCommandTest()
    : mMapDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})},
      mTileLayer {make_shared<TileLayer>(mMapDocument.map().extent())}
  {
    auto& map = mMapDocument.map();
    map.root_layer().append_layer(mTileLayer);
  }

  MapDocument mMapDocument;
  Shared<TileLayer> mTileLayer;
};

/// \tests tactile::AddPropertyCommand::redo
/// \tests tactile::AddPropertyCommand::undo
TEST_F(AddPropertyCommandTest, RedoUndo)
{
  const auto tile_layer_uuid = mTileLayer->meta().uuid();

  const String property_name {"foobar"};
  const AttributeType property_type {AttributeType::kFloat3};

  AddPropertyCommand command {&mMapDocument,
                              tile_layer_uuid,
                              property_name,
                              property_type};

  command.redo();
  ASSERT_TRUE(mTileLayer->meta().has_property(property_name));
  EXPECT_EQ(mTileLayer->meta().get_property(property_name).get_type(), property_type);

  command.undo();
  EXPECT_FALSE(mTileLayer->meta().has_property(property_name));
}
