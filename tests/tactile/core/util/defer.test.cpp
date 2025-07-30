// Copyright (C) 2025 Albin Johansson

#include <gtest/gtest.h>

import tactile.core.util;

namespace tactile::tests {
namespace {

TEST(Defer, Defer)
{
  int value = 0;

  {
    const Defer defer_one {[&value] { value = 1; }};

    {
      const Defer defer_two {[&value] { value = 2; }};
      value = 3;
    }

    EXPECT_EQ(value, 2);
  }

  EXPECT_EQ(value, 1);
}

}  // namespace
}  // namespace tactile::tests
