#include "vector_map.hpp"

#include <gtest/gtest.h>

#include <string>
#include <string_view>

using namespace tactile;
using namespace std::string_view_literals;

TEST(VectorMap, Defaults)
{
  const vector_map<int, int> map;
  EXPECT_EQ(0, map.capacity());
  EXPECT_EQ(0, map.size());
  EXPECT_TRUE(map.empty());
}

TEST(VectorMap, Emplace)
{
  vector_map<int, std::string> map;
  const auto i = 7;

  {  // Emplacing one string
    auto& [key, value] = map.emplace(i, "hello");
    EXPECT_EQ(key, i);
    EXPECT_EQ("hello"sv, value);
  }

  {  // Overwriting the string with the same key
    auto& [key, value] = map.emplace(i, "world");
    EXPECT_EQ(1, map.size());
    EXPECT_EQ(key, i);
    EXPECT_EQ("world"sv, value);
  }

  {  // Emplacing a pair
    vector_map<int, std::pair<int, double>> pairs;
    auto& [key, value] = pairs.emplace(24, 13, 37.3);
    EXPECT_EQ(24, key);
    EXPECT_EQ(13, value.first);
    EXPECT_EQ(37.3, value.second);
  }
}

TEST(VectorMap, Erase)
{
  vector_map<int, std::string> map;
  EXPECT_NO_THROW(map.erase(0));

  map.emplace(0, "foo");
  map.emplace(1, "bar");
  EXPECT_TRUE(map.contains(0));
  EXPECT_TRUE(map.contains(1));
  EXPECT_FALSE(map.empty());

  map.erase(0);
  EXPECT_FALSE(map.contains(0));
  EXPECT_TRUE(map.contains(1));
  EXPECT_FALSE(map.empty());

  map.erase(1);
  EXPECT_FALSE(map.contains(1));
  EXPECT_TRUE(map.empty());
}

TEST(VectorMap, Reserve)
{
  constexpr auto n = 10;

  vector_map<int, int> map;
  map.reserve(n);

  EXPECT_EQ(n, map.capacity());
}

TEST(VectorMap, Clear)
{
  vector_map<int, int> map;

  map.emplace(0, 10);
  map.emplace(1, 20);
  map.emplace(2, 30);

  map.clear();

  EXPECT_TRUE(map.empty());
}

TEST(VectorMap, At)
{
  vector_map<int, std::string> map;
  EXPECT_THROW(map.at(0), std::out_of_range);

  map.emplace(0, "foo");
  const auto& str = map.at(0);
  EXPECT_EQ("foo"sv, str);
}

TEST(VectorMap, SubscriptOperator)
{
  vector_map<int, std::string> map;
  EXPECT_THROW(map[0], std::out_of_range);

  map.emplace(0, "foo");
  const auto& str = map[0];
  EXPECT_EQ("foo"sv, str);
}

TEST(VectorMap, BeginAndEndIterators)
{
  vector_map<int, std::string> map;
  map.emplace(1, "abc");
  map.emplace(2, "def");
  map.emplace(3, "ghi");

  EXPECT_EQ(3, std::distance(map.begin(), map.end()));
}

TEST(VectorMap, IndexOf)
{
  vector_map<int, std::string> map;

  map.emplace(4, "foo");
  map.emplace(8, "bar");

  ASSERT_TRUE(map.index_of(4).has_value());
  EXPECT_EQ(0, map.index_of(4));

  ASSERT_TRUE(map.index_of(8).has_value());
  EXPECT_EQ(1, map.index_of(8));
}

TEST(VectorMap, MoveElemForwardAndBack)
{
  vector_map<int, std::string> map;

  ASSERT_NO_THROW(map.move_elem_forward(0));
  ASSERT_NO_THROW(map.move_elem_back(0));

  map.emplace(3, "foo");
  map.emplace(7, "bar");

  ASSERT_EQ(0, map.index_of(3));
  ASSERT_EQ(1, map.index_of(7));
  ASSERT_NO_THROW(map.move_elem_forward(3));  // noop
  ASSERT_NO_THROW(map.move_elem_back(7));     // noop

  map.move_elem_forward(7);
  ASSERT_EQ(1, map.index_of(3));
  ASSERT_EQ(0, map.index_of(7));

  map.move_elem_back(7);
  ASSERT_EQ(0, map.index_of(3));
  ASSERT_EQ(1, map.index_of(7));
}