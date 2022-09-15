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

#include "core/cmd/comp/undef_component.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

using namespace std::string_literals;

namespace tactile::test {

TEST(UndefComponent, Constructor)
{
  ASSERT_THROW(cmd::UndefComponent(nullptr, make_uuid()), TactileError);
}

TEST(UndefComponent, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto comp_id = ComponentBuilder {index, "Demo"}  //
                           .with_attr("foo", "bar"s)
                           .with_attr("threshold", 1.5f)
                           .result();

  auto& map = document->get_map();
  auto& bundle = map.ctx().comps();

  bundle.add(index->at(comp_id).instantiate());
  bundle.at(comp_id).update_attr("foo", "abc"s);
  bundle.at(comp_id).update_attr("threshold", 8.9f);

  cmd::UndefComponent cmd {document.get(), comp_id};
  cmd.redo();

  ASSERT_FALSE(index->contains("Demo"));
  ASSERT_FALSE(index->contains(comp_id));

  ASSERT_FALSE(bundle.contains(comp_id));
  ASSERT_TRUE(bundle.empty());

  cmd.undo();

  ASSERT_TRUE(index->contains("Demo"));
  ASSERT_TRUE(index->contains(comp_id));

  ASSERT_EQ("bar", index->at(comp_id).get_attr("foo").as_string());
  ASSERT_EQ(1.5f, index->at(comp_id).get_attr("threshold").as_float());

  ASSERT_EQ("abc", bundle.at(comp_id).get_attr("foo").as_string());
  ASSERT_EQ(8.9f, bundle.at(comp_id).get_attr("threshold").as_float());
}

}  // namespace tactile::test
