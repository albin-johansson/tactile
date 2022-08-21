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

#include "core/cmd/comp/set_component_attr_type_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(SetComponentAttrTypeCmd, Constructor)
{
  ASSERT_THROW(SetComponentAttrTypeCmd(nullptr, make_uuid(), "", AttributeType::String),
               TactileError);
}

TEST(SetComponentAttrTypeCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Demo"}  //
                          .with_attr("Attr", true)
                          .with_attr("Attr2", 938)
                          .with_attr("Attr3", 94.3f)
                          .result();

  auto& map = document->get_map();
  auto& bundle = map.get_comps();
  bundle.add(index->at(compId).instantiate());

  SetComponentAttrTypeCmd cmd {document.get(), compId, "Attr", AttributeType::Int};
  cmd.redo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_EQ(AttributeType::Int, def.get_attr("Attr").type());
    ASSERT_EQ(AttributeType::Int, comp.get_attr("Attr").type());

    ASSERT_EQ(0, def.get_attr("Attr").as_int());
    ASSERT_EQ(0, comp.get_attr("Attr").as_int());
  }

  cmd.undo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_EQ(AttributeType::Bool, def.get_attr("Attr").type());
    ASSERT_EQ(AttributeType::Bool, comp.get_attr("Attr").type());

    ASSERT_TRUE(def.get_attr("Attr").as_bool());
    ASSERT_TRUE(comp.get_attr("Attr").as_bool());
  }
}

}  // namespace tactile::test
