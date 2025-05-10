// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

import tactile.core;

namespace tactile::tests {
namespace {

class ErrorTest : public testing::Test
{};

TEST_F(ErrorTest, ToString)
{
  EXPECT_EQ(to_string(Error::kUnknown), "unknown");
  EXPECT_EQ(to_string(Error::kOutOfMemory), "out_of_memory");
  EXPECT_EQ(to_string(Error::kUnsupportedFeature), "unsupported_feature");
  EXPECT_EQ(to_string(Error::kInvalidOp), "invalid_op");
  EXPECT_EQ(to_string(Error::kInvalidArg), "invalid_arg");
  EXPECT_EQ(to_string(Error::kInvalidFile), "invalid_file");
  EXPECT_EQ(to_string(Error::kNoSuchFile), "no_such_file");
  EXPECT_EQ(to_string(Error::kOutOfRange), "out_of_range");
  EXPECT_EQ(to_string(Error::kArithmeticOverflow), "arithmetic_overflow");
  EXPECT_EQ(to_string(Error::kArithmeticUnderflow), "arithmetic_underflow");
  EXPECT_EQ(to_string(Error::kArithmeticPrecision), "arithmetic_precision");
  EXPECT_EQ(to_string(Error::kArithmeticInvalidValue), "arithmetic_invalid_value");
  EXPECT_EQ(to_string(Error::kStackOverflow), "stack_overflow");
  EXPECT_EQ(to_string(Error::kStackUnderflow), "stack_underflow");
  EXPECT_EQ(to_string(Error::kCouldNotParseFile), "could_not_parse_file");
  EXPECT_EQ(to_string(Error::kCouldNotCompress), "could_not_compress");
  EXPECT_EQ(to_string(Error::kCouldNotDecompress), "could_not_decompress");
}

TEST_F(ErrorTest, Describe)
{
  EXPECT_EQ(describe(Error::kUnknown), "an unknown error occurred");
  EXPECT_EQ(describe(Error::kOutOfMemory), "out of memory");
  EXPECT_EQ(describe(Error::kUnsupportedFeature), "a feature isn't supported");
  EXPECT_EQ(describe(Error::kInvalidOp), "attempted an invalid operation");
  EXPECT_EQ(describe(Error::kInvalidArg), "detected an invalid argument");
  EXPECT_EQ(describe(Error::kInvalidFile), "detected an invalid file");
  EXPECT_EQ(describe(Error::kNoSuchFile), "an expected file didn't exist");
  EXPECT_EQ(describe(Error::kOutOfRange), "requested an out of range value");
  EXPECT_EQ(describe(Error::kArithmeticOverflow), "detected arithmetic overflow");
  EXPECT_EQ(describe(Error::kArithmeticUnderflow), "detected arithmetic underflow");
  EXPECT_EQ(describe(Error::kArithmeticPrecision),
            "detected loss of arithmetic precision");
  EXPECT_EQ(describe(Error::kArithmeticInvalidValue),
            "detected invalid arithmetic value");
  EXPECT_EQ(describe(Error::kStackOverflow), "detected stack overflow");
  EXPECT_EQ(describe(Error::kStackUnderflow), "detected stack underflow");
  EXPECT_EQ(describe(Error::kCouldNotParseFile), "could not parse a file");
  EXPECT_EQ(describe(Error::kCouldNotCompress), "could not compress data");
  EXPECT_EQ(describe(Error::kCouldNotDecompress), "could not decompress data");
}

}  // namespace
}  // namespace tactile::tests
