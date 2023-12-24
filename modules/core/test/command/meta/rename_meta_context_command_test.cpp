// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/rename_meta_context_command.hpp"

#include <gtest/gtest.h>

#include "tactile/core/map/layer/object.hpp"

using namespace tactile;

TEST(RenameMetaContextCommand, RedoUndo)
{
  Object object {ObjectType::kPoint};
  object.meta().set_name("ABC");

  RenameMetaContextCommand command {&object, "DEF"};
  EXPECT_EQ(object.meta().get_name(), "ABC");

  command.redo();
  EXPECT_EQ(object.meta().get_name(), "DEF");

  command.undo();
  EXPECT_EQ(object.meta().get_name(), "ABC");
}

TEST(RenameMetaContextCommand, MergeWith)
{
  Object rect {ObjectType::kRect};
  Object ellipse {ObjectType::kEllipse};
  ellipse.meta().set_name("start");


  RenameMetaContextCommand ellipse_foo_command {&ellipse, "foo"};
  const RenameMetaContextCommand rect_bar_command {&rect, "bar"};
  const RenameMetaContextCommand ellipse_bar_command {&ellipse, "bar"};

  EXPECT_FALSE(ellipse_foo_command.merge_with(&rect_bar_command));
  EXPECT_TRUE(ellipse_foo_command.merge_with(&ellipse_bar_command));

  ellipse_foo_command.redo();
  EXPECT_EQ(ellipse.meta().get_name(), "bar");

  ellipse_foo_command.undo();
  EXPECT_EQ(ellipse.meta().get_name(), "start");
}
