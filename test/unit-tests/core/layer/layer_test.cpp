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

#include <gtest/gtest.h>

#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"

namespace tactile::test {

using LayerTypes = testing::Types<TileLayer, ObjectLayer, GroupLayer>;

template <typename T>
struct LayerTest : testing::Test
{};

TYPED_TEST_SUITE(LayerTest, LayerTypes);

TYPED_TEST(LayerTest, Defaults)
{
  const TypeParam layer;
  ASSERT_FALSE(layer.get_uuid().is_nil());
  ASSERT_TRUE(layer.is_visible());
  ASSERT_EQ(1.0f, layer.get_opacity());
}

TYPED_TEST(LayerTest, SetOpacity)
{
  TypeParam layer;

  layer.set_opacity(0.4f);
  ASSERT_EQ(0.4f, layer.get_opacity());

  layer.set_opacity(-0.2f);
  ASSERT_EQ(0.0f, layer.get_opacity());

  layer.set_opacity(1.1f);
  ASSERT_EQ(1.0f, layer.get_opacity());
}

TYPED_TEST(LayerTest, SetVisible)
{
  TypeParam layer;

  layer.set_visible(false);
  ASSERT_FALSE(layer.is_visible());

  layer.set_visible(true);
  ASSERT_TRUE(layer.is_visible());
}

TYPED_TEST(LayerTest, Clone)
{
  TypeParam source;
  source.set_opacity(0.7f);
  source.set_visible(false);

  const auto clone = source.clone();
  ASSERT_NE(source.get_uuid(), clone->get_uuid());

  ASSERT_EQ(source.get_opacity(), clone->get_opacity());
  ASSERT_EQ(source.is_visible(), clone->is_visible());
}

}  // namespace tactile::test
