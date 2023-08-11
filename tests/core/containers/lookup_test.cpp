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

#include "tactile/core/containers/lookup.hpp"

#include <unordered_map>  // unordered_map

#include <gtest/gtest.h>

#include "tactile/core/common/error.hpp"
#include "tactile/core/containers/string_map.hpp"
#include "tactile/core/contexts/property.hpp"

using namespace tactile;

TEST(Lookup, FindIn)
{
  StringMap<Property> map;
  map["foo"] = 3.5f;
  map["bar"] = true;

  EXPECT_NE(find_in(map, "foo"), nullptr);
  EXPECT_NE(find_in(map, "bar"), nullptr);

  EXPECT_EQ(find_in(map, "fooo"), nullptr);
  EXPECT_EQ(find_in(map, "foO"), nullptr);
  EXPECT_EQ(find_in(map, "bar "), nullptr);
}

TEST(Lookup, LookupIn)
{
  StringMap<int> map;
  map["10"] = 10;
  map["20"] = 20;
  map["30"] = 30;

  EXPECT_EQ(lookup_in(map, "10"), 10);
  EXPECT_EQ(lookup_in(map, "20"), 20);
  EXPECT_EQ(lookup_in(map, "30"), 30);

  EXPECT_THROW((void) lookup_in(map, "11"), Error);
}

TEST(Lookup, ExistsIn)
{
  std::unordered_map<int, Property> map;
  map[0] = 10.0f;
  map[1] = 45;

  EXPECT_TRUE(exists_in(map, 0));
  EXPECT_TRUE(exists_in(map, 1));
  EXPECT_FALSE(exists_in(map, 2));
}
