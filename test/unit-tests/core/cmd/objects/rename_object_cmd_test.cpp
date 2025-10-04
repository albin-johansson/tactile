// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/object/rename_object_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RenameObjectCmd, Constructor)
{
  ASSERT_THROW(RenameObjectCmd(nullptr, ""), TactileError);
}

TEST(RenameObjectCmd, RedoUndo)
{
  Shared<Object> object;

  auto document = test::MapBuilder::build()  //
                      .with_object(ObjectType::Rect, &object)
                      .result();

  object->set_name("foo");

  RenameObjectCmd cmd {object, "bar"};

  cmd.redo();
  ASSERT_EQ("bar", object->get_name());

  cmd.undo();
  ASSERT_EQ("foo", object->get_name());
}

TEST(RenameObjectCmd, MergeSupport)
{
  Shared<Object> object;

  auto map = test::MapBuilder::build()  //
                 .with_object(ObjectType::Ellipse, &object)
                 .result();

  object->set_name("start");

  RenameObjectCmd       a {object, "a"};
  const RenameObjectCmd b {object, "b"};
  const RenameObjectCmd c {object, "c"};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ("c", object->get_name());

  a.undo();
  ASSERT_EQ("start", object->get_name());
}

}  // namespace tactile::test
