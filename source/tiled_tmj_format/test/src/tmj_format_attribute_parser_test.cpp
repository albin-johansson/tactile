// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_attribute_parser.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace tactile::test {

// tactile::parse_tiled_tmj_property
TEST(TmjFormatAttributeParser, ParsePropertyWithoutName)
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
TEST(TmjFormatAttributeParser, ParsePropertyWithoutType)
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
TEST(TmjFormatAttributeParser, ParsePropertyWithInvalidType)
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
TEST(TmjFormatAttributeParser, ParsePropertyWithoutValue)
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
TEST(TmjFormatAttributeParser, ParseValidString)
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
TEST(TmjFormatAttributeParser, ParseValidInt)
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
TEST(TmjFormatAttributeParser, ParseValidFloat)
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
TEST(TmjFormatAttributeParser, ParseValidBool)
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
TEST(TmjFormatAttributeParser, ParseValidPath)
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

// TODO color

// tactile::parse_tiled_tmj_property
TEST(TmjFormatAttributeParser, ParseValidObject)
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
TEST(TmjFormatAttributeParser, ParseTiledTmjMetadata)
{
  using namespace nlohmann::json_literals;
  using testing::Contains;
  using testing::Eq;

  // TODO color
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
  EXPECT_EQ(metadata->properties.size(), 6);
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
              Contains(Eq(ir::NamedAttribute {"a_path", Attribute {Path {"a/b/c"}}})));
  EXPECT_THAT(metadata->properties,
              Contains(Eq(ir::NamedAttribute {"an_object", Attribute {ObjectRef {123}}})));
}

// tactile::parse_tiled_tmj_metadata
TEST(TmjFormatAttributeParser, ParseTiledTmjMetadataWithName)
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
