/**
 * This test suite does not perform thorough validation of the resulting IR from parsing,
 * but focuses on the returned error codes. See the round trip test suite for tests
 * related to validating the behavior of the emitters and parsers.
 */

#include "io/maps/parser/map_parser.hpp"

#include <string_view>  // string_view
#include <utility>      // pair

#include <gtest/gtest.h>

#define YAML_DIR "test-resources/yaml/"
#define YAML_ERR "test-resources/yaml/errors/"

#define JSON_DIR "test-resources/json/"
#define JSON_ERR "test-resources/json/errors/"

#define XML_DIR "test-resources/xml/"
#define XML_ERR "test-resources/xml/errors/"

using namespace tactile;

using parsing::parse_error;

namespace {

using test_data = std::pair<std::string_view, parse_error>;

[[nodiscard]] constexpr auto _with(const std::string_view path,
                                   const parse_error expected) -> test_data
{
  return {path, expected};
}

// clang-format off

const auto cases = testing::Values(
  _with("test-resources/dummy.txt", parse_error::unsupported_map_extension),

  _with("foo.yaml", parse_error::map_does_not_exist),
  _with("foo.json", parse_error::map_does_not_exist),
  _with("foo.tmx", parse_error::map_does_not_exist),

  _with(YAML_DIR "minimal.yaml", parse_error::none),
  _with(JSON_DIR "minimal.json", parse_error::none),

  _with(YAML_DIR "normal.yaml", parse_error::none),

  _with(YAML_ERR "map/no_map_width.yaml", parse_error::no_map_width),
  _with(JSON_ERR "map/no_map_width.json", parse_error::no_map_width),

  _with(YAML_ERR "map/no_map_height.yaml", parse_error::no_map_height),
  _with(JSON_ERR "map/no_map_height.json", parse_error::no_map_height),

  _with(YAML_ERR "map/no_map_tile_width.yaml", parse_error::no_map_tile_width),
  _with(JSON_ERR "map/no_map_tile_width.json", parse_error::no_map_tile_width),

  _with(YAML_ERR "map/no_map_tile_height.yaml", parse_error::no_map_tile_height),
  _with(JSON_ERR "map/no_map_tile_height.json", parse_error::no_map_tile_height),

  _with(YAML_ERR "map/no_next_layer_id.yaml", parse_error::no_map_next_layer_id),
  _with(JSON_ERR "map/no_next_layer_id.json", parse_error::no_map_next_layer_id),

  _with(YAML_ERR "map/no_next_object_id.yaml", parse_error::no_map_next_object_id),
  _with(JSON_ERR "map/no_next_object_id.json", parse_error::no_map_next_object_id),

  _with(YAML_ERR "map/no_tileset_first_tile_id.yaml", parse_error::no_tileset_first_tile_id),
  _with(JSON_ERR "map/no_tileset_first_tile_id.json", parse_error::no_tileset_first_tile_id),

  // YAML only
  _with(YAML_ERR "map/no_external_tileset_path.yaml", parse_error::no_external_tileset_path),

  _with(YAML_ERR "map/external_tileset_does_not_exist.yaml", parse_error::external_tileset_does_not_exist),
  _with(JSON_ERR "map/external_tileset_does_not_exist.json", parse_error::external_tileset_does_not_exist),

  _with(YAML_ERR "map/unknown_external_tileset_error.yaml", parse_error::unknown_external_tileset_error),
  _with(JSON_ERR "map/unknown_external_tileset_error.json", parse_error::unknown_external_tileset_error),

  _with(YAML_ERR "layer/unsupported_layer_type.yaml", parse_error::unsupported_layer_type),
  _with(JSON_ERR "layer/unsupported_layer_type.json", parse_error::unsupported_layer_type),

  _with(YAML_ERR "layer/no_layer_id.yaml", parse_error::no_layer_id),
  _with(JSON_ERR "layer/no_layer_id.json", parse_error::no_layer_id),

  _with(YAML_ERR "layer/no_layer_type.yaml", parse_error::no_layer_type),
  _with(JSON_ERR "layer/no_layer_type.json", parse_error::no_layer_type),

  _with(YAML_ERR "layer/no_tile_layer_data.yaml", parse_error::no_tile_layer_data),
  _with(JSON_ERR "layer/no_tile_layer_data.json", parse_error::no_tile_layer_data),

  _with(YAML_ERR "layer/corrupt_tile_layer_data.yaml", parse_error::corrupt_tile_layer_data),
  _with(JSON_ERR "layer/corrupt_tile_layer_data.json", parse_error::corrupt_tile_layer_data),

  _with(YAML_ERR "tileset/no_tileset_tile_width.yaml", parse_error::no_tileset_tile_width),
  _with(JSON_ERR "tileset/no_tileset_tile_width.json", parse_error::no_tileset_tile_width),

  _with(YAML_ERR "tileset/no_tileset_tile_height.yaml", parse_error::no_tileset_tile_height),
  _with(JSON_ERR "tileset/no_tileset_tile_height.json", parse_error::no_tileset_tile_height),

  _with(YAML_ERR "tileset/no_tileset_name.yaml", parse_error::no_tileset_name),
  _with(JSON_ERR "tileset/no_tileset_name.json", parse_error::no_tileset_name),

  _with(YAML_ERR "tileset/tileset_image_does_not_exist.yaml", parse_error::tileset_image_does_not_exist),
  _with(JSON_ERR "tileset/tileset_image_does_not_exist.json", parse_error::tileset_image_does_not_exist),

  _with(YAML_ERR "tileset/no_tileset_image_path.yaml", parse_error::no_tileset_image_path),
  _with(JSON_ERR "tileset/no_tileset_image_path.json", parse_error::no_tileset_image_path),

  _with(YAML_ERR "tileset/no_tileset_image_width.yaml", parse_error::no_tileset_image_width),
  _with(JSON_ERR "tileset/no_tileset_image_width.json", parse_error::no_tileset_image_width),

  _with(YAML_ERR "tileset/no_tileset_image_height.yaml", parse_error::no_tileset_image_height),
  _with(JSON_ERR "tileset/no_tileset_image_height.json", parse_error::no_tileset_image_height),

  _with(YAML_ERR "tileset/no_tileset_tile_count.yaml", parse_error::no_tileset_tile_count),
  _with(JSON_ERR "tileset/no_tileset_tile_count.json", parse_error::no_tileset_tile_count),

  _with(YAML_ERR "tileset/no_tileset_column_count.yaml", parse_error::no_tileset_column_count),
  _with(JSON_ERR "tileset/no_tileset_column_count.json", parse_error::no_tileset_column_count),

  _with(YAML_ERR "tileset/no_tileset_version.yaml", parse_error::no_tileset_version),
  _with(YAML_ERR "tileset/unsupported_tileset_version.yaml", parse_error::unsupported_tileset_version),

  _with(YAML_ERR "property/corrupt_property_value.yaml", parse_error::corrupt_property_value),
  _with(JSON_ERR "property/corrupt_property_value.json", parse_error::corrupt_property_value),

  _with(YAML_ERR "property/unsupported_property_type.yaml", parse_error::unsupported_property_type),
  _with(JSON_ERR "property/unsupported_property_type.json", parse_error::unsupported_property_type),

  _with(YAML_ERR "property/no_property_name.yaml", parse_error::no_property_name),
  _with(JSON_ERR "property/no_property_name.json", parse_error::no_property_name),

  _with(YAML_ERR "property/no_property_type.yaml", parse_error::no_property_type),
  _with(JSON_ERR "property/no_property_type.json", parse_error::no_property_type),

  _with(YAML_ERR "object/no_object_id.yaml", parse_error::no_object_id),
  _with(JSON_ERR "object/no_object_id.json", parse_error::no_object_id),

  // YAML only
  _with(YAML_ERR "object/no_object_type.yaml", parse_error::no_object_type),
  _with(YAML_ERR "object/unsupported_object_type.yaml", parse_error::unsupported_object_type),

  _with(YAML_ERR "fancy-tile/no_fancy_tile_id.yaml", parse_error::no_fancy_tile_id),
  _with(JSON_ERR "fancy-tile/no_fancy_tile_id.json", parse_error::no_fancy_tile_id),

  _with(YAML_ERR "fancy-tile/no_animation_frame_tile.yaml", parse_error::no_animation_frame_tile),
  _with(JSON_ERR "fancy-tile/no_animation_frame_tile.json", parse_error::no_animation_frame_tile),

  _with(YAML_ERR "fancy-tile/no_animation_frame_duration.yaml", parse_error::no_animation_frame_duration),
  _with(JSON_ERR "fancy-tile/no_animation_frame_duration.json", parse_error::no_animation_frame_duration),

  // YAML only
  _with(YAML_ERR "component/no_component_def_name.yaml", parse_error::no_component_def_name),
  _with(YAML_ERR "component/no_component_def_attribute_name.yaml", parse_error::no_component_def_attribute_name),
  _with(YAML_ERR "component/no_component_def_attribute_type.yaml", parse_error::no_component_def_attribute_type),
  _with(YAML_ERR "component/unsupported_component_def_attribute_type.yaml", parse_error::unsupported_component_def_attribute_type),
  _with(YAML_ERR "component/corrupt_component_def_attribute_value.yaml", parse_error::corrupt_component_def_attribute_value),
  _with(YAML_ERR "component/no_component_type.yaml", parse_error::no_component_type),
  _with(YAML_ERR "component/no_component_attribute_name.yaml", parse_error::no_component_attribute_name),
  _with(YAML_ERR "component/no_component_attribute_value.yaml", parse_error::no_component_attribute_value),
  _with(YAML_ERR "component/corrupt_component_attribute_value.yaml", parse_error::corrupt_component_attribute_value),

  _with("test-resources/dummy.txt", parse_error::unsupported_map_extension) // TODO remove
);

// clang-format on

}  // namespace

struct MapParserTest : testing::TestWithParam<test_data>
{};

TEST_P(MapParserTest, Parsing)
{
  const auto [path, expected] = GetParam();

  parsing::map_parser parser;
  parser.parse_map(path);

  ASSERT_EQ(expected, parser.error());
}

INSTANTIATE_TEST_SUITE_P(MapParserTests, MapParserTest, cases);