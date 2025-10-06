// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/util/format_buffer.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace tactile {
namespace {

using testing::Each;
using testing::Eq;

class FormatBufferTest : public testing::Test
{
 protected:
  FormatBuffer<10> m_buffer {};
};

TEST_F(FormatBufferTest, Clear)
{
  m_buffer.push_back('f');
  m_buffer.push_back('o');
  m_buffer.push_back('o');

  EXPECT_EQ(m_buffer.view().size(), 3);
  EXPECT_EQ(m_buffer.view(), "foo");

  m_buffer.clear();

  EXPECT_EQ(m_buffer.view().size(), 0);
  EXPECT_EQ(m_buffer.view(), "");
}

TEST_F(FormatBufferTest, PushBack)
{
  EXPECT_EQ(m_buffer.view().size(), 0);
  EXPECT_EQ(m_buffer.view(), "");

  m_buffer.push_back('f');

  EXPECT_EQ(m_buffer.view().size(), 1);
  EXPECT_EQ(m_buffer.view(), "f");

  m_buffer.push_back('o');

  EXPECT_EQ(m_buffer.view().size(), 2);
  EXPECT_EQ(m_buffer.view(), "fo");

  m_buffer.push_back('o');

  EXPECT_EQ(m_buffer.view().size(), 3);
  EXPECT_EQ(m_buffer.view(), "foo");

  m_buffer.push_back('b');

  EXPECT_EQ(m_buffer.view().size(), 4);
  EXPECT_EQ(m_buffer.view(), "foob");

  m_buffer.push_back('a');

  EXPECT_EQ(m_buffer.view().size(), 5);
  EXPECT_EQ(m_buffer.view(), "fooba");

  m_buffer.push_back('r');

  EXPECT_EQ(m_buffer.view().size(), 6);
  EXPECT_EQ(m_buffer.view(), "foobar");
}

TEST_F(FormatBufferTest, PushBackOverflow)
{
  m_buffer.push_back('a');
  m_buffer.push_back('b');
  m_buffer.push_back('c');
  m_buffer.push_back('d');
  m_buffer.push_back('e');
  m_buffer.push_back('f');
  m_buffer.push_back('g');
  m_buffer.push_back('h');
  m_buffer.push_back('i');
  m_buffer.push_back('j');
  m_buffer.push_back('k');  // ignored

  EXPECT_EQ(m_buffer.view().size(), 10);
  EXPECT_EQ(m_buffer.view(), "abcdefghij");
}

}  // namespace
}  // namespace tactile
