#include "core/utils/vector_map.hpp"

#include <string>  // string, stoi

#include <gtest/gtest.h>

using namespace tactile;

using Map = VectorMap<int, std::string>;

TEST(VectorMap, Defaults)
{
  const Map map;
  ASSERT_TRUE(map.IsEmpty());
  ASSERT_EQ(0u, map.GetSize());
  ASSERT_EQ(0u, map.GetCapacity());
}

TEST(VectorMap, Clear)
{
  Map map;
  ASSERT_NO_THROW(map.Clear());

  map[1] = "a";
  map[2] = "b";
  map[3] = "c";

  ASSERT_FALSE(map.IsEmpty());
  ASSERT_EQ(3u, map.GetSize());

  map.Clear();

  ASSERT_TRUE(map.IsEmpty());
  ASSERT_EQ(0u, map.GetSize());
}

TEST(VectorMap, Reserve)
{
  Map map;
  ASSERT_EQ(0u, map.GetCapacity());

  map.Reserve(5u);
  ASSERT_EQ(5u, map.GetCapacity());
}

TEST(VectorMap, Emplace)
{
  Map map;

  auto& [key, value] = map.Emplace(42, "foo");
  ASSERT_EQ("foo", value);

  ASSERT_TRUE(map.Contains(42));
  ASSERT_EQ(1u, map.GetSize());

  ASSERT_THROW(map.Emplace(42, "bar"), TactileError);
}

TEST(VectorMap, EmplaceOrReplace)
{
  Map map;

  {
    auto& [key, value] = map.EmplaceOrReplace(38, "abc");
    ASSERT_EQ(38, key);
    ASSERT_EQ("abc", value);
  }

  ASSERT_EQ(1u, map.GetSize());
  ASSERT_TRUE(map.Contains(38));
  ASSERT_EQ("abc", map.At(38));

  {
    auto& [key, value] = map.EmplaceOrReplace(38, "def");
    ASSERT_EQ(38, key);
    ASSERT_EQ("def", value);
  }

  ASSERT_EQ(1u, map.GetSize());
  ASSERT_TRUE(map.Contains(38));
  ASSERT_EQ("def", map.At(38));
}

TEST(VectorMap, TryEmplace)
{
  Map map;

  {
    auto& [key, value] = map.TryEmplace(123, "hello");
    ASSERT_EQ("hello", value);
  }

  ASSERT_TRUE(map.Contains(123));
  ASSERT_EQ(1u, map.GetSize());

  {
    auto& [key, value] = map.TryEmplace(123, "world");
    ASSERT_EQ("world", value);
  }

  ASSERT_TRUE(map.Contains(123));
  ASSERT_EQ(1u, map.GetSize());
}

TEST(VectorMap, SubscriptOperator)
{
  Map map;

  auto& a = map[27];
  ASSERT_EQ("", a);

  ASSERT_TRUE(map.Contains(27));
  ASSERT_EQ(1u, map.GetSize());

  auto& b = map[27];
  ASSERT_EQ(&a, &b);

  map[27] = "foo";
  ASSERT_EQ("foo", map.At(27));
}

TEST(VectorMap, Erase)
{
  Map map;
  ASSERT_NO_THROW(map.Erase(42));

  map[10] = "foo";
  map[20] = "bar";

  ASSERT_EQ(2u, map.GetSize());
  ASSERT_TRUE(map.Contains(10));
  ASSERT_TRUE(map.Contains(20));

  map.Erase(10);

  ASSERT_EQ(1u, map.GetSize());
  ASSERT_FALSE(map.IsEmpty());
  ASSERT_FALSE(map.Contains(10));
  ASSERT_TRUE(map.Contains(20));

  map.Erase(20);

  ASSERT_EQ(0u, map.GetSize());
  ASSERT_TRUE(map.IsEmpty());
  ASSERT_FALSE(map.Contains(10));
  ASSERT_FALSE(map.Contains(20));
}

TEST(VectorMap, MoveLater)
{
  Map map;
  map[10] = "A";
  map[20] = "B";
  map[30] = "C";

  ASSERT_EQ(0u, map.IndexOf(10));
  ASSERT_EQ(1u, map.IndexOf(20));
  ASSERT_EQ(2u, map.IndexOf(30));

  map.MoveLater(20);

  ASSERT_EQ(0u, map.IndexOf(10));
  ASSERT_EQ(2u, map.IndexOf(20));
  ASSERT_EQ(1u, map.IndexOf(30));

  ASSERT_NO_THROW(map.MoveLater(20));

  ASSERT_EQ(0u, map.IndexOf(10));
  ASSERT_EQ(2u, map.IndexOf(20));
  ASSERT_EQ(1u, map.IndexOf(30));
}

TEST(VectorMap, MoveEarlier)
{
  Map map;
  map[10] = "A";
  map[20] = "B";
  map[30] = "C";

  ASSERT_EQ(0u, map.IndexOf(10));
  ASSERT_EQ(1u, map.IndexOf(20));
  ASSERT_EQ(2u, map.IndexOf(30));

  map.MoveEarlier(30);

  ASSERT_EQ(0u, map.IndexOf(10));
  ASSERT_EQ(2u, map.IndexOf(20));
  ASSERT_EQ(1u, map.IndexOf(30));

  map.MoveEarlier(30);

  ASSERT_EQ(1u, map.IndexOf(10));
  ASSERT_EQ(2u, map.IndexOf(20));
  ASSERT_EQ(0u, map.IndexOf(30));

  ASSERT_NO_THROW(map.MoveEarlier(30));

  ASSERT_EQ(1u, map.IndexOf(10));
  ASSERT_EQ(2u, map.IndexOf(20));
  ASSERT_EQ(0u, map.IndexOf(30));
}

TEST(VectorMap, Find)
{
  Map map;
  ASSERT_EQ(map.end(), map.Find(12));
  ASSERT_EQ(map.end(), map.Find(24));

  map[12] = "hehe";
  map[24] = "hoho";

  ASSERT_NE(map.end(), map.Find(12));
  ASSERT_NE(map.end(), map.Find(24));
  ASSERT_EQ(map.end(), map.Find(18));

  const auto& ref = map;
  ASSERT_NE(map.end(), ref.Find(12));
  ASSERT_NE(map.end(), ref.Find(24));
  ASSERT_EQ(map.end(), ref.Find(18));
}

TEST(VectorMap, At)
{
  Map map;
  ASSERT_THROW(map.At(0), TactileError);

  map[0] = "123";

  ASSERT_EQ("123", map.At(0));
  ASSERT_THROW(map.At(1), TactileError);

  const auto& ref = map;
  ASSERT_EQ("123", ref.At(0));
  ASSERT_THROW(ref.At(1), TactileError);
}

TEST(VectorMap, AtIndex)
{
  Map map;
  ASSERT_THROW(map.AtIndex(0u), TactileError);

  map[42] = "foo";
  map[84] = "bar";

  ASSERT_EQ("foo", map.AtIndex(0u).second);
  ASSERT_EQ("bar", map.AtIndex(1u).second);

  ASSERT_THROW(map.AtIndex(2u), TactileError);

  const auto& ref = map;
  ASSERT_EQ("foo", ref.AtIndex(0u).second);
  ASSERT_EQ("bar", ref.AtIndex(1u).second);

  ASSERT_THROW(ref.AtIndex(2u), TactileError);
}

TEST(VectorMap, Contains)
{
  Map map;

  ASSERT_FALSE(map.Contains(0));
  ASSERT_FALSE(map.Contains(1));

  map[0] = "A";

  ASSERT_TRUE(map.Contains(0));
  ASSERT_FALSE(map.Contains(1));

  map[1] = "B";

  ASSERT_TRUE(map.Contains(0));
  ASSERT_TRUE(map.Contains(1));

  map.Clear();

  ASSERT_FALSE(map.Contains(0));
  ASSERT_FALSE(map.Contains(1));
}

TEST(VectorMap, IndexOf)
{
  Map map;

  ASSERT_FALSE(map.IndexOf(10).has_value());
  ASSERT_FALSE(map.IndexOf(20).has_value());
  ASSERT_FALSE(map.IndexOf(30).has_value());

  map[10] = "A";
  map[20] = "B";
  map[30] = "C";

  ASSERT_FALSE(map.IndexOf(11).has_value());

  ASSERT_EQ(0u, map.IndexOf(10));
  ASSERT_EQ(1u, map.IndexOf(20));
  ASSERT_EQ(2u, map.IndexOf(30));
}

TEST(VectorMap, Iteration)
{
  Map map;
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