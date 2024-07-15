// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/meta/create_property_command.hpp"

#include <gtest/gtest.h>

#include "tactile/base/container/maybe.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/meta/meta.hpp"
#include "test/document_testing.hpp"

namespace tactile::test {

// tactile::CreatePropertyCommand::redo
// tactile::CreatePropertyCommand::undo
TEST(CreatePropertyCommand, RedoUndo)
{
  MapDocument document {kOrthogonalMapSpec};

  const auto& registry = document.get_registry();
  const auto map_entity = registry.get<CDocumentInfo>().root;

  const auto& meta = registry.get<CMeta>(map_entity);
  ASSERT_EQ(meta.properties.size(), 0);
  ASSERT_EQ(meta.components.size(), 0);

  const String property_name {"foo"};
  const Attribute property_value {42};

  CreatePropertyCommand command {&document,
                                 map_entity,
                                 property_name,
                                 property_value};

  EXPECT_FALSE(meta.properties.contains(property_name));
  EXPECT_EQ(meta.properties.size(), 0);
  EXPECT_EQ(meta.components.size(), 0);

  command.redo();

  EXPECT_TRUE(meta.properties.contains(property_name));
  EXPECT_EQ(meta.properties.at(property_name), property_value);
  EXPECT_EQ(meta.properties.size(), 1);
  EXPECT_EQ(meta.components.size(), 0);

  command.undo();

  EXPECT_FALSE(meta.properties.contains(property_name));
  EXPECT_EQ(meta.properties.size(), 0);
  EXPECT_EQ(meta.components.size(), 0);
}

}  // namespace tactile::test
