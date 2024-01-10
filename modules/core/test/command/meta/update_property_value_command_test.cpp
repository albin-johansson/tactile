// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/update_property_value_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/map/orthogonal_map.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/meta/attribute.hpp"

using namespace tactile;

class UpdatePropertyValueCommandTest : public testing::Test {
 protected:
  UpdatePropertyValueCommandTest()
    : mMapDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})}
  {
    auto& map = mMapDocument.map();
    map.meta().add_property(mPropertyName) = mOldPropertyValue;
  }

  MapDocument mMapDocument;
  const String mPropertyName {"123"};
  const Attribute mOldPropertyValue {1.0f};
  const Attribute mNewPropertyValue {2.0f};
};

/// \tests tactile::UpdatePropertyValueCommand::redo
/// \tests tactile::UpdatePropertyValueCommand::undo
TEST_F(UpdatePropertyValueCommandTest, RedoUndo)
{
  auto& metadata = mMapDocument.map().meta();
  const auto context_uuid = metadata.uuid();

  ASSERT_TRUE(metadata.has_property(mPropertyName));
  ASSERT_EQ(metadata.get_property(mPropertyName), mOldPropertyValue);

  UpdatePropertyValueCommand command {&mMapDocument,
                                      context_uuid,
                                      mPropertyName,
                                      mNewPropertyValue};

  command.redo();
  EXPECT_EQ(metadata.get_property(mPropertyName), mNewPropertyValue);

  command.undo();
  EXPECT_EQ(metadata.get_property(mPropertyName), mOldPropertyValue);
}
