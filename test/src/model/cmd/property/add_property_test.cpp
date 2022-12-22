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

#include "model/cmd/property/add_property.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(AddProperty, Constructor)
{
  ASSERT_THROW(cmd::AddProperty(nullptr, "", AttributeType::String), TactileError);
}

TEST(AddProperty, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();
  auto& props = map->get_ctx().props();

  ASSERT_TRUE(props.empty());

  cmd::AddProperty cmd {map, "Foo", AttributeType::Int};
  cmd.redo();

  ASSERT_TRUE(props.contains("Foo"));
  ASSERT_EQ(0, props.at("Foo").as_int());

  cmd.undo();

  ASSERT_FALSE(props.contains("Foo"));
  ASSERT_TRUE(props.empty());
}

}  // namespace tactile::test
