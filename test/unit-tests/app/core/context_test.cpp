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

#include "core/context.hpp"

#include <gtest/gtest.h>

#include "core/layers/group_layer.hpp"
#include "core/layers/object.hpp"
#include "core/layers/object_layer.hpp"
#include "core/layers/tile_layer.hpp"
#include "core/tilesets/tile.hpp"
#include "core/tilesets/tileset_info.hpp"

using namespace tactile;
using namespace core;

using Contexts = testing::Types<TileLayer,  //
                                ObjectLayer,
                                GroupLayer,
                                Object,
                                Tile>;

template <typename T>
struct ContextTest : testing::Test
{};

namespace {

template <typename T>
auto _make_context() -> T
{
  return T{};
}

template <>
auto _make_context<Tile>() -> Tile
{
  return Tile{42};
}

}  // namespace

TYPED_TEST_SUITE(ContextTest, Contexts);

TYPED_TEST(ContextTest, Defaults)
{
  const auto context = _make_context<TypeParam>();
  ASSERT_FALSE(context.get_uuid().is_nil());

  // TODO ASSERT_TRUE(context.get_comps().empty());
  ASSERT_TRUE(context.get_props().empty());
}
