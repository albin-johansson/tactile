#include "core/common/string.hpp"

#include <gtest/gtest.h>

#include "core/common/filesystem.hpp"

using namespace tactile;

TEST(StringUtils, IntegerFromString)
{
  ASSERT_FALSE(from_string<int>("ABC"));

  ASSERT_EQ(-42, from_string<int>("-42"));
  ASSERT_EQ(123, from_string<int16>("123"));
  ASSERT_EQ(745, from_string<int64>("745"));

  ASSERT_FALSE(from_string<int>("F1"));
  ASSERT_EQ(0xF1, from_string<int>("F1", 16));

  ASSERT_EQ(4, from_string<int>("4298", 1u, 10));
  ASSERT_EQ(0x12, from_string<int>("12AC", 2u, 16));

  // Robustness against invalid length
  ASSERT_EQ(1234, from_string<int>("1234"));
  ASSERT_EQ(1234, from_string<int>("1234", 4u, 10));
  ASSERT_EQ(1234, from_string<int>("1234", 5u, 10));
  ASSERT_EQ(1234, from_string<int>("1234", 6u, 10));
}

TEST(StringUtils, FloatFromString)
{
  ASSERT_FALSE(from_string<float>("foo"));

  ASSERT_EQ(12.3f, from_string<float>("12.3"));
  ASSERT_EQ(42.0f, from_string<float>("42"));

  ASSERT_EQ(78.5, from_string<double>("78.5"));
  ASSERT_EQ(98.0, from_string<double>("98"));
}

TEST(StringUtils, ConvertToForwardSlashes)
{
  const std::filesystem::path source = R"(C:\foo\bar\abc.yaml)";
  ASSERT_EQ("C:/foo/bar/abc.yaml", convert_to_forward_slashes(source));
}