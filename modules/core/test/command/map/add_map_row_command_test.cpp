// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/map/add_map_row_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;
using namespace tactile::core;

/// \tests tactile::AddMapRowCommand::redo
/// \tests tactile::AddMapRowCommand::undo
TEST(AddMapRowCommand, RedoUndo)
{
  const auto initial_extent = MatrixExtent {5, 5};

  MapDocument map_document {make_unique<OrthogonalMap>(initial_extent, Int2 {32, 32})};
  auto& map = map_document.map();

  AddMapRowCommand add_row {&map_document};

  add_row.redo();
  EXPECT_EQ(map.extent().row_count, initial_extent.row_count + 1);
  EXPECT_EQ(map.extent().col_count, initial_extent.col_count);

  add_row.undo();
  EXPECT_EQ(map.extent().row_count, initial_extent.row_count);
  EXPECT_EQ(map.extent().col_count, initial_extent.col_count);
}

/// \tests tactile::AddMapRowCommand::merge_with
TEST(AddMapRowCommand, MergeWith)
{
  const auto initial_extent = MatrixExtent {5, 5};

  MapDocument map_document {make_unique<OrthogonalMap>(initial_extent, Int2 {32, 32})};
  auto& map = map_document.map();

  AddMapRowCommand add_row_1 {&map_document};
  const AddMapRowCommand add_row_2 {&map_document};
  const AddMapRowCommand add_row_3 {&map_document};

  EXPECT_TRUE(add_row_1.merge_with(&add_row_2));
  EXPECT_TRUE(add_row_1.merge_with(&add_row_3));

  add_row_1.redo();
  EXPECT_EQ(map.extent().row_count, initial_extent.row_count + 3);
  EXPECT_EQ(map.extent().col_count, initial_extent.col_count);

  add_row_1.undo();
  EXPECT_EQ(map.extent().row_count, initial_extent.row_count);
  EXPECT_EQ(map.extent().col_count, initial_extent.col_count);
}