// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/meta/create_property_command.hpp"

#include <gtest/gtest.h>

#include "tactile/base/container/maybe.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile::test {

class CreatePropertyCommandTest : public testing::Test
{
 public:
  void SetUp() override
  {
    mDocument.emplace(MapSpec {
      .orientation = MapOrientation::kOrthogonal,
      .extent = MatrixExtent {10, 10},
      .tile_size = Int2 {50, 50},
    });
  }

  [[nodiscard]]
  auto get_map_document() -> MapDocument&
  {
    return mDocument.value();
  }

 private:
  Optional<MapDocument> mDocument {};
};

// tactile::CreatePropertyCommand::redo
// tactile::CreatePropertyCommand::undo
TEST_F(CreatePropertyCommandTest, RedoUndo)
{
  auto& document = get_map_document();

  const auto& registry = document.get_registry();
  const auto map_entity = document.get_root_entity();

  const auto& meta = registry.get<CMeta>(map_entity);
  ASSERT_EQ(meta.properties.size(), 0);
  ASSERT_EQ(meta.components.size(), 0);

  const String property_name {"foo"};
  const auto property_type = AttributeType::kInt;

  CreatePropertyCommand command {&document,
                                 map_entity,
                                 property_name,
                                 property_type};

  EXPECT_FALSE(meta.properties.contains(property_name));
  EXPECT_EQ(meta.properties.size(), 0);
  EXPECT_EQ(meta.components.size(), 0);

  command.redo();

  EXPECT_TRUE(meta.properties.contains(property_name));
  EXPECT_TRUE(meta.properties.at(property_name).has_default_value());
  EXPECT_EQ(meta.properties.at(property_name).get_type(), property_type);
  EXPECT_EQ(meta.properties.size(), 1);
  EXPECT_EQ(meta.components.size(), 0);

  command.undo();

  EXPECT_FALSE(meta.properties.contains(property_name));
  EXPECT_EQ(meta.properties.size(), 0);
  EXPECT_EQ(meta.components.size(), 0);
}

}  // namespace tactile::test
