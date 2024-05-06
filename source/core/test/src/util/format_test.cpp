// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/format.hpp"

#include <gtest/gtest.h>

#include "tactile/base/int.hpp"

namespace tactile {

/// \trace format_cstr_to
TEST(Format, FormatCstrTo)
{
  fmt::memory_buffer buffer {};
  format_cstr_to(buffer, "{}", "foobar");

  ASSERT_EQ(buffer.size(), 7);
  EXPECT_EQ(buffer[0], 'f');
  EXPECT_EQ(buffer[1], 'o');
  EXPECT_EQ(buffer[2], 'o');
  EXPECT_EQ(buffer[3], 'b');
  EXPECT_EQ(buffer[4], 'a');
  EXPECT_EQ(buffer[5], 'r');
  EXPECT_EQ(buffer[6], '\0');
}

/// \trace format_cstr_to
TEST(Format, FormatCstrToWhenFull)
{
  fmt::memory_buffer buffer {};
  for (usize i = 0; i < buffer.capacity(); ++i) {
    buffer.push_back('?');
  }

  ASSERT_EQ(buffer.size(), buffer.capacity());

  format_cstr_to(buffer, "{}", "foobar");

  ASSERT_EQ(buffer.size(), buffer.capacity());
  EXPECT_EQ(buffer[buffer.size() - 3], '?');
  EXPECT_EQ(buffer[buffer.size() - 2], '?');
  EXPECT_EQ(buffer[buffer.size() - 1], '?');
}

/// \trace format_cstr_to
TEST(Format, FormatCstrToWithOverflow)
{
  fmt::memory_buffer buffer {};
  for (usize i = 0; i < buffer.capacity() - 3; ++i) {
    buffer.push_back('?');
  }

  ASSERT_EQ(buffer.size(), buffer.capacity() - 3);

  format_cstr_to(buffer, "{}", "foo");

  ASSERT_EQ(buffer.size(), buffer.capacity());
  EXPECT_EQ(buffer[buffer.size() - 4], '?');
  EXPECT_EQ(buffer[buffer.size() - 3], 'f');
  EXPECT_EQ(buffer[buffer.size() - 2], 'o');
  EXPECT_EQ(buffer[buffer.size() - 1], '\0');
}

/// \trace add_cstr_to
TEST(Format, AddCstrTo)
{
  fmt::memory_buffer buffer {};
  add_cstr_to(buffer, "0123456789");

  ASSERT_EQ(buffer.size(), 10);
  EXPECT_EQ(buffer[0], '0');
  EXPECT_EQ(buffer[1], '1');
  EXPECT_EQ(buffer[2], '2');
  EXPECT_EQ(buffer[3], '3');
  EXPECT_EQ(buffer[4], '4');
  EXPECT_EQ(buffer[5], '5');
  EXPECT_EQ(buffer[6], '6');
  EXPECT_EQ(buffer[7], '7');
  EXPECT_EQ(buffer[8], '8');
  EXPECT_EQ(buffer[9], '9');
}

/// \trace add_cstr_to
TEST(Format, AddCstrToWithOverflow)
{
  fmt::memory_buffer buffer {};
  for (usize i = 0; i < buffer.capacity() - 1; ++i) {
    buffer.push_back('?');
  }

  ASSERT_EQ(buffer.size(), buffer.capacity() - 1);

  add_cstr_to(buffer, "12");

  ASSERT_EQ(buffer.size(), buffer.capacity());
  EXPECT_EQ(buffer[0], '?');
  EXPECT_EQ(buffer[buffer.size() - 2], '?');
  EXPECT_EQ(buffer[buffer.size() - 1], '1');
}

}  // namespace tactile
