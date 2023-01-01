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

#include <doctest/doctest.h>

#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"

namespace tactile::test {

TEST_SUITE("Layer")
{
  TEST_CASE_TEMPLATE("Defaults", T, TileLayer, ObjectLayer, GroupLayer)
  {
    const T layer;
    REQUIRE(layer.is_visible());
    REQUIRE(!layer.get_uuid().is_nil());
    REQUIRE(!layer.get_parent().has_value());
    REQUIRE(!layer.get_meta_id().has_value());
    REQUIRE(1.0f == layer.get_opacity());
  }

  TEST_CASE_TEMPLATE("set_opacity", T, TileLayer, ObjectLayer, GroupLayer)
  {
    T layer;

    layer.set_opacity(0.4f);
    REQUIRE(0.4f == layer.get_opacity());

    layer.set_opacity(-0.2f);
    REQUIRE(0.0f == layer.get_opacity());

    layer.set_opacity(1.1f);
    REQUIRE(1.0f == layer.get_opacity());
  }

  TEST_CASE_TEMPLATE("set_visible", T, TileLayer, ObjectLayer, GroupLayer)
  {
    T layer;

    layer.set_visible(false);
    REQUIRE(!layer.is_visible());

    layer.set_visible(true);
    REQUIRE(layer.is_visible());
  }

  TEST_CASE_TEMPLATE("clone", T, TileLayer, ObjectLayer, GroupLayer)
  {
    T source;
    source.set_opacity(0.7f);
    source.set_parent(make_uuid());
    source.set_visible(false);

    const auto clone = source.clone();
    REQUIRE(source.get_uuid() != clone->get_uuid());

    REQUIRE(source.get_opacity() == clone->get_opacity());
    REQUIRE(source.get_parent() == clone->get_parent());
    REQUIRE(source.is_visible() == clone->is_visible());
  }
}

}  // namespace tactile::test
