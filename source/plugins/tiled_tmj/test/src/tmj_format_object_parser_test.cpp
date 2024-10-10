// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_object_parser.hpp"

#include <gtest/gtest.h>

#include "tactile/runtime/command_line_options.hpp"
#include "tactile/runtime/runtime.hpp"

namespace tactile::test {

class TmjFormatObjectParserTest : public testing::Test
{
 private:
  runtime::Runtime mRuntime {runtime::get_default_command_line_options()};
};

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseValidRectangleObject)
{
  const nlohmann::json object_json = {
    {"name", "R"},
    {"id", 1},
    {"x", 2.0f},
    {"y", 3.0f},
    {"width", 4.0f},
    {"height", 5.0f},
    {"visible", false},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_TRUE(object.has_value());

  EXPECT_EQ(object->meta.name, "R");
  EXPECT_EQ(object->meta.properties.size(), 0);
  EXPECT_EQ(object->meta.components.size(), 0);
  EXPECT_EQ(object->id, ObjectID {1});
  EXPECT_EQ(object->type, ObjectType::kRect);
  EXPECT_EQ(object->position.x(), 2.0f);
  EXPECT_EQ(object->position.y(), 3.0f);
  EXPECT_EQ(object->size.x(), 4.0f);
  EXPECT_EQ(object->size.y(), 5.0f);
  EXPECT_EQ(object->tag, "");
  EXPECT_FALSE(object->visible);
}

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseValidEllipseObject)
{
  const nlohmann::json object_json = {
    {"name", "E"},
    {"id", 10},
    {"x", 12.0f},
    {"y", 34.0f},
    {"width", 56.0f},
    {"height", 78.0f},
    {"visible", true},
    {"ellipse", true},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_TRUE(object.has_value());

  EXPECT_EQ(object->meta.name, "E");
  EXPECT_EQ(object->meta.properties.size(), 0);
  EXPECT_EQ(object->meta.components.size(), 0);
  EXPECT_EQ(object->id, ObjectID {10});
  EXPECT_EQ(object->type, ObjectType::kEllipse);
  EXPECT_EQ(object->position.x(), 12.0f);
  EXPECT_EQ(object->position.y(), 34.0f);
  EXPECT_EQ(object->size.x(), 56.0f);
  EXPECT_EQ(object->size.y(), 78.0f);
  EXPECT_EQ(object->tag, "");
  EXPECT_TRUE(object->visible);
}

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseValidPointObject)
{
  const nlohmann::json object_json = {
    {"name", "P"},
    {"id", 99},
    {"x", 50.0f},
    {"y", 51.0f},
    {"width", 52.0f},
    {"height", 53.0f},
    {"visible", true},
    {"type", "foobar"},
    {"point", true},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_TRUE(object.has_value());

  EXPECT_EQ(object->meta.name, "P");
  EXPECT_EQ(object->meta.properties.size(), 0);
  EXPECT_EQ(object->meta.components.size(), 0);
  EXPECT_EQ(object->id, ObjectID {99});
  EXPECT_EQ(object->type, ObjectType::kPoint);
  EXPECT_EQ(object->position.x(), 50.0f);
  EXPECT_EQ(object->position.y(), 51.0f);
  EXPECT_EQ(object->size.x(), 52.0f);
  EXPECT_EQ(object->size.y(), 53.0f);
  EXPECT_EQ(object->tag, "foobar");
  EXPECT_TRUE(object->visible);
}

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseObjectWithoutName)
{
  const nlohmann::json object_json = {
    {"id", 0},
    {"x", 0.0f},
    {"y", 0.0f},
    {"width", 0.0f},
    {"height", 0.0f},
    {"visible", false},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_FALSE(object.has_value());

  EXPECT_EQ(object.error(), ErrorCode::kParseError);
}

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseObjectWithoutId)
{
  const nlohmann::json object_json = {
    {"name", ""},
    {"x", 0.0f},
    {"y", 0.0f},
    {"width", 0.0f},
    {"height", 0.0f},
    {"visible", false},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_FALSE(object.has_value());

  EXPECT_EQ(object.error(), ErrorCode::kParseError);
}

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseObjectWithoutX)
{
  const nlohmann::json object_json = {
    {"name", ""},
    {"id", 0},
    {"y", 0.0f},
    {"width", 0.0f},
    {"height", 0.0f},
    {"visible", false},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_FALSE(object.has_value());

  EXPECT_EQ(object.error(), ErrorCode::kParseError);
}

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseObjectWithoutY)
{
  const nlohmann::json object_json = {
    {"name", ""},
    {"id", 0},
    {"x", 0.0f},
    {"width", 0.0f},
    {"height", 0.0f},
    {"visible", false},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_FALSE(object.has_value());

  EXPECT_EQ(object.error(), ErrorCode::kParseError);
}

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseObjectWithoutWidth)
{
  const nlohmann::json object_json = {
    {"name", ""},
    {"id", 0},
    {"x", 0.0f},
    {"y", 0.0f},
    {"height", 0.0f},
    {"visible", false},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_FALSE(object.has_value());

  EXPECT_EQ(object.error(), ErrorCode::kParseError);
}

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseObjectWithoutHeight)
{
  const nlohmann::json object_json = {
    {"name", ""},
    {"id", 0},
    {"x", 0.0f},
    {"y", 0.0f},
    {"width", 0.0f},
    {"visible", false},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_FALSE(object.has_value());

  EXPECT_EQ(object.error(), ErrorCode::kParseError);
}

// tactile::parse_tiled_tmj_object
TEST_F(TmjFormatObjectParserTest, ParseObjectWithoutVisibility)
{
  const nlohmann::json object_json = {
    {"name", ""},
    {"id", 0},
    {"x", 0.0f},
    {"y", 0.0f},
    {"width", 0.0f},
    {"height", 0.0f},
  };

  const auto object = parse_tiled_tmj_object(object_json);
  ASSERT_FALSE(object.has_value());

  EXPECT_EQ(object.error(), ErrorCode::kParseError);
}

}  // namespace tactile::test
