// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/map/remove_row_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"
#include "unit-tests/core/helpers/map_test_helpers.hpp"

namespace tactile::test {

TEST(RemoveRowCmd, Constructor)
{
  ASSERT_THROW(RemoveRowCmd {nullptr}, TactileError);
}

TEST(RemoveRowCmd, RedoUndo)
{
  const usize initialRows = 3;
  const usize initialCols = 5;

  auto document = test::MapBuilder::build().with_size(initialRows, initialCols).result();
  auto map = document->get_map_ptr();

  RemoveRowCmd cmd {map};
  cmd.redo();

  ASSERT_EQ(initialRows - 1, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  cmd.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());
}

TEST(RemoveRowCmd, MergeSupport)
{
  const usize initialRows = 3;
  const usize initialCols = 6;

  UUID layerId;

  auto document = test::MapBuilder::build()  //
                      .with_size(initialRows, initialCols)
                      .with_tile_layer(&layerId, 42)
                      .result();
  auto map = document->get_map_ptr();

  RemoveRowCmd       a {map};
  const RemoveRowCmd b {map};

  ASSERT_TRUE(a.merge_with(&b));

  a.redo();

  ASSERT_EQ(initialRows - 2, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  a.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  test::verify_all_tiles_matches(map->view_tile_layer(layerId), 42);
}

}  // namespace tactile::test
