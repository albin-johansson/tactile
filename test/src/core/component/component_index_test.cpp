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

#include "core/component/component_index.hpp"

#include <gtest/gtest.h>

#include "core/component/component_definition.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(ComponentIndex, Defaults)
{
  const ComponentIndex index;
  ASSERT_EQ(0u, index.size());
}

TEST(ComponentIndex, Define)
{
  ComponentIndex index;
  const auto id = index.define("position");

  const auto& def = index.at(id);
  ASSERT_EQ("position", def.name());
  ASSERT_EQ(id, def.get_uuid());
  ASSERT_TRUE(def.empty());

  ASSERT_TRUE(index.contains("position"));
  ASSERT_EQ(1u, index.size());

  ASSERT_THROW(index.define("position"), TactileError);
}

TEST(ComponentIndex, Remove)
{
  ComponentIndex index;
  ASSERT_THROW(index.remove(make_uuid()), TactileError);

  const auto id = index.define("foo");
  ASSERT_EQ(1u, index.size());
  ASSERT_TRUE(index.contains("foo"));

  index.remove(id);

  ASSERT_EQ(0u, index.size());
  ASSERT_FALSE(index.contains("foo"));
}

TEST(ComponentIndex, Rename)
{
  ComponentIndex index;
  ASSERT_THROW(index.rename(make_uuid(), "foo"), TactileError);

  const auto id = index.define("foo");

  ASSERT_TRUE(index.contains("foo"));
  ASSERT_FALSE(index.contains("zoo"));

  index.rename(id, "zoo");

  ASSERT_FALSE(index.contains("foo"));
  ASSERT_TRUE(index.contains("zoo"));

  index.define("woo");
  ASSERT_THROW(index.rename(id, "woo"), TactileError);
}

}  // namespace tactile::test
