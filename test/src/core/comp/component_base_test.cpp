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

#include "core/comp/component_base.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"

using namespace std::string_literals;

namespace tactile::test {

TEST(ComponentBase, Defaults)
{
  const ComponentBase component {make_uuid()};
  ASSERT_TRUE(component.empty());
  ASSERT_EQ(0u, component.size());

  usize count = 0;
  for (const auto& x [[maybe_unused]]: component) {
    ++count;
  }

  ASSERT_EQ(0u, count);
}

TEST(ComponentBase, AddWithType)
{
  ComponentBase component {make_uuid()};

  component.add("str");
  ASSERT_EQ(""s, component.at("str"));

  component.add("int", AttributeType::Int);
  ASSERT_EQ(0, component.at("int"));

  component.add("bool", AttributeType::Bool);
  ASSERT_EQ(false, component.at("bool"));
}

TEST(ComponentBase, AddWithValue)
{
  ComponentBase component {make_uuid()};

  component.add("A", 10);
  ASSERT_EQ(10, component.at("A"));

  ASSERT_TRUE(component.has("A"));
  ASSERT_FALSE(component.has("a"));

  ASSERT_EQ(1u, component.size());

  ASSERT_THROW(component.add("A"), TactileError);
}

TEST(ComponentBase, Update)
{
  ComponentBase component {make_uuid()};
  ASSERT_THROW(component.update("foo", 10), TactileError);

  component.add("foo");
  ASSERT_EQ(""s, component.at("foo"));
  ASSERT_EQ(1u, component.size());

  component.update("foo", "bar"s);
  ASSERT_EQ("bar"s, component.at("foo"));
  ASSERT_EQ(1u, component.size());
}

TEST(ComponentBase, Remove)
{
  ComponentBase component {make_uuid()};
  ASSERT_FALSE(component.remove("foo"));

  component.add("foo", Color {0xFF, 0, 0});
  ASSERT_EQ((Color {0xFF, 0, 0}), component.at("foo"));
  ASSERT_FALSE(component.empty());

  ASSERT_TRUE(component.remove("foo"));
  ASSERT_TRUE(component.empty());
}

TEST(ComponentBase, Rename)
{
  ComponentBase component {make_uuid()};
  ASSERT_FALSE(component.rename("foo", "bar"));

  component.add("foo", 123);
  ASSERT_TRUE(component.rename("foo", "bar"));
  ASSERT_FALSE(component.rename("abc", "def"));

  ASSERT_THROW(component.rename("", "bar"), TactileError);
}

TEST(ComponentBase, Duplicate)
{
  ComponentBase component {make_uuid()};
  ASSERT_THROW(component.duplicate("abc"), TactileError);

  component.add("abc", 3.5f);
  ASSERT_TRUE(component.has("abc"));
  ASSERT_EQ(1u, component.size());

  const auto new_name = component.duplicate("abc");
  ASSERT_TRUE(component.has("abc"));
  ASSERT_TRUE(component.has(new_name));
  ASSERT_EQ(3.5f, component.at(new_name));
  ASSERT_EQ(2u, component.size());
}

}  // namespace tactile::test