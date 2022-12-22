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

  ASSERT_EQ(ObjectType::Rect, object.type());
  ASSERT_TRUE(object.is_rect());

  ASSERT_EQ(Float2 {}, object.pos());
  ASSERT_EQ(Float2 {}, object.size());

  ASSERT_EQ("", object.get_ctx().name());
  ASSERT_EQ("", object.tag());

  ASSERT_FALSE(object.meta_id());
  ASSERT_TRUE(object.visible());
}

TEST(Object, SetPos)
{
  Object object;

  object.set_pos({12, 83});
  ASSERT_EQ(12, object.pos().x);
  ASSERT_EQ(83, object.pos().y);

  object.set_pos({-23, -254});
  ASSERT_EQ(-23, object.pos().x);
  ASSERT_EQ(-254, object.pos().y);
}

TEST(Object, SetSize)
{
  Object object;

  object.set_size({391, 34});
  ASSERT_EQ(391, object.size().x);
  ASSERT_EQ(34, object.size().y);

  ASSERT_NO_THROW(object.set_size({-54, -78}));
}

TEST(Object, SetType)
{
  Object object;

  object.set_type(ObjectType::Ellipse);
  ASSERT_EQ(ObjectType::Ellipse, object.type());

  ASSERT_TRUE(object.is_ellipse());
  ASSERT_FALSE(object.is_rect());
  ASSERT_FALSE(object.is_point());

  object.set_type(ObjectType::Point);
  ASSERT_EQ(ObjectType::Point, object.type());

  ASSERT_TRUE(object.is_point());
  ASSERT_FALSE(object.is_ellipse());
  ASSERT_FALSE(object.is_rect());

  object.set_type(ObjectType::Rect);
  ASSERT_EQ(ObjectType::Rect, object.type());

  ASSERT_TRUE(object.is_rect());
  ASSERT_FALSE(object.is_point());
  ASSERT_FALSE(object.is_ellipse());
}

TEST(Object, SetTypeToPoint)
{
  Object object;
  object.set_size({123, 456});

  object.set_type(ObjectType::Point);
  ASSERT_EQ(0, object.size().x);
  ASSERT_EQ(0, object.size().y);
}

TEST(Object, SetMetaId)
{
  Object object;

  object.set_meta_id(42);
  ASSERT_EQ(42, object.meta_id());
}

TEST(Object, SetVisible)
{
  Object object;

  object.set_visible(false);
  ASSERT_FALSE(object.visible());

  object.set_visible(true);
  ASSERT_TRUE(object.visible());
}

TEST(Object, SetTag)
{
  Object object;

  object.set_tag("foo");
  ASSERT_EQ("foo", object.tag());
}

TEST(Object, Clone)
{
  Object object;
  object.set_type(ObjectType::Ellipse);
  object.set_pos({921, 832});
  object.set_size({43, 83});
  object.set_meta_id(7);
  object.set_tag("foo");
  object.set_visible(false);

  const auto clone = object.clone();
  ASSERT_NE(object.get_uuid(), clone->get_uuid());

  ASSERT_EQ(object.type(), clone->type());
  ASSERT_EQ(object.pos(), clone->pos());
  ASSERT_EQ(object.size(), clone->size());
  ASSERT_EQ(object.meta_id(), clone->meta_id());
  ASSERT_EQ(object.tag(), clone->tag());
  ASSERT_EQ(object.visible(), clone->visible());
}

}  // namespace tactile::test
