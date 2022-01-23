#include "core/utils/buffer_utils.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(BufferUtils, ZeroBuffer)
{
  std::array<char, 5> buffer;  // NOLINT
  ZeroBuffer(buffer);

  ASSERT_EQ(0, buffer.at(0));
  ASSERT_EQ(0, buffer.at(1));
  ASSERT_EQ(0, buffer.at(2));
  ASSERT_EQ(0, buffer.at(3));
  ASSERT_EQ(0, buffer.at(4));
}

TEST(BufferUtils, CopyStringIntoBuffer)
{
  std::array<char, 10> buffer;  // NOLINT
  CopyStringIntoBuffer(buffer, "abcdef");

  ASSERT_EQ('a', buffer.at(0));
  ASSERT_EQ('b', buffer.at(1));
  ASSERT_EQ('c', buffer.at(2));
  ASSERT_EQ('d', buffer.at(3));
  ASSERT_EQ('e', buffer.at(4));
  ASSERT_EQ('f', buffer.at(5));

  // The buffer should have been zeroed
  ASSERT_EQ(0, buffer.at(6));
  ASSERT_EQ(0, buffer.at(7));
  ASSERT_EQ(0, buffer.at(8));
  ASSERT_EQ(0, buffer.at(9));
}

TEST(BufferUtils, CopyStringIntoBufferThatIsTooSmall)
{
  std::array<char, 4> buffer;  // NOLINT
  CopyStringIntoBuffer(buffer, "abcdef");

  ASSERT_EQ('a', buffer.at(0));
  ASSERT_EQ('b', buffer.at(1));
  ASSERT_EQ('c', buffer.at(2));
  ASSERT_EQ('d', buffer.at(3));
}

TEST(BufferUtils, CreateStringFromBuffer)
{
  const std::array buffer = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
  const auto str = CreateStringFromBuffer(buffer);

  ASSERT_EQ("abcdef", str);
}

TEST(BufferUtils, CreateStringViewFromBuffer)
{
  const std::array buffer = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
  const auto str = CreateStringViewFromBuffer(buffer);

  ASSERT_EQ(6, str.size());
  ASSERT_EQ("abcdef", str);
}
