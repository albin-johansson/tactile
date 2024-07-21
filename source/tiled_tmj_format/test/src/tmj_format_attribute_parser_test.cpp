// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_attribute_parser.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/runtime/runtime.hpp"

namespace tactile::test {

class TmjFormatAttributeParserTest : public testing::Test
{
 private:
  Runtime mRuntime {};
};

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParsePropertyWithoutName)
{
  const nlohmann::json property_json = {
    {"type", "int"},
    {"value", 0},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_FALSE(property.has_value());
  EXPECT_EQ(property.error(), SaveFormatParseError::kNoPropertyName);
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParsePropertyWithoutType)
{
  const nlohmann::json property_json = {
    {"name", "abc"},
    {"value", 0},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_FALSE(property.has_value());
  EXPECT_EQ(property.error(), SaveFormatParseError::kNoPropertyType);
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParsePropertyWithInvalidType)
{
  const nlohmann::json property_json = {
    {"name", "abc"},
    {"type", "int2"},
    {"value", "0;0"},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_FALSE(property.has_value());
  EXPECT_EQ(property.error(), SaveFormatParseError::kBadPropertyType);
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParsePropertyWithoutValue)
{
  const nlohmann::json property_json = {
    {"name", "abc"},
    {"type", "string"},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_FALSE(property.has_value());
  EXPECT_EQ(property.error(), SaveFormatParseError::kNoPropertyValue);
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseValidString)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "string"},
    {"value", "bar"},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_TRUE(property.has_value());
  EXPECT_EQ(property->name, "foo");
  EXPECT_EQ(property->value.get_type(), AttributeType::kStr);
  EXPECT_EQ(property->value, Attribute {"bar"});
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseValidInt)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "int"},
    {"value", 42},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_TRUE(property.has_value());
  EXPECT_EQ(property->name, "foo");
  EXPECT_EQ(property->value.get_type(), AttributeType::kInt);
  EXPECT_EQ(property->value, Attribute {42});
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseValidFloat)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "float"},
    {"value", 1.5f},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_TRUE(property.has_value());
  EXPECT_EQ(property->name, "foo");
  EXPECT_EQ(property->value.get_type(), AttributeType::kFloat);
  EXPECT_EQ(property->value, Attribute {1.5f});
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseValidBool)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "bool"},
    {"value", true},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_TRUE(property.has_value());
  EXPECT_EQ(property->name, "foo");
  EXPECT_EQ(property->value.get_type(), AttributeType::kBool);
  EXPECT_EQ(property->value, Attribute {true});
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseValidPath)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "file"},
    {"value", "b/a/r"},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_TRUE(property.has_value());
  EXPECT_EQ(property->name, "foo");
  EXPECT_EQ(property->value.get_type(), AttributeType::kPath);
  EXPECT_EQ(property->value, Attribute {Path {"b/a/r"}});
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseValidColorRGB)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "color"},
    {"value", "#ABCDEF"},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_TRUE(property.has_value());
  EXPECT_EQ(property->name, "foo");
  EXPECT_EQ(property->value.get_type(), AttributeType::kColor);
  EXPECT_EQ(property->value, Attribute(UColor(0xAB, 0xCD, 0xEF, 0xFF)));
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseValidColorARGB)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "color"},
    {"value", "#1A2B3C4D"},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_TRUE(property.has_value());
  EXPECT_EQ(property->name, "foo");
  EXPECT_EQ(property->value.get_type(), AttributeType::kColor);
  EXPECT_EQ(property->value, Attribute(UColor(0x2B, 0x3C, 0x4D, 0x1A)));
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseMalformedColorARGB)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "color"},
    {"value", "#0000000G"},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_FALSE(property.has_value());
  EXPECT_EQ(property.error(), SaveFormatParseError::kBadPropertyValue);
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseTooShortColorARGB)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "color"},
    {"value", "#0000000"},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_FALSE(property.has_value());
  EXPECT_EQ(property.error(), SaveFormatParseError::kBadPropertyValue);
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseTooLongColorARGB)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "color"},
    {"value", "#000000000"},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_FALSE(property.has_value());
  EXPECT_EQ(property.error(), SaveFormatParseError::kBadPropertyValue);
}

// tactile::parse_tiled_tmj_property
TEST_F(TmjFormatAttributeParserTest, ParseValidObject)
{
  const nlohmann::json property_json = {
    {"name", "foo"},
    {"type", "object"},
    {"value", 123},
  };

  const auto property = parse_tiled_tmj_property(property_json);

  ASSERT_TRUE(property.has_value());
  EXPECT_EQ(property->name, "foo");
  EXPECT_EQ(property->value.get_type(), AttributeType::kObject);
  EXPECT_EQ(property->value, Attribute {ObjectRef {123}});
}

// tactile::parse_tiled_tmj_metadata
TEST_F(TmjFormatAttributeParserTest, ParseTiledTmjMetadata)
{
  using namespace nlohmann::json_literals;
  using testing::Contains;
  using testing::Eq;

  const auto metadata_json = R"({
    "properties": [
      {
        "name": "a_string",
        "type": "string",
        "value": "foo"
      },
      {
        "name": "an_int",
        "type": "int",
        "value": 42
      },
      {
        "name": "a_float",
        "type": "float",
        "value": 10.0
      },
      {
        "name": "a_bool",
        "type": "bool",
        "value": false
      },
      {
        "name": "a_color",
        "type": "color",
        "value": "#1A2B3C4D"
      },
      {
        "name": "a_path",
        "type": "file",
        "value": "a/b/c"
      },
      {
        "name": "an_object",
        "type": "object",
        "value": 123
      }
    ]
  })"_json;

  const auto metadata = parse_tiled_tmj_metadata(metadata_json);
  ASSERT_TRUE(metadata.has_value());

  EXPECT_EQ(metadata->name, "");
  EXPECT_EQ(metadata->properties.size(), 7);
  EXPECT_EQ(metadata->components.size(), 0);

  EXPECT_THAT(metadata->properties,
              Contains(Eq(ir::NamedAttribute {"a_string", Attribute {"foo"}})));
  EXPECT_THAT(metadata->properties,
              Contains(Eq(ir::NamedAttribute {"an_int", Attribute {42}})));
  EXPECT_THAT(metadata->properties,
              Contains(Eq(ir::NamedAttribute {"a_float", Attribute {10.0f}})));
  EXPECT_THAT(metadata->properties,
              Contains(Eq(ir::NamedAttribute {"a_bool", Attribute {false}})));
  EXPECT_THAT(metadata->properties,
              Contains(Eq(ir::NamedAttribute {"a_color",
                                              Attribute {UColor(0x2B, 0x3C, 0x4D, 0x1A)}})));
  EXPECT_THAT(metadata->properties,
              Contains(Eq(ir::NamedAttribute {"a_path", Attribute {Path {"a/b/c"}}})));
  EXPECT_THAT(metadata->properties,
              Contains(Eq(ir::NamedAttribute {"an_object", Attribute {ObjectRef {123}}})));
}

// tactile::parse_tiled_tmj_metadata
TEST_F(TmjFormatAttributeParserTest, ParseTiledTmjMetadataWithName)
{
  using namespace nlohmann::json_literals;

  const auto metadata_json = R"({
    "name": "test",
    "properties": []
  })"_json;

  const auto metadata = parse_tiled_tmj_metadata(metadata_json);
  ASSERT_TRUE(metadata.has_value());

  EXPECT_EQ(metadata->name, "test");
  EXPECT_EQ(metadata->properties.size(), 0);
  EXPECT_EQ(metadata->components.size(), 0);
}

}  // namespace tactile::test
