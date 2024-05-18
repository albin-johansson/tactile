// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/format.hpp"

#include <gtest/gtest.h>

#include "tactile/base/int.hpp"

namespace tactile {

/// \trace tactile::format_to_buffer
TEST(Format, FormatToBuffer)
{
  MemoryBuffer<char, 32> buffer;
  format_to_buffer(buffer, "{}", "foobar");

  ASSERT_EQ(buffer.size(), 7);
  EXPECT_EQ(buffer[0], 'f');
  EXPECT_EQ(buffer[1], 'o');
  EXPECT_EQ(buffer[2], 'o');
  EXPECT_EQ(buffer[3], 'b');
  EXPECT_EQ(buffer[4], 'a');
  EXPECT_EQ(buffer[5], 'r');
  EXPECT_EQ(buffer[6], '\0');
}

/// \trace tactile::format_to_buffer
TEST(Format, FormatToBufferWhenFull)
{
  MemoryBuffer<char, 32> buffer;
  for (usize i = 0; i < buffer.capacity(); ++i) {
    buffer.push_back('?');
  }

  ASSERT_EQ(buffer.size(), buffer.capacity());
  EXPECT_EQ(buffer.remaining_capacity(), 0);
  EXPECT_TRUE(buffer.full());

  format_to_buffer(buffer, "{}", "foobar");

  ASSERT_EQ(buffer.size(), buffer.capacity());
  EXPECT_EQ(buffer[buffer.size() - 3], '?');
  EXPECT_EQ(buffer[buffer.size() - 2], '?');
  EXPECT_EQ(buffer[buffer.size() - 1], '?');
}

/// \trace tactile::format_to_buffer
TEST(Format, FormatToBufferWithOverflow)
{
  MemoryBuffer<char, 32> buffer;
  for (usize i = 0; i < buffer.capacity() - 3; ++i) {
    buffer.push_back('?');
  }

  ASSERT_EQ(buffer.size(), buffer.capacity() - 3);
  EXPECT_EQ(buffer.remaining_capacity(), 3);
  EXPECT_FALSE(buffer.full());

  format_to_buffer(buffer, "{}", "foo");

  ASSERT_EQ(buffer.size(), buffer.capacity());
  EXPECT_EQ(buffer.remaining_capacity(), 0);
  EXPECT_TRUE(buffer.full());

  EXPECT_EQ(buffer[buffer.size() - 4], '?');
  EXPECT_EQ(buffer[buffer.size() - 3], 'f');
  EXPECT_EQ(buffer[buffer.size() - 2], 'o');
  EXPECT_EQ(buffer[buffer.size() - 1], '\0');
}

}  // namespace tactile
