// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/math/vector.hpp"

#include <sstream>      // stringstream
#include <type_traits>  // is_nothrow_{}_v

#include <gtest/gtest.h>

using namespace tactile;

/// \trace tactile::Vec
TEST(Vec, SpecialMembers)
{
  EXPECT_TRUE(std::is_nothrow_default_constructible_v<Int2>);
  EXPECT_TRUE(std::is_nothrow_default_constructible_v<Int3>);
  EXPECT_TRUE(std::is_nothrow_default_constructible_v<Int4>);
  EXPECT_TRUE(std::is_nothrow_default_constructible_v<Float2>);
  EXPECT_TRUE(std::is_nothrow_default_constructible_v<Float3>);
  EXPECT_TRUE(std::is_nothrow_default_constructible_v<Float4>);

  EXPECT_TRUE(std::is_nothrow_copy_constructible_v<Int2>);
  EXPECT_TRUE(std::is_nothrow_copy_constructible_v<Int3>);
  EXPECT_TRUE(std::is_nothrow_copy_constructible_v<Int4>);
  EXPECT_TRUE(std::is_nothrow_copy_constructible_v<Float2>);
  EXPECT_TRUE(std::is_nothrow_copy_constructible_v<Float3>);
  EXPECT_TRUE(std::is_nothrow_copy_constructible_v<Float4>);

  EXPECT_TRUE(std::is_nothrow_copy_assignable_v<Int2>);
  EXPECT_TRUE(std::is_nothrow_copy_assignable_v<Int3>);
  EXPECT_TRUE(std::is_nothrow_copy_assignable_v<Int4>);
  EXPECT_TRUE(std::is_nothrow_copy_assignable_v<Float2>);
  EXPECT_TRUE(std::is_nothrow_copy_assignable_v<Float3>);
  EXPECT_TRUE(std::is_nothrow_copy_assignable_v<Float4>);

  EXPECT_TRUE(std::is_nothrow_move_constructible_v<Int2>);
  EXPECT_TRUE(std::is_nothrow_move_constructible_v<Int3>);
  EXPECT_TRUE(std::is_nothrow_move_constructible_v<Int4>);
  EXPECT_TRUE(std::is_nothrow_move_constructible_v<Float2>);
  EXPECT_TRUE(std::is_nothrow_move_constructible_v<Float3>);
  EXPECT_TRUE(std::is_nothrow_move_constructible_v<Float4>);

  EXPECT_TRUE(std::is_nothrow_move_assignable_v<Int2>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<Int3>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<Int4>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<Float2>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<Float3>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<Float4>);

  EXPECT_TRUE(std::is_nothrow_destructible_v<Int2>);
  EXPECT_TRUE(std::is_nothrow_destructible_v<Int3>);
  EXPECT_TRUE(std::is_nothrow_destructible_v<Int4>);
  EXPECT_TRUE(std::is_nothrow_destructible_v<Float2>);
  EXPECT_TRUE(std::is_nothrow_destructible_v<Float3>);
  EXPECT_TRUE(std::is_nothrow_destructible_v<Float4>);
}

/// \trace tactile::Vec::Vec
TEST(Vec, DefaultConstructor)
{
  const Int2 ivec;
  EXPECT_EQ(ivec.x(), 0);
  EXPECT_EQ(ivec.y(), 0);

  const Float4 fvec;
  EXPECT_EQ(fvec.x(), 0.0f);
  EXPECT_EQ(fvec.y(), 0.0f);
  EXPECT_EQ(fvec.z(), 0.0f);
  EXPECT_EQ(fvec.w(), 0.0f);
}

/// \trace tactile::Vec::Vec
TEST(Vec, ValueConstructor2D)
{
  const Int2 ivec {1, 2};
  const Float2 fvec {1.0f, 2.0f};

  EXPECT_EQ(ivec.x(), 1);
  EXPECT_EQ(ivec.y(), 2);

  EXPECT_EQ(fvec.x(), 1.0f);
  EXPECT_EQ(fvec.y(), 2.0f);
}

/// \trace tactile::Vec::Vec
TEST(Vec, ValueConstructor3D)
{
  const Int3 ivec {1, 2, 3};
  const Float3 fvec {1.0f, 2.0f, 3.0f};

  EXPECT_EQ(ivec.x(), 1);
  EXPECT_EQ(ivec.y(), 2);
  EXPECT_EQ(ivec.z(), 3);

  EXPECT_EQ(fvec.x(), 1.0f);
  EXPECT_EQ(fvec.y(), 2.0f);
  EXPECT_EQ(fvec.z(), 3.0f);
}

/// \trace tactile::Vec::Vec
TEST(Vec, ValueConstructor4D)
{
  const Int4 ivec {1, 2, 3, 4};
  const Float4 fvec {1.0f, 2.0f, 3.0f, 4.0f};

  EXPECT_EQ(ivec.x(), 1);
  EXPECT_EQ(ivec.y(), 2);
  EXPECT_EQ(ivec.z(), 3);
  EXPECT_EQ(ivec.w(), 4);

  EXPECT_EQ(fvec.x(), 1.0f);
  EXPECT_EQ(fvec.y(), 2.0f);
  EXPECT_EQ(fvec.z(), 3.0f);
  EXPECT_EQ(fvec.w(), 4.0f);
}

/// \trace tactile::Vec::set_x
/// \trace tactile::Vec::x
TEST(Vec, SetX)
{
  Float2 vec {};
  vec.set_x(1.5f);

  EXPECT_EQ(vec.x(), 1.5f);
  EXPECT_EQ(vec.y(), 0.0f);
}

/// \trace tactile::Vec::set_y
/// \trace tactile::Vec::y
TEST(Vec, SetY)
{
  Int2 vec {};
  vec.set_y(42);

  EXPECT_EQ(vec.x(), 0);
  EXPECT_EQ(vec.y(), 42);
}

/// \trace tactile::Vec::set_z
/// \trace tactile::Vec::z
TEST(Vec, SetZ)
{
  Int3 vec {};
  vec.set_z(123);

  EXPECT_EQ(vec.x(), 0);
  EXPECT_EQ(vec.y(), 0);
  EXPECT_EQ(vec.z(), 123);
}

/// \trace tactile::Vec::set_w
/// \trace tactile::Vec::w
TEST(Vec, SetW)
{
  Float4 vec {};
  vec.set_w(25.8f);

  EXPECT_EQ(vec.x(), 0.0f);
  EXPECT_EQ(vec.y(), 0.0f);
  EXPECT_EQ(vec.z(), 0.0f);
  EXPECT_EQ(vec.w(), 25.8f);
}

/// \trace tactile::Vec::at
TEST(Vec, At)
{
  Int4 vec {};
  vec.set_x(10);
  vec.set_y(20);
  vec.set_z(30);
  vec.set_w(40);

  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
  EXPECT_EQ(vec.at(2), 30);
  EXPECT_EQ(vec.at(3), 40);

  EXPECT_THROW((void) vec.at(4), Exception);
  EXPECT_THROW((void) vec.at(vec.size()), Exception);
}

/// \trace tactile::Vec::operator[]
TEST(Vec, SubscriptOperator)
{
  Float4 vec {};
  vec[0] = 1.0f;
  vec[1] = 2.0f;
  vec[2] = 3.0f;
  vec[3] = 4.0f;

  EXPECT_EQ(vec[0], 1.0f);
  EXPECT_EQ(vec[1], 2.0f);
  EXPECT_EQ(vec[2], 3.0f);
  EXPECT_EQ(vec[3], 4.0f);

  const auto& const_vec = vec;
  EXPECT_EQ(const_vec[0], 1.0f);
  EXPECT_EQ(const_vec[1], 2.0f);
  EXPECT_EQ(const_vec[2], 3.0f);
  EXPECT_EQ(const_vec[3], 4.0f);
}

/// \trace tactile::Vec::data
TEST(Vec, Data)
{
  Int3 vec {};
  vec[0] = 12;
  vec[1] = 34;
  vec[2] = 56;

  const auto& const_vec = vec;

  auto* data = vec.data();
  ASSERT_NE(data, nullptr);
  EXPECT_EQ(data[0], 12);
  EXPECT_EQ(data[1], 34);
  EXPECT_EQ(data[2], 56);

  const auto* const_data = const_vec.data();
  ASSERT_NE(const_data, nullptr);
  EXPECT_EQ(const_data[0], 12);
  EXPECT_EQ(const_data[1], 34);
  EXPECT_EQ(const_data[2], 56);
}

/// \trace tactile::Vec::size
TEST(Vec, Size)
{
  EXPECT_EQ(Int2::size(), 2);
  EXPECT_EQ(Int3::size(), 3);
  EXPECT_EQ(Int4::size(), 4);

  EXPECT_EQ(Float2::size(), 2);
  EXPECT_EQ(Float3::size(), 3);
  EXPECT_EQ(Float4::size(), 4);
}

/// \trace tactile::Vec::operator-
TEST(Vec, UnaryMinus)
{
  const Int3 vec {-1, 2, 3};
  const auto negated_vec = -vec;

  EXPECT_EQ(negated_vec.x(), 1);
  EXPECT_EQ(negated_vec.y(), -2);
  EXPECT_EQ(negated_vec.z(), -3);
}

/// \trace tactile::operator== [Vec]
TEST(Vec, EqualityOperator)
{
  const Int3 a {-83, 934, -193};
  const Int3 b {64, 8, 152};

  EXPECT_EQ(a, a);
  EXPECT_EQ(b, b);

  EXPECT_FALSE(a == b);
  EXPECT_FALSE(b == a);
}

/// \trace tactile::operator!= [Vec]
TEST(Vec, InequalityOperator)
{
  const Float2 a {1.0f, 2.0f};
  const Float2 b {3.0f, 4.0f};

  EXPECT_NE(a, b);
  EXPECT_NE(b, a);

  EXPECT_FALSE(a != a);
  EXPECT_FALSE(b != b);
}

/// \trace tactile::operator<(const Vec&, const Vec&)
TEST(Vec, LessThanOperator)
{
  const Float2 a {1.0f, 0.0f};
  const Float2 b {2.0f, 0.0f};
  const Float2 c {0.0f, 1.0f};
  const Float2 d {0.0f, 2.0f};

  EXPECT_LT(a, b);

  EXPECT_LT(c, a);
  EXPECT_LT(c, b);
  EXPECT_LT(c, d);

  EXPECT_LT(d, a);
  EXPECT_LT(d, b);
}

/// \trace tactile::operator<= [Vec]
TEST(Vec, LessThanEqOperator)
{
  const Int2 a {1, 0};
  const Int2 b {2, 0};
  const Int2 c {0, 1};
  const Int2 d {0, 2};

  EXPECT_LE(a, a);
  EXPECT_LE(b, b);
  EXPECT_LE(c, c);
  EXPECT_LE(d, d);

  EXPECT_LE(a, b);

  EXPECT_LE(c, a);
  EXPECT_LE(c, b);
  EXPECT_LE(c, d);

  EXPECT_LE(d, a);
  EXPECT_LE(d, b);
}

/// \trace tactile::operator> [Vec]
TEST(Vec, GreaterThanOperator)
{
  const Float2 a {1.0f, 0.0f};
  const Float2 b {2.0f, 0.0f};
  const Float2 c {0.0f, 1.0f};
  const Float2 d {0.0f, 2.0f};

  EXPECT_GT(a, c);
  EXPECT_GT(a, d);

  EXPECT_GT(b, a);
  EXPECT_GT(b, c);
  EXPECT_GT(b, d);

  EXPECT_GT(d, c);
}

/// \trace tactile::operator>= [Vec]
TEST(Vec, GreaterThanEqOperator)
{
  const Int2 a {1, 0};
  const Int2 b {2, 0};
  const Int2 c {0, 1};
  const Int2 d {0, 2};

  EXPECT_GE(a, a);
  EXPECT_GE(b, b);
  EXPECT_GE(c, c);
  EXPECT_GE(d, d);

  EXPECT_GE(a, c);
  EXPECT_GE(a, d);

  EXPECT_GE(b, a);
  EXPECT_GE(b, c);
  EXPECT_GE(b, d);

  EXPECT_GE(d, c);
}

/// \trace tactile::vector_cast
TEST(Vec, VectorCast)
{
  const Int4 ivec {10, 20, 30, 40};
  const auto fvec = vector_cast<float>(ivec);

  EXPECT_EQ(fvec.x(), static_cast<float>(ivec.x()));
  EXPECT_EQ(fvec.y(), static_cast<float>(ivec.y()));
  EXPECT_EQ(fvec.z(), static_cast<float>(ivec.z()));
  EXPECT_EQ(fvec.w(), static_cast<float>(ivec.w()));
}

/// \trace tactile::min [Vec]
TEST(Vec, Min)
{
  const Int4 a {-1, +2, -3, +4};
  const Int4 b {+1, -2, +3, -4};
  const auto ab_min = min(a, b);

  EXPECT_EQ(ab_min.x(), -1);
  EXPECT_EQ(ab_min.y(), -2);
  EXPECT_EQ(ab_min.z(), -3);
  EXPECT_EQ(ab_min.w(), -4);
}

/// \trace tactile::max [Vec]
TEST(Vec, Max)
{
  const Int4 a {-1, +2, -3, +4};
  const Int4 b {+1, -2, +3, -4};
  const auto ab_max = max(a, b);

  EXPECT_EQ(ab_max.x(), 1);
  EXPECT_EQ(ab_max.y(), 2);
  EXPECT_EQ(ab_max.z(), 3);
  EXPECT_EQ(ab_max.w(), 4);
}

/// \trace tactile::operator<< [Vec]
TEST(Vec, StreamOperator)
{
  const Int4 vec {1, 2, 3, 4};

  std::stringstream stream {};
  stream << vec;

  EXPECT_EQ(stream.str(), "(1;2;3;4)");
}

/// \trace tactile::compare_elements
TEST(Vec, CompareElements)
{
  {
    const Int4 vec {1, 2, 3, 4};
    const auto result = compare_elements(vec, vec);
    EXPECT_TRUE(result.same_x);
    EXPECT_TRUE(result.same_y);
    EXPECT_TRUE(result.same_z);
    EXPECT_TRUE(result.same_w);
  }

  {
    const Int4 a {1, 2, 3, 4};
    const Int4 b {1, 5, 6, 7};
    const auto result = compare_elements(a, b);
    EXPECT_TRUE(result.same_x);
    EXPECT_FALSE(result.same_y);
    EXPECT_FALSE(result.same_z);
    EXPECT_FALSE(result.same_w);
  }

  {
    const Int4 a {2, 1, 4, 6};
    const Int4 b {3, 1, 5, 7};
    const auto result = compare_elements(a, b);
    EXPECT_FALSE(result.same_x);
    EXPECT_TRUE(result.same_y);
    EXPECT_FALSE(result.same_z);
    EXPECT_FALSE(result.same_w);
  }

  {
    const Int4 a {2, 4, 1, 6};
    const Int4 b {3, 5, 1, 7};
    const auto result = compare_elements(a, b);
    EXPECT_FALSE(result.same_x);
    EXPECT_FALSE(result.same_y);
    EXPECT_TRUE(result.same_z);
    EXPECT_FALSE(result.same_w);
  }

  {
    const Int4 a {2, 4, 6, 1};
    const Int4 b {3, 5, 7, 1};
    const auto result = compare_elements(a, b);
    EXPECT_FALSE(result.same_x);
    EXPECT_FALSE(result.same_y);
    EXPECT_FALSE(result.same_z);
    EXPECT_TRUE(result.same_w);
  }

  {
    const Int4 a {1, 2, 3, 4};
    const Int4 b {5, 6, 7, 8};
    const auto result = compare_elements(a, b);
    EXPECT_FALSE(result.same_x);
    EXPECT_FALSE(result.same_y);
    EXPECT_FALSE(result.same_z);
    EXPECT_FALSE(result.same_w);
  }
}