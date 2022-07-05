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

#include "core/components/component_definition.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"

using namespace std::string_literals;

using namespace tactile;

TEST(ComponentDefinition, Defaults)
{
  const ComponentDefinition def;
  ASSERT_FALSE(def.get_uuid().is_nil());
  ASSERT_EQ(0, def.size());
  ASSERT_TRUE(def.empty());
}

TEST(ComponentDefinition, AddAttrWithType)
{
  ComponentDefinition def;

  def.add_attr("str");
  ASSERT_EQ(""s, def.get_attr("str").as_string());

  def.add_attr("abc", AttributeType::Int);
  ASSERT_EQ(0, def.get_attr("abc").as_int());
  ASSERT_THROW(def.add_attr("abc", AttributeType::Float), TactileError);

  def.add_attr("def", AttributeType::Bool);
  ASSERT_FALSE(def.get_attr("def").as_bool());
}

TEST(ComponentDefinition, AddAttrWithValue)
{
  ComponentDefinition def;

  def.add_attr("foo", "bar"s);
  ASSERT_TRUE(def.has_attr("foo"));
  ASSERT_THROW(def.add_attr("foo", 42), TactileError);

  def.add_attr("bar", 123);
  ASSERT_EQ(123, def.get_attr("bar"));
}

TEST(ComponentDefinition, UpdateAttr)
{
  ComponentDefinition def;
  ASSERT_THROW(def.update_attr("foo", 42), TactileError);

  def.add_attr("foo", "str"s);
  ASSERT_EQ("str", def.get_attr("foo").as_string());

  def.update_attr("foo", 27);
  ASSERT_EQ(27, def.get_attr("foo").as_int());

  def.update_attr("foo", cen::colors::red);
  ASSERT_EQ(cen::colors::red, def.get_attr("foo").as_color());
}

TEST(ComponentDefinition, RemoveAttr)
{
  ComponentDefinition def;
  ASSERT_THROW(def.remove_attr("abc"), TactileError);

  def.add_attr("foo", 3.5f);
  def.add_attr("bar", true);

  ASSERT_TRUE(def.has_attr("foo"));
  ASSERT_TRUE(def.has_attr("bar"));

  def.remove_attr("foo");

  ASSERT_FALSE(def.has_attr("foo"));
  ASSERT_TRUE(def.has_attr("bar"));
}

TEST(ComponentDefinition, RenameAttr)
{
  ComponentDefinition def;
  ASSERT_THROW(def.rename_attr("foo", "boo"), TactileError);

  def.add_attr("foo");

  ASSERT_EQ(1, def.size());
  ASSERT_TRUE(def.has_attr("foo"));
  ASSERT_FALSE(def.has_attr("boo"));

  def.rename_attr("foo", "boo");

  ASSERT_EQ(1, def.size());
  ASSERT_FALSE(def.has_attr("foo"));
  ASSERT_TRUE(def.has_attr("boo"));
}

TEST(ComponentDefinition, DuplicateAttr)
{
  ComponentDefinition def;
  ASSERT_THROW(def.duplicate_attr("foo"), TactileError);

  def.add_attr("hello", "world"s);
  ASSERT_EQ(1, def.size());

  const auto key = def.duplicate_attr("hello");
  ASSERT_EQ(2, def.size());

  ASSERT_NE("hello", key);
  ASSERT_EQ(def.get_attr("hello"), def.get_attr(key));
}
