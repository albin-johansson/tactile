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

#include "core/layer/object_layer.hpp"

#include <doctest/doctest.h>

namespace tactile::test {

// TODO improve this suite

TEST_SUITE("ObjectLayer")
{
  TEST_CASE("Defaults")
  {
    const ObjectLayer layer;
    REQUIRE(LayerType::ObjectLayer == layer.get_type());
    REQUIRE(0u == layer.object_count());
    REQUIRE(!layer.get_meta_id().has_value());
  }

  TEST_CASE("add_object")
  {
    ObjectLayer layer;
    auto object = std::make_shared<Object>();

    REQUIRE(0u == layer.object_count());
    REQUIRE(!layer.has_object(object->get_uuid()));

    layer.add_object(object);

    REQUIRE(1u == layer.object_count());
    REQUIRE(layer.has_object(object->get_uuid()));
  }

  TEST_CASE("remove_object")
  {
    ObjectLayer layer;
    auto object = std::make_shared<Object>();

    REQUIRE_NOTHROW(layer.remove_object(object->get_uuid()));
  }
}

}  // namespace tactile::test
