// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/memory_buffer.hpp"

#include <algorithm>  // fill_n
#include <iterator>   // distance, back_inserter

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/container/array.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

using testing::Const;

/// \trace tactile::MemoryBuffer::MemoryBuffer
TEST(MemoryBuffer, DefaultConstructor)
{
  MemoryBuffer buffer;  // NOLINT

  EXPECT_FALSE(buffer.full());
  EXPECT_TRUE(buffer.empty());
  EXPECT_EQ(buffer.size(), 0);
  EXPECT_EQ(buffer.capacity(), kDefaultMemoryBufferCapacity);
  EXPECT_EQ(buffer.remaining_capacity(), kDefaultMemoryBufferCapacity);

  EXPECT_EQ(buffer.data(), nullptr);
  EXPECT_NE(buffer.begin(), nullptr);
  EXPECT_NE(buffer.end(), nullptr);

  EXPECT_EQ(Const(buffer).data(), nullptr);
  EXPECT_NE(Const(buffer).begin(), nullptr);
  EXPECT_NE(Const(buffer).end(), nullptr);

  EXPECT_EQ(std::distance(buffer.begin(), buffer.end()), 0);
  EXPECT_EQ(std::distance(Const(buffer).begin(), Const(buffer).end()), 0);
}

/// \trace tactile::MemoryBuffer::clear
TEST(MemoryBuffer, Clear)
{
  MemoryBuffer<char> buffer;  // NOLINT
  ASSERT_EQ(buffer.size(), 0);

  buffer.push_back('1');
  buffer.push_back('2');
  buffer.push_back('3');
  EXPECT_FALSE(buffer.empty());
  EXPECT_EQ(buffer.size(), 3);
  EXPECT_EQ(buffer.remaining_capacity(), buffer.capacity() - buffer.size());

  buffer.clear();
  EXPECT_TRUE(buffer.empty());
  EXPECT_EQ(buffer.size(), 0);
  EXPECT_EQ(buffer.remaining_capacity(), buffer.capacity());
}

/// \trace tactile::MemoryBuffer::push_back
TEST(MemoryBuffer, PushBack)
{
  MemoryBuffer<char> buffer;  // NOLINT
  ASSERT_EQ(buffer.size(), 0);

  buffer.push_back('f');
  ASSERT_EQ(buffer.size(), 1);
  EXPECT_EQ(buffer.at(0), 'f');

  buffer.push_back('o');
  ASSERT_EQ(buffer.size(), 2);
  EXPECT_EQ(buffer.at(0), 'f');
  EXPECT_EQ(buffer.at(1), 'o');

  buffer.push_back('o');
  ASSERT_EQ(buffer.size(), 3);
  EXPECT_EQ(buffer.at(0), 'f');
  EXPECT_EQ(buffer.at(1), 'o');
  EXPECT_EQ(buffer.at(2), 'o');

  buffer.push_back('b');
  ASSERT_EQ(buffer.size(), 4);
  EXPECT_EQ(buffer.at(0), 'f');
  EXPECT_EQ(buffer.at(1), 'o');
  EXPECT_EQ(buffer.at(2), 'o');
  EXPECT_EQ(buffer.at(3), 'b');

  buffer.push_back('a');
  ASSERT_EQ(buffer.size(), 5);
  EXPECT_EQ(buffer.at(0), 'f');
  EXPECT_EQ(buffer.at(1), 'o');
  EXPECT_EQ(buffer.at(2), 'o');
  EXPECT_EQ(buffer.at(3), 'b');
  EXPECT_EQ(buffer.at(4), 'a');

  buffer.push_back('r');
  ASSERT_EQ(buffer.size(), 6);
  EXPECT_EQ(buffer.at(0), 'f');
  EXPECT_EQ(buffer.at(1), 'o');
  EXPECT_EQ(buffer.at(2), 'o');
  EXPECT_EQ(buffer.at(3), 'b');
  EXPECT_EQ(buffer.at(4), 'a');
  EXPECT_EQ(buffer.at(5), 'r');
}

/// \trace tactile::MemoryBuffer::push_back
TEST(MemoryBuffer, PushBackWithOverflow)
{
  MemoryBuffer<char> buffer;  // NOLINT
  ASSERT_EQ(buffer.size(), 0);

  std::fill_n(std::back_inserter(buffer), buffer.remaining_capacity(), '?');
  ASSERT_EQ(buffer.size(), buffer.capacity());
  ASSERT_EQ(buffer.remaining_capacity(), 0);
}

/// \trace tactile::MemoryBuffer::push_back
TEST(MemoryBuffer, BackInserterCompatibility)
{
  MemoryBuffer<char> buffer;  // NOLINT
  ASSERT_EQ(buffer.size(), 0);

  std::fill_n(std::back_inserter(buffer), buffer.remaining_capacity(), '?');

  EXPECT_TRUE(buffer.full());
  EXPECT_EQ(buffer.remaining_capacity(), 0);
  ASSERT_EQ(buffer.size(), buffer.capacity());
  EXPECT_EQ(buffer.at(0), '?');
  EXPECT_EQ(buffer.at(buffer.size() - 1), '?');

  // This should not affect the buffer, it's already full.
  std::fill_n(std::back_inserter(buffer), 10, '!');

  EXPECT_TRUE(buffer.full());
  EXPECT_EQ(buffer.remaining_capacity(), 0);
  ASSERT_EQ(buffer.size(), buffer.capacity());
  EXPECT_EQ(buffer.at(0), '?');
  EXPECT_EQ(buffer.at(buffer.size() - 1), '?');
}

/// \trace tactile::MemoryBuffer::append [iterator version]
TEST(MemoryBuffer, AppendIteratorRange)
{
  const Array<char, 4> values = {'!', '?', '+', '-'};
  MemoryBuffer<char, 7> buffer;  // NOLINT

  buffer.append(values.begin(), values.end());
  EXPECT_EQ(buffer.size(), 4);
  EXPECT_EQ(buffer.remaining_capacity(), 3);
  EXPECT_FALSE(buffer.full());

  // All elements won't fit, only some will be included.
  buffer.append(values.begin(), values.end());
  EXPECT_EQ(buffer.size(), 7);
  EXPECT_EQ(buffer.remaining_capacity(), 0);
  EXPECT_TRUE(buffer.full());

  EXPECT_EQ(buffer.at(0), '!');
  EXPECT_EQ(buffer.at(1), '?');
  EXPECT_EQ(buffer.at(2), '+');
  EXPECT_EQ(buffer.at(3), '-');
  EXPECT_EQ(buffer.at(4), '!');
  EXPECT_EQ(buffer.at(5), '?');
  EXPECT_EQ(buffer.at(6), '+');
}

/// \trace tactile::MemoryBuffer::append [container version]
TEST(MemoryBuffer, AppendContainer)
{
  const Vector<uint32> values = {1, 2, 3, 4, 5};
  MemoryBuffer<uint32, 8> buffer;  // NOLINT
  ASSERT_EQ(buffer.size(), 0);

  buffer.append(values);
  ASSERT_EQ(buffer.size(), 5);
  EXPECT_EQ(buffer.capacity(), 8);
  EXPECT_EQ(buffer.remaining_capacity(), 3);
  EXPECT_FALSE(buffer.full());

  EXPECT_EQ(buffer.at(0), 1);
  EXPECT_EQ(buffer.at(1), 2);
  EXPECT_EQ(buffer.at(2), 3);
  EXPECT_EQ(buffer.at(3), 4);
  EXPECT_EQ(buffer.at(4), 5);

  buffer.append(values);
  ASSERT_EQ(buffer.size(), 8);
  EXPECT_EQ(buffer.capacity(), 8);
  EXPECT_EQ(buffer.remaining_capacity(), 0);
  EXPECT_TRUE(buffer.full());

  EXPECT_EQ(buffer.at(0), 1);
  EXPECT_EQ(buffer.at(1), 2);
  EXPECT_EQ(buffer.at(2), 3);
  EXPECT_EQ(buffer.at(3), 4);
  EXPECT_EQ(buffer.at(4), 5);
  EXPECT_EQ(buffer.at(5), 1);
  EXPECT_EQ(buffer.at(6), 2);
  EXPECT_EQ(buffer.at(7), 3);
}

/// \trace tactile::MemoryBuffer::operator[]
TEST(MemoryBuffer, SubscriptOperator)
{
  MemoryBuffer<char, 4> buffer;  // NOLINT
  buffer.push_back('a');
  buffer.push_back('b');
  buffer.push_back('c');
  ASSERT_EQ(buffer.size(), 3);

  EXPECT_EQ(buffer[0], 'a');
  EXPECT_EQ(buffer[1], 'b');
  EXPECT_EQ(buffer[2], 'c');
  EXPECT_EQ(Const(buffer)[0], 'a');
  EXPECT_EQ(Const(buffer)[1], 'b');
  EXPECT_EQ(Const(buffer)[2], 'c');
}

/// \trace tactile::MemoryBuffer::at
TEST(MemoryBuffer, At)
{
  MemoryBuffer<char, 4> buffer;  // NOLINT
  buffer.push_back('1');
  buffer.push_back('2');
  ASSERT_EQ(buffer.size(), 2);

  EXPECT_EQ(buffer.at(0), '1');
  EXPECT_EQ(buffer.at(1), '2');
  EXPECT_EQ(Const(buffer).at(0), '1');
  EXPECT_EQ(Const(buffer).at(1), '2');

  EXPECT_THROW((void) buffer.at(2), Exception);
  EXPECT_THROW((void) Const(buffer).at(2), Exception);
}

/// \trace tactile::MemoryBuffer::data
TEST(MemoryBuffer, Data)
{
  MemoryBuffer<char, 32> buffer;  // NOLINT
  ASSERT_EQ(buffer.size(), 0);

  EXPECT_EQ(buffer.data(), nullptr);
  EXPECT_EQ(Const(buffer).data(), nullptr);

  buffer.push_back('#');
  ASSERT_EQ(buffer.size(), 1);

  EXPECT_NE(buffer.data(), nullptr);
  EXPECT_NE(Const(buffer).data(), nullptr);
}

/// \trace tactile::MemoryBuffer::begin
/// \trace tactile::MemoryBuffer::end
TEST(MemoryBuffer, Iteration)
{
  MemoryBuffer<uint16, 6> buffer;  // NOLINT

  EXPECT_EQ(std::distance(buffer.begin(), buffer.end()), 0);
  EXPECT_EQ(std::distance(Const(buffer).begin(), Const(buffer).end()), 0);

  buffer.push_back(0xF1);
  buffer.push_back(0xF2);
  buffer.push_back(0xF3);

  EXPECT_EQ(std::distance(buffer.begin(), buffer.end()), 3);
  EXPECT_EQ(std::distance(Const(buffer).begin(), Const(buffer).end()), 3);
}

}  // namespace tactile
