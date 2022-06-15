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

#include "core/components/component_definition_manager.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"

using namespace tactile;
using namespace core;

TEST(ComponentDefinitionManager, Defaults)
{
  const ComponentDefinitionManager manager;
  ASSERT_EQ(0, manager.size());
}

TEST(ComponentDefinitionManager, DefineComp)
{
  ComponentDefinitionManager manager;
  const auto id = manager.define_comp("position");

  const auto& def = manager.at(id);
  ASSERT_EQ("position", def.get_name());
  ASSERT_EQ(id, def.get_uuid());
  ASSERT_TRUE(def.empty());

  ASSERT_TRUE(manager.contains("position"));
  ASSERT_EQ(1, manager.size());

  ASSERT_THROW(manager.define_comp("position"), TactileError);
}

TEST(ComponentDefinitionManager, RemoveComp)
{
  ComponentDefinitionManager manager;
  ASSERT_THROW(manager.remove_comp(make_uuid()), TactileError);

  const auto id = manager.define_comp("foo");
  ASSERT_EQ(1, manager.size());
  ASSERT_TRUE(manager.contains("foo"));

  manager.remove_comp(id);

  ASSERT_EQ(0, manager.size());
  ASSERT_FALSE(manager.contains("foo"));
}

TEST(ComponentDefinitionManager, RenameComp)
{
  ComponentDefinitionManager manager;
  ASSERT_THROW(manager.rename_comp(make_uuid(), "foo"), TactileError);

  const auto id = manager.define_comp("foo");

  ASSERT_TRUE(manager.contains("foo"));
  ASSERT_FALSE(manager.contains("zoo"));

  manager.rename_comp(id, "zoo");

  ASSERT_FALSE(manager.contains("foo"));
  ASSERT_TRUE(manager.contains("zoo"));

  manager.define_comp("woo");
  ASSERT_THROW(manager.rename_comp(id, "woo"), TactileError);
}
