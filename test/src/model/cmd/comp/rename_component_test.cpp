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

#include "model/cmd/comp/rename_component.hpp"

#include <gtest/gtest.h>

#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"
#include "core/debug/panic.hpp"

namespace tactile::test {

TEST(RenameComponent, Constructor)
{
  ASSERT_THROW(cmd::RenameComponent(nullptr, make_uuid(), ""), TactileError);
}

TEST(RenameComponent, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto comp_id = ComponentBuilder {index, "Foo"}.result();

  auto& map = document->get_map();
  auto& bundle = map.get_ctx().comps();
  bundle.add(index->at(comp_id).instantiate());

  cmd::RenameComponent cmd {index, comp_id, "Bar"};
  cmd.redo();

  ASSERT_TRUE(index->contains(comp_id));
  ASSERT_FALSE(index->contains("Foo"));
  ASSERT_TRUE(index->contains("Bar"));

  ASSERT_EQ(1u, bundle.size());
  ASSERT_TRUE(bundle.contains(comp_id));

  cmd.undo();

  ASSERT_TRUE(index->contains(comp_id));
  ASSERT_TRUE(index->contains("Foo"));
  ASSERT_FALSE(index->contains("Bar"));

  ASSERT_EQ(1u, bundle.size());
  ASSERT_TRUE(bundle.contains(comp_id));
}

}  // namespace tactile::test
