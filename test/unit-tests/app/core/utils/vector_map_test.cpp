#include "core/utils/vector_map.hpp"

#include <string>  // string, stoi

#include <gtest/gtest.h>

using namespace tactile;

using string_map = vector_map<int, std::string>;

TEST(VectorMap, Defaults)
{
  const string_map map;
  ASSERT_TRUE(map.empty());
  ASSERT_EQ(0u, map.size());
  ASSERT_EQ(0u, map.capacity());
}

TEST(VectorMap, Clear)
{
  string_map map;
  ASSERT_NO_THROW(map.clear());

  map[1] = "a";
  map[2] = "b";
  map[3] = "c";

  ASSERT_FALSE(map.empty());
  ASSERT_EQ(3u, map.size());

  map.clear();

  ASSERT_TRUE(map.empty());
  ASSERT_EQ(0u, map.size());
}

TEST(VectorMap, Reserve)
{
  string_map map;
  ASSERT_EQ(0u, map.capacity());

  map.reserve(5u);
  ASSERT_EQ(5u, map.capacity());
}

TEST(VectorMap, Emplace)
{
  string_map map;

  auto& [key, value] = map.emplace(42, "foo");
  ASSERT_EQ("foo", value);

  ASSERT_TRUE(map.contains(42));
  ASSERT_EQ(1u, map.size());

  ASSERT_THROW(map.emplace(42, "bar"), tactile_error);
}

TEST(VectorMap, EmplaceOrReplace)
{
  string_map map;

  {
    auto& [key, value] = map.emplace_or_replace(38, "abc");
    ASSERT_EQ(38, key);
    ASSERT_EQ("abc", value);
  }

  ASSERT_EQ(1u, map.size());
  ASSERT_TRUE(map.contains(38));
  ASSERT_EQ("abc", map.at(38));

  {
    auto& [key, value] = map.emplace_or_replace(38, "def");
    ASSERT_EQ(38, key);
    ASSERT_EQ("def", value);
  }

  ASSERT_EQ(1u, map.size());
  ASSERT_TRUE(map.contains(38));
  ASSERT_EQ("def", map.at(38));
}

TEST(VectorMap, TryEmplace)
{
  string_map map;

  {
    auto& [key, value] = map.try_emplace(123, "hello");
    ASSERT_EQ("hello", value);
  }

  ASSERT_TRUE(map.contains(123));
  ASSERT_EQ(1u, map.size());

  {
    auto& [key, value] = map.try_emplace(123, "world");
    ASSERT_EQ("world", value);
  }

  ASSERT_TRUE(map.contains(123));
  ASSERT_EQ(1u, map.size());
}

TEST(VectorMap, SubscriptOperator)
{
  string_map map;

  auto& a = map[27];
  ASSERT_EQ("", a);

  ASSERT_TRUE(map.contains(27));
  ASSERT_EQ(1u, map.size());

  auto& b = map[27];
  ASSERT_EQ(&a, &b);

  map[27] = "foo";
  ASSERT_EQ("foo", map.at(27));
}

TEST(VectorMap, Erase)
{
  string_map map;
  ASSERT_NO_THROW(map.erase(42));

  map[10] = "foo";
  map[20] = "bar";

  ASSERT_EQ(2u, map.size());
  ASSERT_TRUE(map.contains(10));
  ASSERT_TRUE(map.contains(20));

  map.erase(10);

  ASSERT_EQ(1u, map.size());
  ASSERT_FALSE(map.empty());
  ASSERT_FALSE(map.contains(10));
  ASSERT_TRUE(map.contains(20));

  map.erase(20);

  ASSERT_EQ(0u, map.size());
  ASSERT_TRUE(map.empty());
  ASSERT_FALSE(map.contains(10));
  ASSERT_FALSE(map.contains(20));
}

TEST(VectorMap, MoveLater)
{
  string_map map;
  map[10] = "A";
  map[20] = "B";
  map[30] = "C";

  ASSERT_EQ(0u, map.index_of(10));
  ASSERT_EQ(1u, map.index_of(20));
  ASSERT_EQ(2u, map.index_of(30));

  map.move_later(20);

  ASSERT_EQ(0u, map.index_of(10));
  ASSERT_EQ(2u, map.index_of(20));
  ASSERT_EQ(1u, map.index_of(30));

  ASSERT_NO_THROW(map.move_later(20));

  ASSERT_EQ(0u, map.index_of(10));
  ASSERT_EQ(2u, map.index_of(20));
  ASSERT_EQ(1u, map.index_of(30));
}

TEST(VectorMap, MoveEarlier)
{
  string_map map;
  map[10] = "A";
  map[20] = "B";
  map[30] = "C";

  ASSERT_EQ(0u, map.index_of(10));
  ASSERT_EQ(1u, map.index_of(20));
  ASSERT_EQ(2u, map.index_of(30));

  map.move_earlier(30);

  ASSERT_EQ(0u, map.index_of(10));
  ASSERT_EQ(2u, map.index_of(20));
  ASSERT_EQ(1u, map.index_of(30));

  map.move_earlier(30);

  ASSERT_EQ(1u, map.index_of(10));
  ASSERT_EQ(2u, map.index_of(20));
  ASSERT_EQ(0u, map.index_of(30));

  ASSERT_NO_THROW(map.move_earlier(30));

  ASSERT_EQ(1u, map.index_of(10));
  ASSERT_EQ(2u, map.index_of(20));
  ASSERT_EQ(0u, map.index_of(30));
}

TEST(VectorMap, Find)
{
  string_map map;
  ASSERT_EQ(map.end(), map.find(12));
  ASSERT_EQ(map.end(), map.find(24));

  map[12] = "hehe";
  map[24] = "hoho";

  ASSERT_NE(map.end(), map.find(12));
  ASSERT_NE(map.end(), map.find(24));
  ASSERT_EQ(map.end(), map.find(18));

  const auto& ref = map;
  ASSERT_NE(map.end(), ref.find(12));
  ASSERT_NE(map.end(), ref.find(24));
  ASSERT_EQ(map.end(), ref.find(18));
}

TEST(VectorMap, At)
{
  string_map map;
  ASSERT_THROW(map.at(0), tactile_error);

  map[0] = "123";

  ASSERT_EQ("123", map.at(0));
  ASSERT_THROW(map.at(1), tactile_error);

  const auto& ref = map;
  ASSERT_EQ("123", ref.at(0));
  ASSERT_THROW(ref.at(1), tactile_error);
}

TEST(VectorMap, AtIndex)
{
  string_map map;
  ASSERT_THROW(map.at_index(0u), tactile_error);

  map[42] = "foo";
  map[84] = "bar";

  ASSERT_EQ("foo", map.at_index(0u).second);
  ASSERT_EQ("bar", map.at_index(1u).second);

  ASSERT_THROW(map.at_index(2u), tactile_error);

  const auto& ref = map;
  ASSERT_EQ("foo", ref.at_index(0u).second);
  ASSERT_EQ("bar", ref.at_index(1u).second);

  ASSERT_THROW(ref.at_index(2u), tactile_error);
}

TEST(VectorMap, Contains)
{
  string_map map;

  ASSERT_FALSE(map.contains(0));
  ASSERT_FALSE(map.contains(1));

  map[0] = "A";

  ASSERT_TRUE(map.contains(0));
  ASSERT_FALSE(map.contains(1));

  map[1] = "B";

  ASSERT_TRUE(map.contains(0));
  ASSERT_TRUE(map.contains(1));

  map.clear();

  ASSERT_FALSE(map.contains(0));
  ASSERT_FALSE(map.contains(1));
}

TEST(VectorMap, IndexOf)
{
  string_map map;

  ASSERT_FALSE(map.index_of(10).has_value());
  ASSERT_FALSE(map.index_of(20).has_value());
  ASSERT_FALSE(map.index_of(30).has_value());

  map[10] = "A";
  map[20] = "B";
  map[30] = "C";

  ASSERT_FALSE(map.index_of(11).has_value());

  ASSERT_EQ(0u, map.index_of(10));
  ASSERT_EQ(1u, map.index_of(20));
  ASSERT_EQ(2u, map.index_of(30));
}

TEST(VectorMap, Iteration)
{
  string_map map;
  map[1] = "1";
  map[2] = "2";
  map[3] = "3";

  int count = 0;
  for (const auto& [key, value] : map) {
    ASSERT_EQ(count + 1, key);
    ASSERT_EQ(key, std::stoi(value));
    ++count;
  }

  ASSERT_EQ(3, count);
}