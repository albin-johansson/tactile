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

#include "core/cmd/object/set_object_name_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(SetObjectNameCmd, Constructor)
{
  ASSERT_THROW(SetObjectNameCmd(nullptr, ""), TactileError);
}

TEST(SetObjectNameCmd, RedoUndo)
{
  Shared<Object> object;

  auto document = test::MapBuilder::build()  //
                      .with_object(ObjectType::Rect, &object)
                      .result();

  object->set_name("foo");

  SetObjectNameCmd cmd {object, "bar"};

  cmd.redo();
  ASSERT_EQ("bar", object->get_name());

  cmd.undo();
  ASSERT_EQ("foo", object->get_name());
}

TEST(SetObjectNameCmd, MergeSupport)
{
  Shared<Object> object;

  auto map = test::MapBuilder::build()  //
                 .with_object(ObjectType::Ellipse, &object)
                 .result();

  object->set_name("start");

  SetObjectNameCmd       a {object, "a"};
  const SetObjectNameCmd b {object, "b"};
  const SetObjectNameCmd c {object, "c"};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ("c", object->get_name());

  a.undo();
  ASSERT_EQ("start", object->get_name());
}

}  // namespace tactile::test
