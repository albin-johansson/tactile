// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/property/rename_property_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RenamePropertyCmd, Constructor)
{
  ASSERT_THROW(RenamePropertyCmd(nullptr, "", ""), TactileError);
}

TEST(RenamePropertyCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& props = map->get_props();
  props.add("foo", cen::colors::red);

  RenamePropertyCmd cmd {map, "foo", "bar"};
  cmd.redo();

  ASSERT_FALSE(props.contains("foo"));
  ASSERT_TRUE(props.contains("bar"));
  ASSERT_EQ(cen::colors::red, props.at("bar"));

  cmd.undo();

  ASSERT_TRUE(props.contains("foo"));
  ASSERT_FALSE(props.contains("bar"));
  ASSERT_EQ(cen::colors::red, props.at("foo"));
}

}  // namespace tactile::test
