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

#include "core/layers/object.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(Object, Defaults)
{
  const Object object;

  ASSERT_EQ(Vector2f {}, object.get_pos());
  ASSERT_EQ(Vector2f {}, object.get_size());
  ASSERT_EQ(ObjectType::Rect, object.get_type());
  ASSERT_EQ("", object.get_tag());
  ASSERT_TRUE(object.is_visible());
}

TEST(Object, SetPos)
{
  Object object;

  object.set_pos({12, 83});
  ASSERT_EQ(12, object.get_pos().x);
  ASSERT_EQ(83, object.get_pos().y);

  object.set_pos({-23, -254});
  ASSERT_EQ(-23, object.get_pos().x);
  ASSERT_EQ(-254, object.get_pos().y);
}

TEST(Object, SetSize)
{
  Object object;

  object.set_size({391, 34});
  ASSERT_EQ(391, object.get_size().x);
  ASSERT_EQ(34, object.get_size().y);

  ASSERT_NO_THROW(object.set_size({-54, -78}));
}

TEST(Object, SetType)
{
  Object object;

  object.set_type(ObjectType::Ellipse);
  ASSERT_EQ(ObjectType::Ellipse, object.get_type());

  object.set_type(ObjectType::Point);
  ASSERT_EQ(ObjectType::Point, object.get_type());

  object.set_type(ObjectType::Rect);
  ASSERT_EQ(ObjectType::Rect, object.get_type());
}

TEST(Object, SetTag)
{
  Object object;

  object.set_tag("foo");
  ASSERT_EQ("foo", object.get_tag());
}

TEST(Object, SetVisible)
{
  Object object;

  object.set_visible(false);
  ASSERT_FALSE(object.is_visible());

  object.set_visible(true);
  ASSERT_TRUE(object.is_visible());
}

}  // namespace tactile::test
