// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/map/add_column_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(AddColumnCmd, Constructor)
{
  ASSERT_THROW(AddColumnCmd {nullptr}, TactileError);
}

TEST(AddColumnCmd, RedoUndo)
{
  const usize initialRows = 5;
  const usize initialCols = 7;

  auto document = test::MapBuilder::build().with_size(initialRows, initialCols).result();
  auto map = document->get_map_ptr();

  AddColumnCmd cmd {map};
  cmd.redo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols + 1, map->column_count());

  cmd.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());
}

TEST(AddColumnCmd, MergeSupport)
{
  const usize initialRows = 13;
  const usize initialCols = 5;

  auto document = test::MapBuilder::build().with_size(initialRows, initialCols).result();
  auto map = document->get_map_ptr();

  AddColumnCmd       a {map};
  const AddColumnCmd b {map};
  const AddColumnCmd c {map};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols + 3, map->column_count());

  a.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());
}

}  // namespace tactile::test
