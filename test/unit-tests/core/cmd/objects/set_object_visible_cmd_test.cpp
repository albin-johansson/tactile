// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/object/set_object_visible_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(SetObjectVisibleCmd, Constructor)
{
  ASSERT_THROW(SetObjectVisibleCmd(nullptr, false), TactileError);
}

TEST(SetObjectVisibleCmd, RedoUndo)
{
  Shared<Object> object;

  auto document = test::MapBuilder::build()  //
                      .with_object(ObjectType::Rect, &object)
                      .result();

  SetObjectVisibleCmd cmd {object, false};

  cmd.redo();
  ASSERT_FALSE(object->is_visible());

  cmd.undo();
  ASSERT_TRUE(object->is_visible());
}

}  // namespace tactile::test
