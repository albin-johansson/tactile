// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/remove_property_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/meta/attribute.hpp"

using namespace tactile;
using namespace tactile::core;

class RemovePropertyCommandTest : public testing::Test {
 protected:
  RemovePropertyCommandTest()
    : mMapDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})}
  {
    auto& map = mMapDocument.map();
    map.meta().add_property(mPropertyName) = mPropertyValue;
  }

  MapDocument mMapDocument;
  const String mPropertyName {"foobar"};
  const Attribute mPropertyValue {42};
};

/// \tests tactile::RemovePropertyCommand::redo
/// \tests tactile::RemovePropertyCommand::undo
TEST_F(RemovePropertyCommandTest, RedoUndo)
{
  auto& metadata = mMapDocument.map().meta();
  const auto context_uuid = metadata.uuid();

  ASSERT_TRUE(metadata.has_property(mPropertyName));
  ASSERT_EQ(metadata.get_property(mPropertyName), mPropertyValue);

  RemovePropertyCommand command {&mMapDocument, context_uuid, mPropertyName};

  command.redo();
  EXPECT_FALSE(metadata.has_property(mPropertyName));

  command.undo();
  ASSERT_TRUE(metadata.has_property(mPropertyName));
  EXPECT_EQ(metadata.get_property(mPropertyName), mPropertyValue);
}
