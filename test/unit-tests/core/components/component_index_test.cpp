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

#include "core/components/component_index.hpp"

#include <gtest/gtest.h>

#include "core/components/component_definition.hpp"
#include "misc/panic.hpp"

using namespace tactile;

TEST(ComponentIndex, Defaults)
{
  const ComponentIndex index;
  ASSERT_EQ(0, index.size());
}

TEST(ComponentIndex, DefineComp)
{
  ComponentIndex index;
  const auto     id = index.define_comp("position");

  const auto& def = index.at(id);
  ASSERT_EQ("position", def.get_name());
  ASSERT_EQ(id, def.get_uuid());
  ASSERT_TRUE(def.empty());

  ASSERT_TRUE(index.contains("position"));
  ASSERT_EQ(1, index.size());

  ASSERT_THROW(index.define_comp("position"), TactileError);
}

TEST(ComponentIndex, RemoveComp)
{
  ComponentIndex index;
  ASSERT_THROW(index.remove_comp(make_uuid()), TactileError);

  const auto id = index.define_comp("foo");
  ASSERT_EQ(1, index.size());
  ASSERT_TRUE(index.contains("foo"));

  index.remove_comp(id);

  ASSERT_EQ(0, index.size());
  ASSERT_FALSE(index.contains("foo"));
}

TEST(ComponentIndex, RenameComp)
{
  ComponentIndex index;
  ASSERT_THROW(index.rename_comp(make_uuid(), "foo"), TactileError);

  const auto id = index.define_comp("foo");

  ASSERT_TRUE(index.contains("foo"));
  ASSERT_FALSE(index.contains("zoo"));

  index.rename_comp(id, "zoo");

  ASSERT_FALSE(index.contains("foo"));
  ASSERT_TRUE(index.contains("zoo"));

  index.define_comp("woo");
  ASSERT_THROW(index.rename_comp(id, "woo"), TactileError);
}
