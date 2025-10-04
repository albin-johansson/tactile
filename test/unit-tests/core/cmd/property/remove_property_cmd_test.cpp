// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/property/remove_property_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RemovePropertyCmd, Constructor)
{
  ASSERT_THROW(RemovePropertyCmd(nullptr, ""), TactileError);
}

TEST(RemovePropertyCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& props = map->get_props();
  props.add("id", 42);

  RemovePropertyCmd cmd {map, "id"};
  cmd.redo();

  ASSERT_FALSE(props.contains("id"));

  cmd.undo();

  ASSERT_TRUE(props.contains("id"));
  ASSERT_EQ(42, props.at("id").as_int());
}

}  // namespace tactile::test
