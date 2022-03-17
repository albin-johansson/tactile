/**
 * This test suite does not perform thorough validation of the resulting IR from parsing,
 * but focuses on the returned error codes. See the round trip test suite for tests
 * related to validating the behavior of the emitters and parsers.
 */

#include <string_view>  // string_view
#include <utility>      // pair

#include <gtest/gtest.h>

#include "io/maps/parser/parse_map.hpp"

#define YAML_DIR "test-resources/yaml/"
#define YAML_ERR "test-resources/yaml/errors/"

#define JSON_DIR "test-resources/json/"
#define JSON_ERR "test-resources/json/errors/"

#define XML_DIR "test-resources/xml/"
#define XML_ERR "test-resources/xml/errors/"

using namespace tactile;

using parsing::ParseError;

namespace {

using test_data = std::pair<std::string_view, ParseError>;

[[nodiscard]] constexpr auto _with(const std::string_view path,
                                   const ParseError expected) -> test_data
{
  return {path, expected};
}

// clang-format off

const auto cases = testing::Values(
  _with("test-resources/dummy.txt", ParseError::unsupported_map_extension),

  _with("foo.yaml", ParseError::map_does_not_exist),
  _with("foo.json", ParseError::map_does_not_exist),
  _with("foo.tmx", ParseError::map_does_not_exist),

  _with(YAML_DIR "minimal.yaml", ParseError::none),
  _with(JSON_DIR "minimal.json", ParseError::none),
  _with(XML_DIR "minimal.xml", ParseError::none),

  _with(YAML_DIR "normal.yaml", ParseError::none),

  _with(YAML_ERR "map/no_map_width.yaml", ParseError::no_map_width),
  _with(JSON_ERR "map/no_map_width.json", ParseError::no_map_width),
  _with(XML_ERR "map/no_map_width.xml", ParseError::no_map_width),

  _with(YAML_ERR "map/no_map_height.yaml", ParseError::no_map_height),
  _with(JSON_ERR "map/no_map_height.json", ParseError::no_map_height),
  _with(XML_ERR "map/no_map_height.xml", ParseError::no_map_height),

  _with(YAML_ERR "map/no_map_tile_width.yaml", ParseError::no_map_tile_width),
  _with(JSON_ERR "map/no_map_tile_width.json", ParseError::no_map_tile_width),
  _with(XML_ERR "map/no_map_tile_width.xml", ParseError::no_map_tile_width),

  _with(YAML_ERR "map/no_map_tile_height.yaml", ParseError::no_map_tile_height),
  _with(JSON_ERR "map/no_map_tile_height.json", ParseError::no_map_tile_height),
  _with(XML_ERR "map/no_map_tile_height.xml", ParseError::no_map_tile_height),

  _with(YAML_ERR "map/no_next_layer_id.yaml", ParseError::no_map_next_layer_id),
  _with(JSON_ERR "map/no_next_layer_id.json", ParseError::no_map_next_layer_id),
  _with(XML_ERR "map/no_next_layer_id.xml", ParseError::no_map_next_layer_id),

  _with(YAML_ERR "map/no_next_object_id.yaml", ParseError::no_map_next_object_id),
  _with(JSON_ERR "map/no_next_object_id.json", ParseError::no_map_next_object_id),
  _with(XML_ERR "map/no_next_object_id.xml", ParseError::no_map_next_object_id),

  _with(YAML_ERR "map/no_tileset_first_tile_id.yaml", ParseError::no_tileset_first_tile_id),
  _with(JSON_ERR "map/no_tileset_first_tile_id.json", ParseError::no_tileset_first_tile_id),

  // YAML only
  _with(YAML_ERR "map/no_external_tileset_path.yaml", ParseError::no_external_tileset_path),

  _with(YAML_ERR "map/external_tileset_does_not_exist.yaml", ParseError::external_tileset_does_not_exist),
  _with(JSON_ERR "map/external_tileset_does_not_exist.json", ParseError::external_tileset_does_not_exist),
  _with(XML_ERR "map/external_tileset_does_not_exist.xml", ParseError::external_tileset_does_not_exist),

  _with(YAML_ERR "map/unknown_external_tileset_error.yaml", ParseError::unknown_external_tileset_error),
  _with(JSON_ERR "map/unknown_external_tileset_error.json", ParseError::unknown_external_tileset_error),
  _with(XML_ERR "map/unknown_external_tileset_error.xml", ParseError::unknown_external_tileset_error),

  _with(YAML_ERR "layer/unsupported_layer_type.yaml", ParseError::unsupported_layer_type),
  _with(JSON_ERR "layer/unsupported_layer_type.json", ParseError::unsupported_layer_type),

  _with(YAML_ERR "layer/no_layer_id.yaml", ParseError::no_layer_id),
  _with(JSON_ERR "layer/no_layer_id.json", ParseError::no_layer_id),
  _with(XML_ERR "layer/no_layer_id.xml", ParseError::no_layer_id),

  _with(YAML_ERR "layer/no_layer_type.yaml", ParseError::no_layer_type),
  _with(JSON_ERR "layer/no_layer_type.json", ParseError::no_layer_type),

  _with(YAML_ERR "layer/no_tile_layer_data.yaml", ParseError::no_tile_layer_data),
  _with(JSON_ERR "layer/no_tile_layer_data.json", ParseError::no_tile_layer_data),
  _with(XML_ERR "layer/no_tile_layer_data.xml", ParseError::no_tile_layer_data),

  _with(YAML_ERR "layer/corrupt_tile_layer_data.yaml", ParseError::corrupt_tile_layer_data),
  _with(JSON_ERR "layer/corrupt_tile_layer_data.json", ParseError::corrupt_tile_layer_data),
  _with(XML_ERR "layer/corrupt_tile_layer_data.xml", ParseError::corrupt_tile_layer_data),

  _with(YAML_ERR "tileset/no_tileset_tile_width.yaml", ParseError::no_tileset_tile_width),
  _with(JSON_ERR "tileset/no_tileset_tile_width.json", ParseError::no_tileset_tile_width),
  _with(XML_ERR "tileset/no_tileset_tile_width.xml", ParseError::no_tileset_tile_width),

  _with(YAML_ERR "tileset/no_tileset_tile_height.yaml", ParseError::no_tileset_tile_height),
  _with(JSON_ERR "tileset/no_tileset_tile_height.json", ParseError::no_tileset_tile_height),
  _with(XML_ERR "tileset/no_tileset_tile_height.xml", ParseError::no_tileset_tile_height),

  _with(YAML_ERR "tileset/no_tileset_name.yaml", ParseError::no_tileset_name),
  _with(JSON_ERR "tileset/no_tileset_name.json", ParseError::no_tileset_name),
  _with(XML_ERR "tileset/no_tileset_name.xml", ParseError::no_tileset_name),

  _with(YAML_ERR "tileset/tileset_image_does_not_exist.yaml", ParseError::tileset_image_does_not_exist),
  _with(JSON_ERR "tileset/tileset_image_does_not_exist.json", ParseError::tileset_image_does_not_exist),
  _with(XML_ERR "tileset/tileset_image_does_not_exist.xml", ParseError::tileset_image_does_not_exist),

  _with(YAML_ERR "tileset/no_tileset_image_path.yaml", ParseError::no_tileset_image_path),
  _with(JSON_ERR "tileset/no_tileset_image_path.json", ParseError::no_tileset_image_path),
  _with(XML_ERR "tileset/no_tileset_image_path.xml", ParseError::no_tileset_image_path),

  _with(YAML_ERR "tileset/no_tileset_image_width.yaml", ParseError::no_tileset_image_width),
  _with(JSON_ERR "tileset/no_tileset_image_width.json", ParseError::no_tileset_image_width),
  _with(XML_ERR "tileset/no_tileset_image_width.xml", ParseError::no_tileset_image_width),

  _with(YAML_ERR "tileset/no_tileset_image_height.yaml", ParseError::no_tileset_image_height),
  _with(JSON_ERR "tileset/no_tileset_image_height.json", ParseError::no_tileset_image_height),
  _with(XML_ERR "tileset/no_tileset_image_height.xml", ParseError::no_tileset_image_height),

  _with(YAML_ERR "tileset/no_tileset_tile_count.yaml", ParseError::no_tileset_tile_count),
  _with(JSON_ERR "tileset/no_tileset_tile_count.json", ParseError::no_tileset_tile_count),
  _with(XML_ERR "tileset/no_tileset_tile_count.xml", ParseError::no_tileset_tile_count),

  _with(YAML_ERR "tileset/no_tileset_column_count.yaml", ParseError::no_tileset_column_count),
  _with(JSON_ERR "tileset/no_tileset_column_count.json", ParseError::no_tileset_column_count),
  _with(XML_ERR "tileset/no_tileset_column_count.xml", ParseError::no_tileset_column_count),

  // YAML only
  _with(YAML_ERR "tileset/no_tileset_version.yaml", ParseError::no_tileset_version),
  _with(YAML_ERR "tileset/unsupported_tileset_version.yaml", ParseError::unsupported_tileset_version),

  _with(YAML_ERR "property/corrupt_property_value.yaml", ParseError::corrupt_property_value),
  _with(JSON_ERR "property/corrupt_property_value.json", ParseError::corrupt_property_value),
  _with(XML_ERR "property/corrupt_property_value.xml", ParseError::corrupt_property_value),

  _with(YAML_ERR "property/unsupported_property_type.yaml", ParseError::unsupported_property_type),
  _with(JSON_ERR "property/unsupported_property_type.json", ParseError::unsupported_property_type),
  _with(XML_ERR "property/unsupported_property_type.xml", ParseError::unsupported_property_type),

  _with(YAML_ERR "property/no_property_name.yaml", ParseError::no_property_name),
  _with(JSON_ERR "property/no_property_name.json", ParseError::no_property_name),
  _with(XML_ERR "property/no_property_name.xml", ParseError::no_property_name),

  _with(YAML_ERR "property/no_property_type.yaml", ParseError::no_property_type),
  _with(JSON_ERR "property/no_property_type.json", ParseError::no_property_type),

  _with(YAML_ERR "object/no_object_id.yaml", ParseError::no_object_id),
  _with(JSON_ERR "object/no_object_id.json", ParseError::no_object_id),
  _with(XML_ERR "object/no_object_id.xml", ParseError::no_object_id),

  // YAML only
  _with(YAML_ERR "object/no_object_type.yaml", ParseError::no_object_type),
  _with(YAML_ERR "object/unsupported_object_type.yaml", ParseError::unsupported_object_type),

  _with(YAML_ERR "fancy-tile/no_fancy_tile_id.yaml", ParseError::no_fancy_tile_id),
  _with(JSON_ERR "fancy-tile/no_fancy_tile_id.json", ParseError::no_fancy_tile_id),
  _with(XML_ERR "fancy-tile/no_fancy_tile_id.xml", ParseError::no_fancy_tile_id),

  _with(YAML_ERR "fancy-tile/no_animation_frame_tile.yaml", ParseError::no_animation_frame_tile),
  _with(JSON_ERR "fancy-tile/no_animation_frame_tile.json", ParseError::no_animation_frame_tile),
  _with(XML_ERR "fancy-tile/no_animation_frame_tile.xml", ParseError::no_animation_frame_tile),

  _with(YAML_ERR "fancy-tile/no_animation_frame_duration.yaml", ParseError::no_animation_frame_duration),
  _with(JSON_ERR "fancy-tile/no_animation_frame_duration.json", ParseError::no_animation_frame_duration),
  _with(XML_ERR "fancy-tile/no_animation_frame_duration.xml", ParseError::no_animation_frame_duration),

  // YAML only
  _with(YAML_ERR "component/no_component_def_name.yaml", ParseError::no_component_def_name),
  _with(YAML_ERR "component/no_component_def_attribute_name.yaml", ParseError::no_component_def_attribute_name),
  _with(YAML_ERR "component/no_component_def_attribute_type.yaml", ParseError::no_component_def_attribute_type),
  _with(YAML_ERR "component/unsupported_component_def_attribute_type.yaml", ParseError::unsupported_component_def_attribute_type),
  _with(YAML_ERR "component/corrupt_component_def_attribute_value.yaml", ParseError::corrupt_component_def_attribute_value),
  _with(YAML_ERR "component/no_component_type.yaml", ParseError::no_component_type),
  _with(YAML_ERR "component/no_component_attribute_name.yaml", ParseError::no_component_attribute_name),
  _with(YAML_ERR "component/no_component_attribute_value.yaml", ParseError::no_component_attribute_value),
  _with(YAML_ERR "component/corrupt_component_attribute_value.yaml", ParseError::corrupt_component_attribute_value)
);

// clang-format on

}  // namespace

struct MapParserTest : testing::TestWithParam<test_data>
{};

TEST_P(MapParserTest, Parsing)
{
  const auto [path, expected] = GetParam();

  const auto result = parsing::parse_map(path);
  ASSERT_EQ(expected, result.error());
}

INSTANTIATE_TEST_SUITE_P(MapParserTests, MapParserTest, cases);