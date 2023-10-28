// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/misc/uuid.hpp"

#include <algorithm>    // count, count_if
#include <cctype>       // isxdigit
#include <type_traits>  // is_copy_constructible_v, is_move_constructible_v, is_copy_assignable_v, is_move_assignable_v

#include <gtest/gtest.h>

using namespace tactile;
using namespace tactile::int_literals;

/// \tests tactile::UUID::UUID
/// \tests tactile::UUID::is_null
/// \tests tactile::UUID::hash_code
TEST(UUID, Defaults)
{
  const UUID id;

  EXPECT_TRUE(id.is_null());
  EXPECT_EQ(id, id);
  EXPECT_NE(id.hash_code(), 0_uz);

  EXPECT_TRUE(std::is_copy_constructible_v<UUID>);
  EXPECT_TRUE(std::is_move_constructible_v<UUID>);
  EXPECT_TRUE(std::is_copy_assignable_v<UUID>);
  EXPECT_TRUE(std::is_move_assignable_v<UUID>);
}

/// \tests tactile::UUID::generate
TEST(UUID, Generate)
{
  const auto a = UUID::generate();
  const auto b = UUID::generate();
  const auto c = UUID::generate();

  EXPECT_EQ(a, a);
  EXPECT_EQ(b, b);
  EXPECT_EQ(c, c);

  EXPECT_NE(a, b);
  EXPECT_NE(a, c);
  EXPECT_NE(b, c);

  EXPECT_FALSE(a.is_null());
  EXPECT_FALSE(b.is_null());
  EXPECT_FALSE(c.is_null());
}

/// \tests tactile::UUID::generate
/// \tests tactile::UUID::hash_code
TEST(UUID, HashCode)
{
  const auto a = UUID::generate();
  const auto b = UUID::generate();
  EXPECT_EQ(a.hash_code(), a.hash_code());
  EXPECT_EQ(b.hash_code(), b.hash_code());
  EXPECT_NE(a.hash_code(), b.hash_code());
}

/// \tests tactile::to_string(const UUID&)
TEST(UUID, ToString)
{
  auto is_hex_digit = [](const char digit) -> bool {
    return std::isxdigit(static_cast<uchar>(digit));
  };

  {
    const UUID null;
    const auto str = to_string(null);
    EXPECT_EQ(to_string(null), "00000000-0000-0000-0000-000000000000");
  }

  {
    const auto id = UUID::generate();
    const auto str = to_string(id);
    EXPECT_EQ(std::count(str.begin(), str.end(), '-'), 4_uz);
    EXPECT_EQ(std::count_if(str.begin(), str.end(), is_hex_digit), 32_uz);
    ASSERT_EQ(str.size(), 36_uz);
    EXPECT_EQ(str[8], '-');
    EXPECT_EQ(str[13], '-');
    EXPECT_EQ(str[18], '-');
    EXPECT_EQ(str[23], '-');
    EXPECT_NE(str, "00000000-0000-0000-0000-000000000000");
  }
}
