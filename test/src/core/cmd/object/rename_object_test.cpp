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

#include "core/cmd/object/rename_object.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(RenameObject, Constructor)
{
  ASSERT_THROW(cmd::RenameObject(nullptr, ""), TactileError);
}

TEST(RenameObject, RedoUndo)
{
  Shared<Object> object;

  auto document = test::MapBuilder::build()  //
                      .with_object(ObjectType::Rect, &object)
                      .result();

  object->ctx().set_name("foo");

  cmd::RenameObject cmd {object, "bar"};

  cmd.redo();
  ASSERT_EQ("bar", object->ctx().name());

  cmd.undo();
  ASSERT_EQ("foo", object->ctx().name());
}

TEST(RenameObject, MergeSupport)
{
  Shared<Object> object;

  auto map = test::MapBuilder::build()  //
                 .with_object(ObjectType::Ellipse, &object)
                 .result();

  object->ctx().set_name("start");

  cmd::RenameObject a {object, "a"};
  const cmd::RenameObject b {object, "b"};
  const cmd::RenameObject c {object, "c"};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ("c", object->ctx().name());

  a.undo();
  ASSERT_EQ("start", object->ctx().name());
}

}  // namespace tactile::test
