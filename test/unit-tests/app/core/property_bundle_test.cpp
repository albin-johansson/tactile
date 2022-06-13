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

#include "core/property_bundle.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"

using namespace std::string_literals;
using namespace tactile;
using namespace core;

TEST(PropertyBundle, Defaults)
{
  const PropertyBundle bundle;
  ASSERT_EQ(0u, bundle.size());
  ASSERT_TRUE(bundle.empty());
}

TEST(PropertyBundle, AddWithType)
{
  PropertyBundle bundle;

  bundle.add("foo", AttributeType::Int);
  ASSERT_EQ(0, bundle.at("foo").as_int());

  ASSERT_THROW(bundle.add("foo", AttributeType::Float), TactileError);

  bundle.add("bar", AttributeType::Float);
  ASSERT_EQ(0.0f, bundle.at("bar").as_float());

  ASSERT_EQ(2, bundle.size());
}

TEST(PropertyBundle, AddWithValue)
{
  PropertyBundle bundle;

  bundle.add("foo", "bar"s);
  ASSERT_TRUE(bundle.contains("foo"));

  ASSERT_EQ("bar", bundle.at("foo").as_string());
  ASSERT_THROW(bundle.add("foo", 42), TactileError);
}

TEST(PropertyBundle, Update)
{
  PropertyBundle bundle;
  ASSERT_THROW(bundle.update("abc", 10), TactileError);

  bundle.add("qwerty", AttributeType::Int);
  ASSERT_EQ(0, bundle.at("qwerty"));

  bundle.update("qwerty", 42);
  ASSERT_EQ(42, bundle.at("qwerty"));

  bundle.update("qwerty", "str"s);
  ASSERT_EQ("str"s, bundle.at("qwerty"));
}

TEST(PropertyBundle, ChangeType)
{
  PropertyBundle bundle;
  bundle.add("abc", AttributeType::Int);
  ASSERT_EQ(0, bundle.at("abc").as_int());

  ASSERT_EQ(0, bundle.change_type("abc", AttributeType::Bool));
  ASSERT_FALSE(bundle.at("abc").as_bool());
}

TEST(PropertyBundle, Find)
{
  PropertyBundle bundle;
  const auto& ref = bundle;

  ASSERT_FALSE(bundle.find("foo"));
  ASSERT_FALSE(ref.find("foo"));

  bundle.add("foo", 123);

  ASSERT_EQ(123, bundle.find("foo")->as_int());
  ASSERT_EQ(123, ref.find("foo")->as_int());
}

TEST(PropertyBundle, At)
{
  PropertyBundle bundle;
  const auto& ref = bundle;

  ASSERT_THROW(bundle.at("foo"), TactileError);
  ASSERT_THROW(ref.at("foo"), TactileError);

  bundle.add("foo", true);

  ASSERT_TRUE(bundle.at("foo").as_bool());
  ASSERT_TRUE(ref.at("foo").as_bool());
}

TEST(PropertyBundle, Contains)
{
  PropertyBundle bundle;
  ASSERT_FALSE(bundle.contains("xyz"));

  bundle.add("xyz", 4.5f);
  ASSERT_TRUE(bundle.contains("xyz"));
  ASSERT_FALSE(bundle.contains("XYZ"));
}