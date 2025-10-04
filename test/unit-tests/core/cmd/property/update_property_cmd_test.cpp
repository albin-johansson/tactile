// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/property/update_property_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(UpdatePropertyCmd, Constructor)
{
  ASSERT_THROW(UpdatePropertyCmd(nullptr, "", 0), TactileError);
}

TEST(UpdatePropertyCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& props = map->get_props();
  props.add("int", 10);

  UpdatePropertyCmd cmd {map, "int", 20};

  cmd.redo();
  ASSERT_EQ(20, props.at("int"));

  cmd.undo();
  ASSERT_EQ(10, props.at("int"));
}

TEST(UpdatePropertyCmd, MergeSupport)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& props = map->get_props();
  props.add("color", cen::colors::cyan);

  UpdatePropertyCmd       a {map, "color", cen::colors::azure};
  const UpdatePropertyCmd b {map, "color", cen::colors::gold};
  const UpdatePropertyCmd c {map, "color", cen::colors::violet};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ(cen::colors::violet, props.at("color").as_color());

  a.undo();
  ASSERT_EQ(cen::colors::cyan, props.at("color").as_color());
}

}  // namespace tactile::test
