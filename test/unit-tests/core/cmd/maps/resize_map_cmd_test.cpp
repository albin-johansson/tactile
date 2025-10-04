// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/map/resize_map_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(ResizeMapCmd, Constructor)
{
  ASSERT_THROW(ResizeMapCmd(nullptr, 1, 1), TactileError);
}

TEST(ResizeMapCmd, RedoUndo)
{
  auto document = test::MapBuilder::build().with_size(5, 7).result();
  auto map = document->get_map_ptr();

  ResizeMapCmd cmd {map, 3, 9};
  cmd.redo();

  ASSERT_EQ(3, map->row_count());
  ASSERT_EQ(9, map->column_count());

  cmd.undo();

  ASSERT_EQ(5, map->row_count());
  ASSERT_EQ(7, map->column_count());
}

}  // namespace tactile::test
