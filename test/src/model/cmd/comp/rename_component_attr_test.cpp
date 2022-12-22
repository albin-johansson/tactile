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

#include "model/cmd/comp/rename_component_attr.hpp"

#include <gtest/gtest.h>

#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(RenameComponentAttr, Constructor)
{
  ASSERT_THROW(cmd::RenameComponentAttr(nullptr, make_uuid(), "", ""), TactileError);
}

TEST(RenameComponentAttr, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto comp_id = ComponentBuilder {index, "Demo"}  //
                           .with_attr("Foo", 123)
                           .result();

  auto& map = document->get_map();
  auto& bundle = map.get_ctx().comps();
  bundle.add(index->at(comp_id).instantiate());

  cmd::RenameComponentAttr cmd {document.get(), comp_id, "Foo", "Bar"};
  cmd.redo();

  {
    const auto& def = index->at(comp_id);
    const auto& comp = bundle.at(comp_id);

    ASSERT_FALSE(def.has("Foo"));
    ASSERT_FALSE(comp.has("Foo"));

    ASSERT_TRUE(def.has("Bar"));
    ASSERT_TRUE(comp.has("Bar"));

    ASSERT_EQ(123, def.at("Bar"));
    ASSERT_EQ(123, comp.at("Bar"));
  }

  cmd.undo();

  {
    const auto& def = index->at(comp_id);
    const auto& comp = bundle.at(comp_id);

    ASSERT_TRUE(def.has("Foo"));
    ASSERT_TRUE(comp.has("Foo"));

    ASSERT_FALSE(def.has("Bar"));
    ASSERT_FALSE(comp.has("Bar"));

    ASSERT_EQ(123, def.at("Foo"));
    ASSERT_EQ(123, comp.at("Foo"));
  }
}

}  // namespace tactile::test
