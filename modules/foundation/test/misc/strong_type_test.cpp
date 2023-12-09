// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/strong_type.hpp"

#include <gtest/gtest.h>

#include "tactile/foundation/container/string.hpp"

namespace {

TACTILE_STRONG_TYPE(StrongInt, int);
TACTILE_STRONG_TYPE(StrongStr, tactile::StringView);

}  // namespace

TEST(StrongType, DefaultValue)
{
  const StrongInt integer {};
  const StrongStr string {};

  EXPECT_EQ(integer.value, 0);
  EXPECT_EQ(string.value, "");
}

TEST(StrongType, EqualityComparison)
{
  const StrongStr s1 {"foo"};
  const StrongStr s2 {"bar"};

  EXPECT_EQ(s1, s1);
  EXPECT_EQ(s2, s2);
  EXPECT_NE(s1, s2);
}

TEST(StrongType, SpaceshipComparison)
{
  const StrongInt i1 {-413};
  const StrongInt i2 {382};

  EXPECT_EQ(i1 <=> i1, std::strong_ordering::equal);
  EXPECT_EQ(i1 <=> i2, std::strong_ordering::less);
  EXPECT_EQ(i2 <=> i1, std::strong_ordering::greater);
}
