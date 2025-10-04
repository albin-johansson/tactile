// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/property/add_property_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(AddPropertyCmd, Constructor)
{
  ASSERT_THROW(AddPropertyCmd(nullptr, "", AttributeType::String), TactileError);
}

TEST(AddPropertyCmd, RedoUndo)
{
  auto  document = MapBuilder::build().result();
  auto  map = document->get_map_ptr();
  auto& props = map->get_props();

  ASSERT_TRUE(props.empty());

  AddPropertyCmd cmd {map, "Foo", AttributeType::Int};
  cmd.redo();

  ASSERT_TRUE(props.contains("Foo"));
  ASSERT_EQ(0, props.at("Foo").as_int());

  cmd.undo();

  ASSERT_FALSE(props.contains("Foo"));
  ASSERT_TRUE(props.empty());
}

}  // namespace tactile::test
