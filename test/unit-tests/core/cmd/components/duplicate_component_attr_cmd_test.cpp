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

#include "core/cmd/comp/duplicate_component_attr_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(DuplicateComponentAttrCmd, Constructor)
{
  ASSERT_THROW(DuplicateComponentAttrCmd(nullptr, make_uuid(), ""), TactileError);
}

TEST(DuplicateComponentAttrCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Demo"}  //
                          .with_attr("Attr", 42)
                          .result();

  auto& map = document->get_map();
  auto& bundle = map.get_comps();
  bundle.add(index->at(compId).instantiate());

  DuplicateComponentAttrCmd cmd {document.get(), compId, "Attr"};
  cmd.redo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_EQ(2, def.size());
    ASSERT_TRUE(def.has_attr("Attr"));
    ASSERT_TRUE(def.has_attr("Attr (1)"));
    ASSERT_EQ(def.get_attr("Attr"), def.get_attr("Attr (1)"));

    ASSERT_EQ(2, comp.size());
    ASSERT_TRUE(comp.has_attr("Attr"));
    ASSERT_TRUE(comp.has_attr("Attr (1)"));
    ASSERT_EQ(comp.get_attr("Attr"), comp.get_attr("Attr (1)"));
  }

  cmd.undo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_EQ(1, def.size());
    ASSERT_TRUE(def.has_attr("Attr"));
    ASSERT_FALSE(def.has_attr("Attr (1)"));

    ASSERT_EQ(1, comp.size());
    ASSERT_TRUE(comp.has_attr("Attr"));
    ASSERT_FALSE(comp.has_attr("Attr (1)"));
  }
}

}  // namespace tactile::test
