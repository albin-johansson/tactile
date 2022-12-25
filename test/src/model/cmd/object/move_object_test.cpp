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

#include "model/cmd/object/move_object.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "core/debug/panic.hpp"

namespace tactile::test {

TEST(MoveObject, Constructor)
{
  ASSERT_THROW(cmd::MoveObject(nullptr, {}, {}), TactileError);
}

TEST(MoveObject, RedoUndo)
{
  Shared<Object> object;

  auto document = test::MapBuilder::build()  //
                      .with_object(ObjectType::Rect, &object)
                      .result();

  const Float2 initial_pos {843, 317};
  object->set_pos(initial_pos);

  const Float2 new_pos {-835, 94};
  cmd::MoveObject cmd {object, initial_pos, new_pos};

  cmd.redo();
  ASSERT_EQ(new_pos, object->get_pos());

  cmd.undo();
  ASSERT_EQ(initial_pos, object->get_pos());
}

}  // namespace tactile::test
