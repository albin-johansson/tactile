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

#include <doctest/doctest.h>

namespace tactile::test {

TEST_SUITE("Object")
{
  TEST_CASE("Defaults")
  {
    const Object object;

    REQUIRE(ObjectType::Rect == object.get_type());
    REQUIRE(object.is_rect());

    REQUIRE(Float2 {} == object.get_pos());
    REQUIRE(Float2 {} == object.get_size());

    REQUIRE("" == object.get_ctx().name());
    REQUIRE("" == object.get_tag());

    REQUIRE(!object.get_meta_id());
    REQUIRE(object.is_visible());
  }

  TEST_CASE("Seset_postPos")
  {
    Object object;

    object.set_pos({12, 83});
    REQUIRE(12 == object.get_pos().x());
    REQUIRE(83 == object.get_pos().y());

    object.set_pos({-23, -254});
    REQUIRE(-23 == object.get_pos().x());
    REQUIRE(-254 == object.get_pos().y());
  }

  TEST_CASE("set_size")
  {
    Object object;

    object.set_size({391, 34});
    REQUIRE(391 == object.get_size().x());
    REQUIRE(34 == object.get_size().y());

    REQUIRE_NOTHROW(object.set_size({-54, -78}));
  }

  TEST_CASE("set_type")
  {
    Object object;

    object.set_type(ObjectType::Ellipse);
    REQUIRE(ObjectType::Ellipse == object.get_type());

    REQUIRE(object.is_ellipse());
    REQUIRE(!object.is_rect());
    REQUIRE(!object.is_point());

    object.set_type(ObjectType::Point);
    REQUIRE(ObjectType::Point == object.get_type());

    REQUIRE(object.is_point());
    REQUIRE(!object.is_ellipse());
    REQUIRE(!object.is_rect());

    object.set_type(ObjectType::Rect);
    REQUIRE(ObjectType::Rect == object.get_type());

    REQUIRE(object.is_rect());
    REQUIRE(!object.is_point());
    REQUIRE(!object.is_ellipse());
  }

  TEST_CASE("set_type (point special case)")
  {
    Object object;
    object.set_size({123, 456});

    object.set_type(ObjectType::Point);
    REQUIRE(0 == object.get_size().x());
    REQUIRE(0 == object.get_size().y());
  }

  TEST_CASE("set_meta_id")
  {
    Object object;

    object.set_meta_id(42);
    REQUIRE(42 == object.get_meta_id());
  }

  TEST_CASE("set_visible")
  {
    Object object;

    object.set_visible(false);
    REQUIRE(!object.is_visible());

    object.set_visible(true);
    REQUIRE(object.is_visible());
  }

  TEST_CASE("set_tag")
  {
    Object object;

    object.set_tag("foo");
    REQUIRE("foo" == object.get_tag());
  }

  TEST_CASE("clone")
  {
    Object object;
    object.set_type(ObjectType::Ellipse);
    object.set_pos({921, 832});
    object.set_size({43, 83});
    object.set_meta_id(7);
    object.set_tag("foo");
    object.set_visible(false);

    const auto clone = object.clone();
    REQUIRE(object.get_uuid() != clone->get_uuid());

    REQUIRE(object.get_type() == clone->get_type());
    REQUIRE(object.get_pos() == clone->get_pos());
    REQUIRE(object.get_size() == clone->get_size());
    REQUIRE(object.get_meta_id() == clone->get_meta_id());
    REQUIRE(object.get_tag() == clone->get_tag());
    REQUIRE(object.is_visible() == clone->is_visible());
  }
}

}  // namespace tactile::test
