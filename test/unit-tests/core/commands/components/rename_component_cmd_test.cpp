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

#include "core/commands/components/rename_component_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RenameComponentCmd, Constructor)
{
  ASSERT_THROW(RenameComponentCmd(nullptr, make_uuid(), ""), TactileError);
}

TEST(RenameComponentCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Foo"}.result();

  auto& map = document->get_map();
  auto& bundle = map.get_comps();
  bundle.add(index->at(compId).instantiate());

  RenameComponentCmd cmd {index, compId, "Bar"};
  cmd.redo();

  ASSERT_TRUE(index->contains(compId));
  ASSERT_FALSE(index->contains("Foo"));
  ASSERT_TRUE(index->contains("Bar"));

  ASSERT_EQ(1, bundle.size());
  ASSERT_TRUE(bundle.contains(compId));

  cmd.undo();

  ASSERT_TRUE(index->contains(compId));
  ASSERT_TRUE(index->contains("Foo"));
  ASSERT_FALSE(index->contains("Bar"));

  ASSERT_EQ(1, bundle.size());
  ASSERT_TRUE(bundle.contains(compId));
}

}  // namespace tactile::test
