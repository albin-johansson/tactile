// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/lookup.hpp"

#include <unordered_map>  // unordered_map

#include <gtest/gtest.h>

#include "tactile/base/container/string.hpp"
#include "tactile/base/container/string_map.hpp"
#include "tactile/base/container/tree_map.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

/// \trace tactile::exists_in
/// \trace tactile::find_in
/// \trace tactile::lookup_in
TEST(Lookup, EmptyMap)
{
  const StringMap<int> hash_map {};
  const TreeMap<String, int> tree_map {};

  EXPECT_FALSE(exists_in(hash_map, "foo"));
  EXPECT_FALSE(exists_in(tree_map, "foo"));

  EXPECT_EQ(find_in(hash_map, "foo"), nullptr);
  EXPECT_EQ(find_in(tree_map, "foo"), nullptr);

  EXPECT_THROW((void) lookup_in(hash_map, "foo"), Exception);
  EXPECT_THROW((void) lookup_in(tree_map, "foo"), Exception);
}

/// \trace tactile::find_in
TEST(Lookup, FindIn)
{
  TreeMap<int, String> map {};
  map[0] = "0";
  map[1] = "1";

  const auto& const_map = map;

  EXPECT_EQ(find_in(map, 0), &map.at(0));
  EXPECT_EQ(find_in(map, 1), &map.at(1));
  EXPECT_EQ(find_in(map, 2), nullptr);
  EXPECT_EQ(find_in(const_map, 0), &const_map.at(0));
  EXPECT_EQ(find_in(const_map, 1), &const_map.at(1));
  EXPECT_EQ(find_in(const_map, 2), nullptr);
}

/// \trace tactile::lookup_in
TEST(Lookup, LookupIn)
{
  StringMap<String> map {};
  map["A"] = "1";
  map["B"] = "2";

  const auto& const_map = map;

  EXPECT_EQ(lookup_in(map, "A"), "1");
  EXPECT_EQ(lookup_in(map, "B"), "2");
  EXPECT_EQ(lookup_in(const_map, "A"), "1");
  EXPECT_EQ(lookup_in(const_map, "B"), "2");

  EXPECT_THROW((void) lookup_in(map, ""), Exception);
  EXPECT_THROW((void) lookup_in(map, "a"), Exception);
  EXPECT_THROW((void) lookup_in(const_map, ""), Exception);
  EXPECT_THROW((void) lookup_in(const_map, "a"), Exception);
}

/// \trace tactile::erase_from
TEST(Lookup, EraseFrom)
{
  StringMap<int> map {};
  map["A"] = 0xA;
  map["B"] = 0xB;

  EXPECT_EQ(map.size(), 2);

  erase_from(map, "A");
  erase_from(map, "A");

  EXPECT_EQ(map.size(), 1);

  erase_from(map, "B");
  erase_from(map, "B");

  EXPECT_EQ(map.size(), 0);
}

/// \trace tactile::take_from
TEST(Lookup, TakeFrom)
{
  StringMap<int> map {};
  map["A"] = 0xA;
  map["B"] = 0xB;

  EXPECT_EQ(take_from(map, "A"), 0xA);
  EXPECT_EQ(take_from(map, "A"), std::nullopt);

  EXPECT_EQ(take_from(map, "B"), 0xB);
  EXPECT_EQ(take_from(map, "B"), std::nullopt);

  EXPECT_EQ(map.size(), 0);
}

/// \trace tactile::exists_in
TEST(Lookup, ExistsIn)
{
  StringMap<int> map {};
  map["A"] = 10;
  map["B"] = 20;
  map["C"] = 30;

  ASSERT_EQ(map.size(), 3);
  EXPECT_TRUE(exists_in(map, "A"));
  EXPECT_TRUE(exists_in(map, "B"));
  EXPECT_TRUE(exists_in(map, "C"));

  EXPECT_FALSE(exists_in(map, ""));
  EXPECT_FALSE(exists_in(map, "A "));
  EXPECT_FALSE(exists_in(map, "a"));
  EXPECT_FALSE(exists_in(map, "b"));
  EXPECT_FALSE(exists_in(map, "c"));
}

}  // namespace tactile
