#include <string_view>  // string_view

#include <gtest/gtest.h>

#include "io/maps/parser/parse_map.hpp"

using namespace tactile;

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