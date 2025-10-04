// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/map/add_row_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(AddRowCmd, Constructor)
{
  ASSERT_THROW(AddRowCmd {nullptr}, TactileError);
}

TEST(AddRowCmd, RedoUndo)
{
  const usize initialRows = 10;
  const usize initialCols = 7;

  auto document = test::MapBuilder::build().with_size(initialRows, initialCols).result();
  auto map = document->get_map_ptr();

  AddRowCmd cmd {map};
  cmd.redo();

  ASSERT_EQ(initialRows + 1, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  cmd.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());
}

TEST(AddRowCmd, MergeSupport)
{
  const usize initialRows = 6;
  const usize initialCols = 7;

  auto document = test::MapBuilder::build().with_size(initialRows, initialCols).result();
  auto map = document->get_map_ptr();

  AddRowCmd       a {map};
  const AddRowCmd b {map};

  ASSERT_TRUE(a.merge_with(&b));

  a.redo();

  ASSERT_EQ(initialRows + 2, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  a.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());
}

}  // namespace tactile::test
