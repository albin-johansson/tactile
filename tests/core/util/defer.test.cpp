// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/util/defer.hpp"

#include <gtest/gtest.h>

namespace tactile {
namespace {

class DeferTest : public testing::Test
{};

TEST_F(DeferTest, Defer)
{
  int value = 0;

  {
    const Defer defer_one {[&value] noexcept { value = 1; }};
    value = 3;

    {
      const Defer defer_two {[&value] noexcept { value = 2; }};
      EXPECT_EQ(value, 3);
    }

    EXPECT_EQ(value, 2);
  }

  EXPECT_EQ(value, 1);
}

}  // namespace
}  // namespace tactile
