// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/container/lookup.hpp"

#include <gtest/gtest.h>

#include "tactile/core/container/hash_map.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/container/string_map.hpp"
#include "tactile/core/container/tree_map.hpp"
#include "tactile/core/debug/error.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

using namespace tactile;
using tactile::int_literals::operator""_uz;

TEST(Lookup, EmptyMap)
{
  const StringMap<int> hash_map;
  const TreeMap<String, int> tree_map;

  EXPECT_FALSE(exists_in(hash_map, "foo"));
  EXPECT_FALSE(exists_in(tree_map, "foo"));

  EXPECT_EQ(find_in(hash_map, "foo"), nullptr);
  EXPECT_EQ(find_in(tree_map, "foo"), nullptr);

  EXPECT_THROW((void) lookup_in(hash_map, "foo"), Error);
  EXPECT_THROW((void) lookup_in(tree_map, "foo"), Error);
}

TEST(Lookup, FindIn)
{
  TreeMap<int, String> map;
  map[0] = "0";
  map[1] = "1";

  EXPECT_EQ(find_in(map, 0), &map.at(0));
  EXPECT_EQ(find_in(map, 1), &map.at(1));
  EXPECT_EQ(find_in(map, 2), nullptr);
}

TEST(Lookup, LookupIn)
{
  StringMap<String> map;
  map["A"] = "1";
  map["B"] = "2";

  EXPECT_EQ(lookup_in(map, "A"), "1");
  EXPECT_EQ(lookup_in(map, "B"), "2");

  EXPECT_THROW((void) lookup_in(map, ""), Error);
  EXPECT_THROW((void) lookup_in(map, "a"), Error);
}

TEST(Lookup, ExistsIn)
{
  StringMap<int> map;
  map["A"] = 10;
  map["B"] = 20;
  map["C"] = 30;

  ASSERT_EQ(map.size(), 3_uz);
  EXPECT_TRUE(exists_in(map, "A"));
  EXPECT_TRUE(exists_in(map, "B"));
  EXPECT_TRUE(exists_in(map, "C"));

  EXPECT_FALSE(exists_in(map, ""));
  EXPECT_FALSE(exists_in(map, "A "));
  EXPECT_FALSE(exists_in(map, "a"));
  EXPECT_FALSE(exists_in(map, "b"));
  EXPECT_FALSE(exists_in(map, "c"));
}
