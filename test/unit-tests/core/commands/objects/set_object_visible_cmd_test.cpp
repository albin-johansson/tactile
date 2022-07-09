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

#include "core/commands/objects/set_object_visible_cmd.hpp"

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
