// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/update_property_value_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/map/orthogonal_map.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/meta/attribute.hpp"

using namespace tactile;
using namespace tactile::core;

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

/// \tests tactile::UpdatePropertyValueCommand::merge_with
TEST_F(UpdatePropertyValueCommandTest, MergeWith)
{
  auto& metadata = mMapDocument.map().meta();
  const auto context_uuid = metadata.uuid();

  ASSERT_TRUE(metadata.has_property(mPropertyName));

  // clang-format off
  UpdatePropertyValueCommand set_10 {&mMapDocument, context_uuid, mPropertyName, 10.0f};
  const UpdatePropertyValueCommand set_20 {&mMapDocument, context_uuid, mPropertyName, 20.0f};
  const UpdatePropertyValueCommand set_30 {&mMapDocument, context_uuid, mPropertyName, 30.0f};
  const UpdatePropertyValueCommand set_40 {&mMapDocument, context_uuid, mPropertyName + "!", 40.0f};
  const UpdatePropertyValueCommand set_50 {&mMapDocument, UUID::generate(), mPropertyName, 50.0f};
  // clang-format on

  EXPECT_TRUE(set_10.merge_with(&set_20));
  EXPECT_TRUE(set_10.merge_with(&set_30));
  EXPECT_FALSE(set_10.merge_with(&set_40));
  EXPECT_FALSE(set_10.merge_with(&set_50));

  set_10.redo();
  EXPECT_EQ(metadata.get_property(mPropertyName), 30.0f);

  set_10.undo();
  EXPECT_EQ(metadata.get_property(mPropertyName), mOldPropertyValue);
}
