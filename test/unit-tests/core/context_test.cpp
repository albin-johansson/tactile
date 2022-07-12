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

#include <gtest/gtest.h>

#include "core/layer/group_layer.hpp"
#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/map.hpp"
#include "core/tileset/tile.hpp"
#include "core/tileset/tileset.hpp"
#include "core/tileset/tileset_info.hpp"

namespace tactile::test {

using Contexts = testing::Types<Map,  //
                                TileLayer,
                                ObjectLayer,
                                GroupLayer,
                                Object,
                                Tileset,
                                Tile>;

template <typename T>
struct ContextTest : testing::Test
{};

namespace {

template <typename T>
auto _make_context() -> T
{
  return T {};
}

template <>
auto _make_context<Tileset>() -> Tileset
{
  return Tileset {{.texture_path = "foo.png",
                   .texture_id = 8,
                   .texture_size = {1024, 768},
                   .tile_size = {16, 32}}};
}

template <>
auto _make_context<Tile>() -> Tile
{
  return Tile {42};
}

}  // namespace

TYPED_TEST_SUITE(ContextTest, Contexts);

TYPED_TEST(ContextTest, Defaults)
{
  const auto context = _make_context<TypeParam>();
  ASSERT_FALSE(context.get_uuid().is_nil());

  ASSERT_TRUE(context.get_comps().empty());
  ASSERT_TRUE(context.get_props().empty());
}

TYPED_TEST(ContextTest, SetName)
{
  auto context = _make_context<TypeParam>();

  context.set_name("foobar");
  ASSERT_EQ("foobar", context.get_name());
}

}  // namespace tactile::test
