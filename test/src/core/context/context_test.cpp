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

#include "core/context/context.hpp"

#include <doctest/doctest.h>

#include "core/layer/group_layer.hpp"
#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/map.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tileset.hpp"
#include "core/tile/tileset_info.hpp"
#include "io/load_texture.hpp"

namespace tactile::test {

#define CONTEXT_TYPES Map, TileLayer, ObjectLayer, GroupLayer, Object, Tileset, Tile

namespace {

template <typename T>
auto make_context() -> T
{
  return T {};
}

template <>
auto make_context<Tileset>() -> Tileset
{
  return Tileset {TilesetInfo {
      .texture = io::load_texture("resources/terrain.png"),
      .tile_size = {16, 32},
  }};
}

template <>
auto make_context<Tile>() -> Tile
{
  return Tile {42};
}

}  // namespace

TEST_SUITE("Context")
{
  TEST_CASE_TEMPLATE("Defaults", T, CONTEXT_TYPES)
  {
    const auto context = make_context<T>();
    REQUIRE(!context.get_uuid().is_nil());

    REQUIRE(context.get_ctx().comps().empty());
    REQUIRE(context.get_ctx().props().empty());
  }

  TEST_CASE_TEMPLATE("set_name", T, CONTEXT_TYPES)
  {
    auto context = make_context<T>();

    context.get_ctx().set_name("foobar");
    REQUIRE("foobar" == context.get_ctx().name());
  }
}

}  // namespace tactile::test
