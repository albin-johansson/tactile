// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/property/change_property_type_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(ChangePropertyTypeCmd, Constructor)
{
  ASSERT_THROW(ChangePropertyTypeCmd(nullptr, "", AttributeType::Int), TactileError);
}

TEST(ChangePropertyTypeCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& props = map->get_props();
  props.add("property", 123);

  ChangePropertyTypeCmd cmd {map, "property", AttributeType::Bool};
  cmd.redo();

  ASSERT_TRUE(props.contains("property"));
  ASSERT_FALSE(props.at("property").as_bool());

  cmd.undo();

  ASSERT_TRUE(props.contains("property"));
  ASSERT_EQ(123, props.at("property").as_int());
}

}  // namespace tactile::test
