#include "core/utils/from_string.hpp"

#include <gtest/gtest.h>

using namespace Tactile;

TEST(FromString, FloatingPoint)
{
  ASSERT_FALSE(FromString<float>("foo"));

  ASSERT_EQ(12.3f, FromString<float>("12.3"));
  ASSERT_EQ(42.0f, FromString<float>("42"));

  ASSERT_EQ(78.5, FromString<double>("78.5"));
  ASSERT_EQ(98.0, FromString<double>("98"));
}

TEST(FromString, Integral)
{
  ASSERT_FALSE(FromString<int>("ABC"));

  ASSERT_EQ(123, FromString<int>("123"));
  ASSERT_EQ(-42, FromString<int>("-42"));
}
