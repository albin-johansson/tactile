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

#include "core/comp/component_definition.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"

using namespace eastl::string_literals;

namespace tactile::test {

TEST(ComponentDefinition, Defaults)
{
  const ComponentDefinition def;
  ASSERT_FALSE(def.uuid().is_nil());
  ASSERT_EQ(0, def.size());
  ASSERT_TRUE(def.empty());
}

TEST(ComponentDefinition, AddWithType)
{
  ComponentDefinition def;

  def.add("str");
  ASSERT_EQ(""s, def.at("str").as_string());

  def.add("abc", AttributeType::Int);
  ASSERT_EQ(0, def.at("abc").as_int());
  ASSERT_THROW(def.add("abc", AttributeType::Float), TactileError);

  def.add("def", AttributeType::Bool);
  ASSERT_FALSE(def.at("def").as_bool());
}

TEST(ComponentDefinition, AddWithValue)
{
  ComponentDefinition def;

  def.add("foo", "bar"s);
  ASSERT_TRUE(def.has("foo"));
  ASSERT_THROW(def.add("foo", 42), TactileError);

  def.add("bar", 123);
  ASSERT_EQ(123, def.at("bar"));
}

TEST(ComponentDefinition, Update)
{
  ComponentDefinition def;
  ASSERT_THROW(def.update("foo", 42), TactileError);

  def.add("foo", "str"s);
  ASSERT_EQ("str", def.at("foo").as_string());

  def.update("foo", 27);
  ASSERT_EQ(27, def.at("foo").as_int());

  def.update("foo", cen::colors::red);
  ASSERT_EQ(cen::colors::red, def.at("foo").as_color());
}

TEST(ComponentDefinition, Remove)
{
  ComponentDefinition def;
  ASSERT_FALSE(def.remove("abc"));

  def.add("foo", 3.5f);
  def.add("bar", true);

  ASSERT_TRUE(def.has("foo"));
  ASSERT_TRUE(def.has("bar"));

  ASSERT_TRUE(def.remove("foo"));

  ASSERT_FALSE(def.has("foo"));
  ASSERT_TRUE(def.has("bar"));
}

TEST(ComponentDefinition, Rename)
{
  ComponentDefinition def;
  ASSERT_FALSE(def.rename("foo", "boo"));

  def.add("foo");

  ASSERT_EQ(1, def.size());
  ASSERT_TRUE(def.has("foo"));
  ASSERT_FALSE(def.has("boo"));

  ASSERT_TRUE(def.rename("foo", "boo"));

  ASSERT_EQ(1, def.size());
  ASSERT_FALSE(def.has("foo"));
  ASSERT_TRUE(def.has("boo"));
}

TEST(ComponentDefinition, Duplicate)
{
  ComponentDefinition def;
  ASSERT_THROW(def.duplicate("foo"), TactileError);

  def.add("hello", "world"s);
  ASSERT_EQ(1, def.size());

  const auto key = def.duplicate("hello");
  ASSERT_EQ(2, def.size());

  ASSERT_NE("hello", key);
  ASSERT_EQ(def.at("hello"), def.at(key));
}

}  // namespace tactile::test
