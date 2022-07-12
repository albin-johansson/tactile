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

#include "core/cmd/comp/rename_component_attr_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RenameComponentAttrCmd, Constructor)
{
  ASSERT_THROW(RenameComponentAttrCmd(nullptr, make_uuid(), "", ""), TactileError);
}

TEST(RenameComponentAttrCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Demo"}  //
                          .with_attr("Foo", 123)
                          .result();

  auto& map = document->get_map();
  auto& bundle = map.get_comps();
  bundle.add(index->at(compId).instantiate());

  RenameComponentAttrCmd cmd {document.get(), compId, "Foo", "Bar"};
  cmd.redo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_FALSE(def.has_attr("Foo"));
    ASSERT_FALSE(comp.has_attr("Foo"));

    ASSERT_TRUE(def.has_attr("Bar"));
    ASSERT_TRUE(comp.has_attr("Bar"));

    ASSERT_EQ(123, def.get_attr("Bar"));
    ASSERT_EQ(123, comp.get_attr("Bar"));
  }

  cmd.undo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_TRUE(def.has_attr("Foo"));
    ASSERT_TRUE(comp.has_attr("Foo"));

    ASSERT_FALSE(def.has_attr("Bar"));
    ASSERT_FALSE(comp.has_attr("Bar"));

    ASSERT_EQ(123, def.get_attr("Foo"));
    ASSERT_EQ(123, comp.get_attr("Foo"));
  }
}

}  // namespace tactile::test
