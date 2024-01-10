// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/rename_property_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;

class RenamePropertyCommandTest : public testing::Test {
 protected:
  RenamePropertyCommandTest()
    : mMapDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})}
  {
    auto& map = mMapDocument.map();
    map.meta().add_property(mOldPropertyName) = mPropertyValue;
  }

  MapDocument mMapDocument;
  const String mOldPropertyName {"foo"};
  const String mNewPropertyName {"bar"};
  const Attribute mPropertyValue {"123456"};
};

/// \tests tactile::RenamePropertyCommand::redo
/// \tests tactile::RenamePropertyCommand::undo
TEST_F(RenamePropertyCommandTest, RedoUndo)
{
  auto& metadata = mMapDocument.map().meta();
  const auto context_uuid = metadata.uuid();

  ASSERT_TRUE(metadata.has_property(mOldPropertyName));
  ASSERT_EQ(metadata.get_property(mOldPropertyName), mPropertyValue);

  RenamePropertyCommand command {&mMapDocument,
                                 context_uuid,
                                 mOldPropertyName,
                                 mNewPropertyName};

  command.redo();

  EXPECT_FALSE(metadata.has_property(mOldPropertyName));
  EXPECT_TRUE(metadata.has_property(mNewPropertyName));
  EXPECT_EQ(metadata.get_property(mNewPropertyName), mPropertyValue);

  command.undo();

  EXPECT_TRUE(metadata.has_property(mOldPropertyName));
  EXPECT_FALSE(metadata.has_property(mNewPropertyName));
  EXPECT_EQ(metadata.get_property(mOldPropertyName), mPropertyValue);
}
