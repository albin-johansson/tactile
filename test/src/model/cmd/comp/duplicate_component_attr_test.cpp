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

#include "model/cmd/comp/duplicate_component_attr.hpp"

#include <gtest/gtest.h>

#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(DuplicateComponentAttr, Constructor)
{
  ASSERT_THROW(cmd::DuplicateComponentAttr(nullptr, make_uuid(), ""), TactileError);
}

TEST(DuplicateComponentAttr, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto comp_id = ComponentBuilder {index, "Demo"}  //
                           .with_attr("Attr", 42)
                           .result();

  auto& map = document->get_map();
  auto& bundle = map.ctx().comps();
  bundle.add(index->at(comp_id).instantiate());

  cmd::DuplicateComponentAttr cmd {document.get(), comp_id, "Attr"};
  cmd.redo();

  {
    const auto& def = index->at(comp_id);
    const auto& comp = bundle.at(comp_id);

    ASSERT_EQ(2u, def.size());
    ASSERT_TRUE(def.has("Attr"));
    ASSERT_TRUE(def.has("Attr (1)"));
    ASSERT_EQ(def.at("Attr"), def.at("Attr (1)"));

    ASSERT_EQ(2u, comp.size());
    ASSERT_TRUE(comp.has("Attr"));
    ASSERT_TRUE(comp.has("Attr (1)"));
    ASSERT_EQ(comp.at("Attr"), comp.at("Attr (1)"));
  }

  cmd.undo();

  {
    const auto& def = index->at(comp_id);
    const auto& comp = bundle.at(comp_id);

    ASSERT_EQ(1u, def.size());
    ASSERT_TRUE(def.has("Attr"));
    ASSERT_FALSE(def.has("Attr (1)"));

    ASSERT_EQ(1u, comp.size());
    ASSERT_TRUE(comp.has("Attr"));
    ASSERT_FALSE(comp.has("Attr (1)"));
  }
}

}  // namespace tactile::test
