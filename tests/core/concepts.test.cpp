// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/concepts.hpp"

#include <gtest/gtest.h>

#include "tactile/core/primitives.hpp"

namespace tactile {
namespace {

class ConceptsTest : public testing::Test
{};

TEST_F(ConceptsTest, CvUnqualified)
{
  EXPECT_TRUE(CvUnqualified<int>);
  EXPECT_TRUE(CvUnqualified<int&>);
  EXPECT_TRUE(CvUnqualified<const int&>);
  EXPECT_TRUE(CvUnqualified<volatile int&>);
  EXPECT_TRUE(CvUnqualified<const int*>);
  EXPECT_TRUE(CvUnqualified<volatile int*>);

  EXPECT_FALSE(CvUnqualified<const int>);
  EXPECT_FALSE(CvUnqualified<volatile int>);
  EXPECT_FALSE(CvUnqualified<int* const>);
  EXPECT_FALSE(CvUnqualified<int* volatile>);
}

TEST_F(ConceptsTest, SignedInteger)
{
  EXPECT_FALSE(SignedInteger<bool>);
  EXPECT_FALSE(SignedInteger<char>);
  EXPECT_FALSE(SignedInteger<float>);

  EXPECT_TRUE(SignedInteger<signed char>);
  EXPECT_TRUE(SignedInteger<signed short>);
  EXPECT_TRUE(SignedInteger<signed int>);
  EXPECT_TRUE(SignedInteger<signed long>);
  EXPECT_TRUE(SignedInteger<signed long long>);
}

TEST_F(ConceptsTest, UnsignedInteger)
{
  EXPECT_FALSE(UnsignedInteger<bool>);
  EXPECT_FALSE(UnsignedInteger<char>);
  EXPECT_FALSE(UnsignedInteger<float>);

  EXPECT_TRUE(UnsignedInteger<unsigned char>);
  EXPECT_TRUE(UnsignedInteger<unsigned short>);
  EXPECT_TRUE(UnsignedInteger<unsigned int>);
  EXPECT_TRUE(UnsignedInteger<unsigned long>);
  EXPECT_TRUE(UnsignedInteger<unsigned long long>);
}

TEST_F(ConceptsTest, Integer)
{
  EXPECT_FALSE(Integer<bool>);
  EXPECT_FALSE(Integer<char>);
  EXPECT_FALSE(Integer<float>);
  EXPECT_FALSE(Integer<const int>);
  EXPECT_FALSE(Integer<volatile int>);

  EXPECT_TRUE(Integer<signed char>);
  EXPECT_TRUE(Integer<signed short>);
  EXPECT_TRUE(Integer<signed int>);
  EXPECT_TRUE(Integer<signed long>);
  EXPECT_TRUE(Integer<signed long long>);

  EXPECT_TRUE(Integer<unsigned char>);
  EXPECT_TRUE(Integer<unsigned short>);
  EXPECT_TRUE(Integer<unsigned int>);
  EXPECT_TRUE(Integer<unsigned long>);
  EXPECT_TRUE(Integer<unsigned long long>);
}

TEST_F(ConceptsTest, FloatingPoint)
{
  EXPECT_FALSE(FloatingPoint<bool>);
  EXPECT_FALSE(FloatingPoint<char>);
  EXPECT_FALSE(FloatingPoint<int>);
  EXPECT_FALSE(FloatingPoint<const float>);
  EXPECT_FALSE(FloatingPoint<volatile float>);

  EXPECT_TRUE(FloatingPoint<float>);
  EXPECT_TRUE(FloatingPoint<double>);
  EXPECT_TRUE(FloatingPoint<long double>);
}

TEST_F(ConceptsTest, Number)
{
  EXPECT_FALSE(Number<bool>);
  EXPECT_FALSE(Number<char>);

  EXPECT_TRUE(Number<float>);
  EXPECT_TRUE(Number<double>);

  EXPECT_TRUE(Number<signed char>);
  EXPECT_TRUE(Number<signed short>);
  EXPECT_TRUE(Number<signed int>);
  EXPECT_TRUE(Number<signed long>);
  EXPECT_TRUE(Number<signed long long>);

  EXPECT_TRUE(Number<unsigned char>);
  EXPECT_TRUE(Number<unsigned short>);
  EXPECT_TRUE(Number<unsigned int>);
  EXPECT_TRUE(Number<unsigned long>);
  EXPECT_TRUE(Number<unsigned long long>);
}

TEST_F(ConceptsTest, NothrowInvocable)
{
  EXPECT_EQ(NothrowInvocable<decltype([] {})>, false);
  EXPECT_EQ(NothrowInvocable<decltype([] noexcept {})>, true);
}

TEST_F(ConceptsTest, TriviallyConvertible)
{
  // Assumptions for float types
  ASSERT_EQ(std::numeric_limits<float32>::digits, 24);
  ASSERT_EQ(std::numeric_limits<float64>::digits, 53);

  EXPECT_EQ((TriviallyConvertible<uint8, char>), false);
  EXPECT_EQ((TriviallyConvertible<uint8, bool>), false);
  EXPECT_EQ((TriviallyConvertible<int32, float>), false);

  // from int8
  EXPECT_EQ((TriviallyConvertible<int8, int8>), true);
  EXPECT_EQ((TriviallyConvertible<int8, int16>), true);
  EXPECT_EQ((TriviallyConvertible<int8, int32>), true);
  EXPECT_EQ((TriviallyConvertible<int8, int64>), true);
  EXPECT_EQ((TriviallyConvertible<int8, uint8>), false);
  EXPECT_EQ((TriviallyConvertible<int8, uint16>), false);
  EXPECT_EQ((TriviallyConvertible<int8, uint32>), false);
  EXPECT_EQ((TriviallyConvertible<int8, uint64>), false);
  EXPECT_EQ((TriviallyConvertible<int8, float32>), true);
  EXPECT_EQ((TriviallyConvertible<int8, float64>), true);

  // from int16
  EXPECT_EQ((TriviallyConvertible<int16, int8>), false);
  EXPECT_EQ((TriviallyConvertible<int16, int16>), true);
  EXPECT_EQ((TriviallyConvertible<int16, int32>), true);
  EXPECT_EQ((TriviallyConvertible<int16, int64>), true);
  EXPECT_EQ((TriviallyConvertible<int16, uint8>), false);
  EXPECT_EQ((TriviallyConvertible<int16, uint16>), false);
  EXPECT_EQ((TriviallyConvertible<int16, uint32>), false);
  EXPECT_EQ((TriviallyConvertible<int16, uint64>), false);
  EXPECT_EQ((TriviallyConvertible<int16, float32>), true);
  EXPECT_EQ((TriviallyConvertible<int16, float64>), true);

  // from int32
  EXPECT_EQ((TriviallyConvertible<int32, int8>), false);
  EXPECT_EQ((TriviallyConvertible<int32, int16>), false);
  EXPECT_EQ((TriviallyConvertible<int32, int32>), true);
  EXPECT_EQ((TriviallyConvertible<int32, int64>), true);
  EXPECT_EQ((TriviallyConvertible<int32, uint8>), false);
  EXPECT_EQ((TriviallyConvertible<int32, uint16>), false);
  EXPECT_EQ((TriviallyConvertible<int32, uint32>), false);
  EXPECT_EQ((TriviallyConvertible<int32, uint64>), false);
  EXPECT_EQ((TriviallyConvertible<int32, float32>), false);
  EXPECT_EQ((TriviallyConvertible<int32, float64>), true);

  // from int64
  EXPECT_EQ((TriviallyConvertible<int64, int8>), false);
  EXPECT_EQ((TriviallyConvertible<int64, int16>), false);
  EXPECT_EQ((TriviallyConvertible<int64, int32>), false);
  EXPECT_EQ((TriviallyConvertible<int64, int64>), true);
  EXPECT_EQ((TriviallyConvertible<int64, uint8>), false);
  EXPECT_EQ((TriviallyConvertible<int64, uint16>), false);
  EXPECT_EQ((TriviallyConvertible<int64, uint32>), false);
  EXPECT_EQ((TriviallyConvertible<int64, uint64>), false);
  EXPECT_EQ((TriviallyConvertible<int64, float32>), false);
  EXPECT_EQ((TriviallyConvertible<int64, float64>), false);

  // from uint8
  EXPECT_EQ((TriviallyConvertible<uint8, uint8>), true);
  EXPECT_EQ((TriviallyConvertible<uint8, uint16>), true);
  EXPECT_EQ((TriviallyConvertible<uint8, uint32>), true);
  EXPECT_EQ((TriviallyConvertible<uint8, uint64>), true);
  EXPECT_EQ((TriviallyConvertible<uint8, int8>), false);
  EXPECT_EQ((TriviallyConvertible<uint8, int16>), true);
  EXPECT_EQ((TriviallyConvertible<uint8, int32>), true);
  EXPECT_EQ((TriviallyConvertible<uint8, int64>), true);
  EXPECT_EQ((TriviallyConvertible<uint8, float32>), true);
  EXPECT_EQ((TriviallyConvertible<uint8, float64>), true);

  // from uint16
  EXPECT_EQ((TriviallyConvertible<uint16, uint8>), false);
  EXPECT_EQ((TriviallyConvertible<uint16, uint16>), true);
  EXPECT_EQ((TriviallyConvertible<uint16, uint32>), true);
  EXPECT_EQ((TriviallyConvertible<uint16, uint64>), true);
  EXPECT_EQ((TriviallyConvertible<uint16, int8>), false);
  EXPECT_EQ((TriviallyConvertible<uint16, int16>), false);
  EXPECT_EQ((TriviallyConvertible<uint16, int32>), true);
  EXPECT_EQ((TriviallyConvertible<uint16, int64>), true);
  EXPECT_EQ((TriviallyConvertible<uint16, float32>), true);
  EXPECT_EQ((TriviallyConvertible<uint16, float64>), true);

  // from uint32
  EXPECT_EQ((TriviallyConvertible<uint32, uint8>), false);
  EXPECT_EQ((TriviallyConvertible<uint32, uint16>), false);
  EXPECT_EQ((TriviallyConvertible<uint32, uint32>), true);
  EXPECT_EQ((TriviallyConvertible<uint32, uint64>), true);
  EXPECT_EQ((TriviallyConvertible<uint32, int8>), false);
  EXPECT_EQ((TriviallyConvertible<uint32, int16>), false);
  EXPECT_EQ((TriviallyConvertible<uint32, int32>), false);
  EXPECT_EQ((TriviallyConvertible<uint32, int64>), true);
  EXPECT_EQ((TriviallyConvertible<uint32, float32>), false);
  EXPECT_EQ((TriviallyConvertible<uint32, float64>), true);

  // from uint64
  EXPECT_EQ((TriviallyConvertible<uint64, uint8>), false);
  EXPECT_EQ((TriviallyConvertible<uint64, uint16>), false);
  EXPECT_EQ((TriviallyConvertible<uint64, uint32>), false);
  EXPECT_EQ((TriviallyConvertible<uint64, uint64>), true);
  EXPECT_EQ((TriviallyConvertible<uint64, int8>), false);
  EXPECT_EQ((TriviallyConvertible<uint64, int16>), false);
  EXPECT_EQ((TriviallyConvertible<uint64, int32>), false);
  EXPECT_EQ((TriviallyConvertible<uint64, int64>), false);
  EXPECT_EQ((TriviallyConvertible<uint64, float32>), false);
  EXPECT_EQ((TriviallyConvertible<uint64, float64>), false);

  // from float32
  EXPECT_EQ((TriviallyConvertible<float32, uint8>), false);
  EXPECT_EQ((TriviallyConvertible<float32, uint16>), false);
  EXPECT_EQ((TriviallyConvertible<float32, uint32>), false);
  EXPECT_EQ((TriviallyConvertible<float32, uint64>), false);
  EXPECT_EQ((TriviallyConvertible<float32, int8>), false);
  EXPECT_EQ((TriviallyConvertible<float32, int16>), false);
  EXPECT_EQ((TriviallyConvertible<float32, int32>), false);
  EXPECT_EQ((TriviallyConvertible<float32, int64>), false);
  EXPECT_EQ((TriviallyConvertible<float32, float32>), true);
  EXPECT_EQ((TriviallyConvertible<float32, float64>), true);

  // from float64
  EXPECT_EQ((TriviallyConvertible<float64, uint8>), false);
  EXPECT_EQ((TriviallyConvertible<float64, uint16>), false);
  EXPECT_EQ((TriviallyConvertible<float64, uint32>), false);
  EXPECT_EQ((TriviallyConvertible<float64, uint64>), false);
  EXPECT_EQ((TriviallyConvertible<float64, int8>), false);
  EXPECT_EQ((TriviallyConvertible<float64, int16>), false);
  EXPECT_EQ((TriviallyConvertible<float64, int32>), false);
  EXPECT_EQ((TriviallyConvertible<float64, int64>), false);
  EXPECT_EQ((TriviallyConvertible<float64, float32>), false);
  EXPECT_EQ((TriviallyConvertible<float64, float64>), true);
}

}  // namespace
}  // namespace tactile
