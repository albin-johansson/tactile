/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "tactile/core/contexts/property_bundle.hpp"

#include <gtest/gtest.h>

#include "tactile/core/common/error.hpp"

using namespace tactile;

TEST(PropertyBundle, Add)
{
  PropertyBundle properties;

  properties.add("A");
  EXPECT_EQ(properties.size(), 1);
  EXPECT_TRUE(properties.has("A"));

  properties.add("B");
  EXPECT_EQ(properties.size(), 2);
  EXPECT_TRUE(properties.has("B"));
}

TEST(PropertyBundle, Remove)
{
  PropertyBundle properties;
  properties.add("1").set(1);
  properties.add("2").set("two");

  EXPECT_EQ(properties.size(), 2);
  EXPECT_TRUE(properties.has("1"));
  EXPECT_TRUE(properties.has("2"));

  EXPECT_EQ(properties.remove("1"), 1);

  EXPECT_EQ(properties.size(), 1);
  EXPECT_FALSE(properties.has("1"));
  EXPECT_TRUE(properties.has("2"));

  EXPECT_EQ(properties.remove("2"), "two");

  EXPECT_EQ(properties.size(), 0);
  EXPECT_FALSE(properties.has("1"));
  EXPECT_FALSE(properties.has("2"));

  EXPECT_THROW(properties.remove("1"), Error);
  EXPECT_THROW(properties.remove("2"), Error);
}

TEST(PropertyBundle, Update)
{
  PropertyBundle properties;
  properties.add("X");

  properties.update("X", 42);
  EXPECT_EQ(properties.get("X"), 42);

  properties.update("X", "str");
  EXPECT_EQ(properties.get("X"), "str");

  EXPECT_THROW(properties.update("Y", 100), Error);
}

TEST(PropertyBundle, Get)
{
  PropertyBundle properties;
  properties.add("X").set(42);

  EXPECT_EQ(properties.get("X"), 42);
  EXPECT_THROW((void) properties.get("Y"), Error);
}
