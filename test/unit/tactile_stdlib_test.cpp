#include <tactile_stdlib.hpp>

#include <gtest/gtest.h>

using namespace Tactile;

TEST(TactileStdLib, IntegerFromString)
{
  ASSERT_FALSE(FromString<int>("ABC"));

  ASSERT_EQ(-42, FromString<int>("-42"));
  ASSERT_EQ(123, FromString<int16>("123"));
  ASSERT_EQ(745, FromString<int64>("745"));

  ASSERT_FALSE(FromString<int>("F1"));
  ASSERT_EQ(0xF1, FromString<int>("F1", 16));

  ASSERT_EQ(4, FromString<int>("4298", 1u, 10));
  ASSERT_EQ(0x12, FromString<int>("12AC", 2u, 16));

  // Robustness against invalid length
  ASSERT_EQ(1234, FromString<int>("1234"));
  ASSERT_EQ(1234, FromString<int>("1234", 4u, 10));
  ASSERT_EQ(1234, FromString<int>("1234", 5u, 10));
  ASSERT_EQ(1234, FromString<int>("1234", 6u, 10));
}

TEST(TactileStdLib, FloatFromString)
{
  ASSERT_FALSE(FromString<float>("foo"));

  ASSERT_EQ(12.3f, FromString<float>("12.3"));
  ASSERT_EQ(42.0f, FromString<float>("42"));

  ASSERT_EQ(78.5, FromString<double>("78.5"));
  ASSERT_EQ(98.0, FromString<double>("98"));
}