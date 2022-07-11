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

#include "core/commands/objects/set_object_tag_cmd.hpp"

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
