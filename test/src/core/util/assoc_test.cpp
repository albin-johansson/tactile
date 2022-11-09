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

#include "core/util/assoc.hpp"

#include <gtest/gtest.h>

#include "core/attribute.hpp"
#include "core/type/hash_map.hpp"
#include "core/type/string.hpp"
#include "core/type/string_map.hpp"
#include "core/type/tree_map.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace tactile::test {

TEST(Assoc, EmptyHashMap)
{
  const StringMap<int> map;

  ASSERT_FALSE(has_key(map, "foo"));
  ASSERT_THROW((void) lookup_in(map, "foo"), TactileError);
}

TEST(Assoc, EmptyTreeMap)
{
  const TreeMap<String, int> map;

  ASSERT_FALSE(has_key(map, "foo"));
  ASSERT_THROW((void) lookup_in(map, "foo"), TactileError);
}

TEST(Assoc, LookupInHashMap)
{
  StringMap<Attribute> map;
  map["foo"] = 3.5f;
  map["bar"] = true;

  ASSERT_THROW((void) lookup_in(map, ""), TactileError);
  ASSERT_THROW((void) lookup_in(map, "foO"), TactileError);
  ASSERT_THROW((void) lookup_in(map, "BAR"), TactileError);

  ASSERT_EQ(3.5f, lookup_in(map, "foo"));
  ASSERT_EQ(true, lookup_in(map, "bar"));
}

TEST(Assoc, LookupInTreeMap)
{
  TreeMap<int, Attribute> map;
  map[10] = "hello"s;
  map[11] = cen::colors::red;

  ASSERT_THROW((void) lookup_in(map, 9), TactileError);
  ASSERT_THROW((void) lookup_in(map, 12), TactileError);

  ASSERT_EQ("hello"s, lookup_in(map, 10));
  ASSERT_EQ(cen::colors::red, lookup_in(map, 11));
}

TEST(Assoc, HashMapHasKey)
{
  StringMap<int> map;
  map["foo"] = 10;
  map["bar"] = 20;
  map["abc"] = 30;

  ASSERT_EQ(3, map.size());

  ASSERT_TRUE(has_key(map, "foo"));
  ASSERT_TRUE(has_key(map, "bar"));
  ASSERT_TRUE(has_key(map, "abc"));

  ASSERT_FALSE(has_key(map, ""));
  ASSERT_FALSE(has_key(map, "fooo"));
  ASSERT_FALSE(has_key(map, "foO"));
  ASSERT_FALSE(has_key(map, "BAR"));
  ASSERT_FALSE(has_key(map, "bar "));
}

TEST(Assoc, TreeMapHasKey)
{
  TreeMap<StringView, float> map;
  map["A"] = 1.0f;
  map["B"] = 42.0f;

  ASSERT_EQ(2, map.size());

  ASSERT_TRUE(has_key(map, "A"));
  ASSERT_TRUE(has_key(map, "A"s));
  ASSERT_TRUE(has_key(map, "A"sv));
  ASSERT_TRUE(has_key(map, "B"));
  ASSERT_TRUE(has_key(map, "B"s));
  ASSERT_TRUE(has_key(map, "B"sv));

  ASSERT_FALSE(has_key(map, ""));
  ASSERT_FALSE(has_key(map, "a"));
  ASSERT_FALSE(has_key(map, "b"));
  ASSERT_FALSE(has_key(map, "c"));
  ASSERT_FALSE(has_key(map, "c"s));
  ASSERT_FALSE(has_key(map, "c"sv));
}

}  // namespace tactile::test