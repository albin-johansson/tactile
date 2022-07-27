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

#include "core/cmd/comp/remove_component_attr_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RemoveComponentAttrCmd, Constructor)
{
  ASSERT_THROW(RemoveComponentAttrCmd(nullptr, make_uuid(), ""), TactileError);
}

TEST(RemoveComponentAttrCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Demo"}.with_attr("attr", 42).result();

  auto& definition = index->at(compId);
  auto& map = document->get_map();
  auto& bundle = map.get_comps();

  bundle.add(definition.instantiate());
  ASSERT_TRUE(bundle.contains(compId));
  ASSERT_EQ(42, bundle.at(compId).get_attr("attr"));

  RemoveComponentAttrCmd cmd {document.get(), compId, "attr"};

  cmd.redo();
  ASSERT_TRUE(definition.empty());
  ASSERT_TRUE(bundle.at(compId).empty());

  cmd.undo();
  ASSERT_TRUE(definition.has_attr("attr"));
  ASSERT_EQ(42, definition.get_attr("attr"));

  ASSERT_TRUE(bundle.at(compId).has_attr("attr"));
  ASSERT_EQ(definition.get_attr("attr"), bundle.at(compId).get_attr("attr"));
}

}  // namespace tactile::test
