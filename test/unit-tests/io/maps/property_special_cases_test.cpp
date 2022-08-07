/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <string_view>  // string_view

#include <gtest/gtest.h>

#include "io/map/parse/parse_map.hpp"

namespace tactile::test {

using io::ParseError;

const auto cases = testing::Values("test-resources/json/properties.tmj",
                                   "test-resources/xml/properties.tmx");

struct PropertySpecialCaseTest : testing::TestWithParam<std::string_view>
{};

TEST_P(PropertySpecialCaseTest, Parsing)
{
  const auto path = GetParam();
  const auto result = io::parse_map(path);
  ASSERT_EQ(ParseError::None, result.error());

  const auto& data = result.data();
  ASSERT_EQ(3u, data.context.properties.size());

  const auto& color = data.context.properties.at("empty-color");
  ASSERT_TRUE(color.is_color());
  ASSERT_TRUE(color.has_default_value());

  const auto& file = data.context.properties.at("empty-file");
  ASSERT_TRUE(file.is_path());
  ASSERT_TRUE(file.has_default_value());

  const auto& obj = data.context.properties.at("empty-object");
  ASSERT_TRUE(obj.is_object());
  ASSERT_TRUE(obj.has_default_value());
}

INSTANTIATE_TEST_SUITE_P(PropertySpecialCaseTests, PropertySpecialCaseTest, cases);

}  // namespace tactile::test
