// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/math/vec.hpp"

#include <type_traits>

#include <gtest/gtest.h>

#include "tactile/core/primitives.hpp"

namespace tactile {
namespace {

static_assert(std::is_nothrow_constructible_v<Vec2F>);
static_assert(std::is_nothrow_copy_constructible_v<Vec2F>);
static_assert(std::is_nothrow_move_constructible_v<Vec2F>);
static_assert(std::is_nothrow_copy_assignable_v<Vec2F>);
static_assert(std::is_nothrow_move_assignable_v<Vec2F>);
static_assert(std::is_nothrow_destructible_v<Vec2F>);

static_assert(std::is_same_v<Vec2F::value_type, float32>);
static_assert(std::is_same_v<Vec2D::value_type, float64>);

class VecTest : public testing::Test
{};

TEST_F(VecTest, Defaults)
{
  constexpr Vec2D vec {};

  EXPECT_EQ(vec.x, 0.0f);
  EXPECT_EQ(vec.y, 0.0f);
}

TEST_F(VecTest, Equality)
{
  constexpr Vec2D vec1 {1.0f, 2.0f};
  constexpr Vec2D vec2 {3.0f, 4.0f};

  EXPECT_EQ(vec1, vec1);
  EXPECT_EQ(vec2, vec2);

  EXPECT_NE(vec1, vec2);
  EXPECT_NE(vec2, vec1);
}

TEST_F(VecTest, Addition)
{
  constexpr Vec2D vec1 {12.0f, 34.0f};
  constexpr Vec2D vec2 {56.0f, 78.0f};

  constexpr auto sum = vec1 + vec2;

  EXPECT_EQ(sum.x, vec1.x + vec2.x);
  EXPECT_EQ(sum.y, vec1.y + vec2.y);
}

TEST_F(VecTest, Subtraction)
{
  constexpr Vec2D vec1 {30.0f, 10.0f};
  constexpr Vec2D vec2 {10.0f, 30.0f};

  constexpr auto diff = vec1 - vec2;

  EXPECT_EQ(diff.x, vec1.x - vec2.x);
  EXPECT_EQ(diff.y, vec1.y - vec2.y);
}

TEST_F(VecTest, Multiplication)
{
  constexpr Vec2D vec1 {10.0f, 4.0f};
  constexpr Vec2D vec2 {3.0f, 25.0f};

  constexpr auto product = vec1 * vec2;

  EXPECT_EQ(product.x, vec1.x * vec2.x);
  EXPECT_EQ(product.y, vec1.y * vec2.y);
}

TEST_F(VecTest, Scaling)
{
  constexpr Vec2D vec {10.0f, 25.0f};
  constexpr Vec2D::value_type scale {4.0f};

  constexpr auto scaled_vec = vec * scale;

  EXPECT_EQ(scaled_vec.x, vec.x * scale);
  EXPECT_EQ(scaled_vec.y, vec.y * scale);
}

}  // namespace
}  // namespace tactile
