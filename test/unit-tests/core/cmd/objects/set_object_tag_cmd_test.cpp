// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/object/set_object_tag_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(SetObjectTagCmd, Constructor)
{
  ASSERT_THROW(SetObjectTagCmd(nullptr, ""), TactileError);
}

TEST(SetObjectTagCmd, RedoUndo)
{
  Shared<Object> object;

  auto map = test::MapBuilder::build()  //
                 .with_object(ObjectType::Rect, &object)
                 .result();

  object->set_tag("old-tag");

  SetObjectTagCmd cmd {object, "new-tag"};

  cmd.redo();
  ASSERT_EQ("new-tag", object->get_tag());

  cmd.undo();
  ASSERT_EQ("old-tag", object->get_tag());
}

TEST(SetObjectTagCmd, MergeSupport)
{
  Shared<Object> object;

  auto document = test::MapBuilder::build()  //
                      .with_object(ObjectType::Rect, &object)
                      .result();

  object->set_tag("x");

  SetObjectTagCmd       a {object, "a"};
  const SetObjectTagCmd b {object, "b"};

  ASSERT_TRUE(a.merge_with(&b));

  a.redo();
  ASSERT_EQ("b", object->get_tag());

  a.undo();
  ASSERT_EQ("x", object->get_tag());
}

}  // namespace tactile::test
