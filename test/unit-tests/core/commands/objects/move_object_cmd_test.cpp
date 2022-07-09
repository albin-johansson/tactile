/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/commands/objects/move_object_cmd.hpp"

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
