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

#include "model/cmd/comp/add_component_attr.hpp"

#include <gtest/gtest.h>

#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(AddComponentAttr, Constructor)
{
  ASSERT_THROW(cmd::AddComponentAttr(nullptr, make_uuid(), ""), TactileError);
}

TEST(AddComponentAttr, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto comp_id = ComponentBuilder {index, "Demo"}.result();
  auto& definition = index->at(comp_id);
  ASSERT_TRUE(definition.empty());

  auto& map = document->get_map();
  auto& bundle = map.ctx().comps();

  bundle.add(definition.instantiate());
  ASSERT_TRUE(bundle.contains(comp_id));
  ASSERT_TRUE(bundle.at(comp_id).empty());

  cmd::AddComponentAttr cmd {document.get(), comp_id, "attr"};

  cmd.redo();
  ASSERT_EQ(1, definition.size());
  ASSERT_TRUE(definition.has("attr"));
  ASSERT_EQ(definition.at("attr"), bundle.at(comp_id).at("attr"));

  cmd.undo();
  ASSERT_TRUE(definition.empty());
  ASSERT_FALSE(definition.has("attr"));
  ASSERT_THROW(bundle.at(comp_id).at("attr"), TactileError);
}

}  // namespace tactile::test
