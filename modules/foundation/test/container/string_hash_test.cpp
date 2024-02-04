// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

#include "tactile/foundation/container/string_map.hpp"

using namespace tactile;

using namespace std::string_literals;
using namespace std::string_view_literals;

TEST(StringHash, HashDifferentStringRepresentations)
{
  const auto c_str_hash = StringHash {}("foobar");
  const auto str_view_hash = StringHash {}("foobar"sv);
  const auto str_hash = StringHash {}("foobar"s);

  EXPECT_EQ(c_str_hash, str_hash);
  EXPECT_EQ(c_str_hash, str_view_hash);
  EXPECT_EQ(str_hash, str_view_hash);
}
