// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/update_property_type_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/map/orthogonal_map.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/meta/attribute.hpp"

using namespace tactile;
using namespace tactile::core;

class UpdatePropertyTypeCommandTest : public testing::Test {
 protected:
  UpdatePropertyTypeCommandTest()
    : mMapDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})}
  {
    auto& map = mMapDocument.map();
    map.meta().add_property(mPropertyName) = mOldPropertyValue;
  }

  MapDocument mMapDocument;
  const String mPropertyName {"ABC"};
  const Attribute mOldPropertyValue {123};
  const AttributeType mNewPropertyType {AttributeType::kColor};
};

/// \tests tactile::UpdatePropertyTypeCommand::redo
/// \tests tactile::UpdatePropertyTypeCommand::undo
TEST_F(UpdatePropertyTypeCommandTest, RedoUndo)
{
  auto& metadata = mMapDocument.map().meta();
  const auto context_uuid = metadata.uuid();

  ASSERT_TRUE(metadata.has_property(mPropertyName));
  ASSERT_EQ(metadata.get_property(mPropertyName), mOldPropertyValue);

  UpdatePropertyTypeCommand command {&mMapDocument,
                                     context_uuid,
                                     mPropertyName,
                                     mNewPropertyType};

  command.redo();
  EXPECT_EQ(metadata.get_property(mPropertyName).get_type(), mNewPropertyType);
  EXPECT_TRUE(metadata.get_property(mPropertyName).has_default_value());

  command.undo();
  EXPECT_EQ(metadata.get_property(mPropertyName).get_type(),
            mOldPropertyValue.get_type());
  EXPECT_EQ(metadata.get_property(mPropertyName), mOldPropertyValue);
}
