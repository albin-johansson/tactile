// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include <gmock/gmock.h>
#include <gtest/gtest.h>

import std;
import tactile.numeric;

namespace tactile::tests {
namespace {

using testing::Const;

static_assert(Int2::size() == 2);
static_assert(Int3::size() == 3);
static_assert(Int4::size() == 4);

static_assert(Float2::size() == 2);
static_assert(Float3::size() == 3);
static_assert(Float4::size() == 4);

class VecTest : public testing::Test
{};

TEST_F(VecTest, Defaults)
{
  constexpr Int4 vec {};
  EXPECT_EQ(vec.x(), 0);
  EXPECT_EQ(vec.y(), 0);
  EXPECT_EQ(vec.z(), 0);
  EXPECT_EQ(vec.w(), 0);
  EXPECT_NE(vec.data(), nullptr);
  EXPECT_NE(Const(vec).data(), nullptr);
}

TEST_F(VecTest, Setters)
{
  Int4 vec {};

  vec.set_x(10);
  vec.set_y(20);
  vec.set_z(30);
  vec.set_w(40);

  EXPECT_EQ(vec.x(), 10);
  EXPECT_EQ(vec.y(), 20);
  EXPECT_EQ(vec.z(), 30);
  EXPECT_EQ(vec.w(), 40);

  EXPECT_EQ(vec[0uz], 10);
  EXPECT_EQ(vec[1uz], 20);
  EXPECT_EQ(vec[2uz], 30);
  EXPECT_EQ(vec[3uz], 40);
}

TEST_F(VecTest, At)
{
  constexpr Float4 vec {1.0f, 2.0f, 3.0f, 4.0f};

  EXPECT_EQ(vec.at(0uz), 1.0f);
  EXPECT_EQ(vec.at(1uz), 2.0f);
  EXPECT_EQ(vec.at(2uz), 3.0f);
  EXPECT_EQ(vec.at(3uz), 4.0f);

  EXPECT_THROW((void) vec.at(4uz), std::out_of_range);
}

TEST_F(VecTest, Data)
{
  Int3 vec {1, 2, 3};

  EXPECT_EQ(vec.data()[0uz], 1);
  EXPECT_EQ(vec.data()[1uz], 2);
  EXPECT_EQ(vec.data()[2uz], 3);

  EXPECT_EQ(Const(vec).data()[0uz], 1);
  EXPECT_EQ(Const(vec).data()[1uz], 2);
  EXPECT_EQ(Const(vec).data()[2uz], 3);
}

TEST_F(VecTest, Add)
{
  constexpr auto vec = Int3 {1, 2, 3} + Int3 {4, 5, 6};
  EXPECT_EQ(vec.x(), 5);
  EXPECT_EQ(vec.y(), 7);
  EXPECT_EQ(vec.z(), 9);
}

TEST_F(VecTest, AddAssign)
{
  Int3 vec {1, 2, 3};

  vec += Int3 {10, 20, 30};

  EXPECT_EQ(vec.x(), 11);
  EXPECT_EQ(vec.y(), 22);
  EXPECT_EQ(vec.z(), 33);
}

TEST_F(VecTest, Sub)
{
  constexpr auto vec = Int2 {9, 8} - Int2 {1, 2};
  EXPECT_EQ(vec.x(), 8);
  EXPECT_EQ(vec.y(), 6);
}

TEST_F(VecTest, SubAssign)
{
  Int3 vec {10, 11, 12};

  vec -= Int3 {0, -5, 10};

  EXPECT_EQ(vec.x(), 10);
  EXPECT_EQ(vec.y(), 16);
  EXPECT_EQ(vec.z(), 2);
}

TEST_F(VecTest, Mul)
{
  constexpr auto vec = Int4 {1, -2, 3, 4} * Int4 {10, 20, 30, -40};
  EXPECT_EQ(vec.x(), 10);
  EXPECT_EQ(vec.y(), -40);
  EXPECT_EQ(vec.z(), 90);
  EXPECT_EQ(vec.w(), -160);
}

TEST_F(VecTest, MulAssign)
{
  Int4 vec {1, 2, 3, 4};

  vec *= Int4 {4, 0, 1, 2};

  EXPECT_EQ(vec.x(), 4);
  EXPECT_EQ(vec.y(), 0);
  EXPECT_EQ(vec.z(), 3);
  EXPECT_EQ(vec.w(), 8);
}

TEST_F(VecTest, MulWithScalar)
{
  constexpr auto vec = Int4 {1, 2, 3, 4} * 2;
  EXPECT_EQ(vec.x(), 2);
  EXPECT_EQ(vec.y(), 4);
  EXPECT_EQ(vec.z(), 6);
  EXPECT_EQ(vec.w(), 8);
}

TEST_F(VecTest, MulAssignWithScalar)
{
  Int4 vec {1, 2, 3, 4};

  vec *= -2;

  EXPECT_EQ(vec.x(), -2);
  EXPECT_EQ(vec.y(), -4);
  EXPECT_EQ(vec.z(), -6);
  EXPECT_EQ(vec.w(), -8);
}

TEST_F(VecTest, Eq)
{
  constexpr Int3 a {1, 2, 3};
  constexpr Int3 b {1, 2, 4};

  EXPECT_EQ(a, a);
  EXPECT_NE(a, b);
}

TEST_F(VecTest, Ord)
{
  constexpr Int3 a {1, 2, 3};
  constexpr Int3 b {1, 2, 4};

  EXPECT_FALSE(a < a);

  EXPECT_LT(a, b);
  EXPECT_GT(b, a);
}

}  // namespace
}  // namespace tactile::tests
