// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/math/vector.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(MathVector, CompareVectors)
{
  {
    const Int4 vec {1, 2, 3, 4};
    const auto result = compare_vectors(vec, vec);
    EXPECT_TRUE(result.same_x);
    EXPECT_TRUE(result.same_y);
    EXPECT_TRUE(result.same_z);
    EXPECT_TRUE(result.same_w);
  }

  {
    const Int4 a {1, 2, 3, 4};
    const Int4 b {1, 5, 6, 7};
    const auto result = compare_vectors(a, b);
    EXPECT_TRUE(result.same_x);
    EXPECT_FALSE(result.same_y);
    EXPECT_FALSE(result.same_z);
    EXPECT_FALSE(result.same_w);
  }

  {
    const Int4 a {2, 1, 4, 6};
    const Int4 b {3, 1, 5, 7};
    const auto result = compare_vectors(a, b);
    EXPECT_FALSE(result.same_x);
    EXPECT_TRUE(result.same_y);
    EXPECT_FALSE(result.same_z);
    EXPECT_FALSE(result.same_w);
  }

  {
    const Int4 a {2, 4, 1, 6};
    const Int4 b {3, 5, 1, 7};
    const auto result = compare_vectors(a, b);
    EXPECT_FALSE(result.same_x);
    EXPECT_FALSE(result.same_y);
    EXPECT_TRUE(result.same_z);
    EXPECT_FALSE(result.same_w);
  }

  {
    const Int4 a {2, 4, 6, 1};
    const Int4 b {3, 5, 7, 1};
    const auto result = compare_vectors(a, b);
    EXPECT_FALSE(result.same_x);
    EXPECT_FALSE(result.same_y);
    EXPECT_FALSE(result.same_z);
    EXPECT_TRUE(result.same_w);
  }

  {
    const Int4 a {1, 2, 3, 4};
    const Int4 b {5, 6, 7, 8};
    const auto result = compare_vectors(a, b);
    EXPECT_FALSE(result.same_x);
    EXPECT_FALSE(result.same_y);
    EXPECT_FALSE(result.same_z);
    EXPECT_FALSE(result.same_w);
  }
}