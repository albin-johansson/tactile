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

#include "core/layer/object.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(Object, Defaults)
{
  const Object object;

  ASSERT_EQ(ObjectType::Rect, object.get_type());
  ASSERT_TRUE(object.is_rect());

  ASSERT_EQ(Float2 {}, object.get_pos());
  ASSERT_EQ(Float2 {}, object.get_size());

  ASSERT_EQ("", object.ctx().name());
  ASSERT_EQ("", object.get_tag());

  ASSERT_FALSE(object.get_meta_id());
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

  ASSERT_TRUE(object.is_ellipse());
  ASSERT_FALSE(object.is_rect());
  ASSERT_FALSE(object.is_point());

  object.set_type(ObjectType::Point);
  ASSERT_EQ(ObjectType::Point, object.get_type());

  ASSERT_TRUE(object.is_point());
  ASSERT_FALSE(object.is_ellipse());
  ASSERT_FALSE(object.is_rect());

  object.set_type(ObjectType::Rect);
  ASSERT_EQ(ObjectType::Rect, object.get_type());

  ASSERT_TRUE(object.is_rect());
  ASSERT_FALSE(object.is_point());
  ASSERT_FALSE(object.is_ellipse());
}

TEST(Object, SetTypeToPoint)
{
  Object object;
  object.set_size({123, 456});

  object.set_type(ObjectType::Point);
  ASSERT_EQ(0, object.get_size().x);
  ASSERT_EQ(0, object.get_size().y);
}

TEST(Object, SetTag)
{
  Object object;

  object.set_tag("foo");
  ASSERT_EQ("foo", object.get_tag());
}

TEST(Object, SetMetaId)
{
  Object object;

  object.set_meta_id(42);
  ASSERT_EQ(42, object.get_meta_id());
}

TEST(Object, SetVisible)
{
  Object object;

  object.set_visible(false);
  ASSERT_FALSE(object.is_visible());

  object.set_visible(true);
  ASSERT_TRUE(object.is_visible());
}

TEST(Object, Clone)
{
  Object object;
  object.set_type(ObjectType::Ellipse);
  object.set_pos({921, 832});
  object.set_size({43, 83});
  object.set_meta_id(7);
  object.ctx().set_name("abc");
  object.set_tag("foo");
  object.set_visible(false);

  const auto clone = object.clone();

  ASSERT_EQ(object.get_type(), clone->get_type());
  ASSERT_EQ(object.get_pos(), clone->get_pos());
  ASSERT_EQ(object.get_size(), clone->get_size());
  ASSERT_EQ(object.get_meta_id(), clone->get_meta_id());
  ASSERT_EQ(object.ctx().name(), clone->ctx().name());
  ASSERT_EQ(object.get_tag(), clone->get_tag());
  ASSERT_EQ(object.is_visible(), clone->is_visible());
}

}  // namespace tactile::test
