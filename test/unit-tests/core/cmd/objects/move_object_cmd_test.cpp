// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/object/move_object_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(MoveObjectCmd, Constructor)
{
  ASSERT_THROW(MoveObjectCmd(nullptr, {}, {}), TactileError);
}

TEST(MoveObjectCmd, RedoUndo)
{
  Shared<Object> object;

  auto document = test::MapBuilder::build()  //
                      .with_object(ObjectType::Rect, &object)
                      .result();

  const Vector2f initialPos {843, 317};
  object->set_pos(initialPos);

  const Vector2f newPos {-835, 94};
  MoveObjectCmd  cmd {object, initialPos, newPos};

  cmd.redo();
  ASSERT_EQ(newPos, object->get_pos());

  cmd.undo();
  ASSERT_EQ(initialPos, object->get_pos());
}

}  // namespace tactile::test
