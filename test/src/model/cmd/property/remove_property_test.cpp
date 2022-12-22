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

#include "model/cmd/property/remove_property.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(RemoveProperty, Constructor)
{
  ASSERT_THROW(cmd::RemoveProperty(nullptr, ""), TactileError);
}

TEST(RemoveProperty, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& props = map->get_ctx().props();
  props.add("id", 42);

  cmd::RemoveProperty cmd {map, "id"};
  cmd.redo();

  ASSERT_FALSE(props.contains("id"));

  cmd.undo();

  ASSERT_TRUE(props.contains("id"));
  ASSERT_EQ(42, props.at("id").as_int());
}

}  // namespace tactile::test
