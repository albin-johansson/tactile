#include <gtest/gtest.h>

#include "io/maps/parser/parse_map.hpp"

using namespace tactile;

using parsing::ParseError;

TEST(PropertyParserSpecialCases, JSON)
{
  const auto result = parsing::parse_map("test-resources/json/properties.tmj");
  ASSERT_EQ(ParseError::none, result.error());

  const auto& data = result.data();
  ASSERT_EQ(3u, data.context.properties.size());

  const auto& color = data.context.properties.at("empty-color");
  ASSERT_TRUE(color.is_color());
  ASSERT_TRUE(color.has_default_value());

  const auto& path = data.context.properties.at("empty-file");
  ASSERT_TRUE(path.is_file());
  ASSERT_TRUE(path.has_default_value());

  const auto& obj = data.context.properties.at("empty-object");
  ASSERT_TRUE(obj.is_object());
  ASSERT_TRUE(obj.has_default_value());
}