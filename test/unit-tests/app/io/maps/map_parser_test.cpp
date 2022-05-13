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
  _with("test-resources/dummy.txt", ParseError::UnsupportedMapExtension),

  _with("foo.yaml", ParseError::MapDoesNotExist),
  _with("foo.json", ParseError::MapDoesNotExist),
  _with("foo.tmx", ParseError::MapDoesNotExist),

  _with(YAML_DIR "minimal.yaml", ParseError::None),
  _with(JSON_DIR "minimal.json", ParseError::None),
  _with(XML_DIR "minimal.xml", ParseError::None),

  _with(YAML_DIR "normal.yaml", ParseError::None),

  _with(YAML_ERR "map/no_map_width.yaml", ParseError::NoMapWidth),
  _with(JSON_ERR "map/no_map_width.json", ParseError::NoMapWidth),
  _with(XML_ERR "map/no_map_width.xml", ParseError::NoMapWidth),

  _with(YAML_ERR "map/no_map_height.yaml", ParseError::NoMapHeight),
  _with(JSON_ERR "map/no_map_height.json", ParseError::NoMapHeight),
  _with(XML_ERR "map/no_map_height.xml", ParseError::NoMapHeight),

  _with(YAML_ERR "map/no_map_tile_width.yaml", ParseError::NoMapTileWidth),
  _with(JSON_ERR "map/no_map_tile_width.json", ParseError::NoMapTileWidth),
  _with(XML_ERR "map/no_map_tile_width.xml", ParseError::NoMapTileWidth),

  _with(YAML_ERR "map/no_map_tile_height.yaml", ParseError::NoMapTileHeight),
  _with(JSON_ERR "map/no_map_tile_height.json", ParseError::NoMapTileHeight),
  _with(XML_ERR "map/no_map_tile_height.xml", ParseError::NoMapTileHeight),

  _with(YAML_ERR "map/no_next_layer_id.yaml", ParseError::NoMapNextLayerId),
  _with(JSON_ERR "map/no_next_layer_id.json", ParseError::NoMapNextLayerId),
  _with(XML_ERR "map/no_next_layer_id.xml", ParseError::NoMapNextLayerId),

  _with(YAML_ERR "map/no_next_object_id.yaml", ParseError::NoMapNextObjectId),
  _with(JSON_ERR "map/no_next_object_id.json", ParseError::NoMapNextObjectId),
  _with(XML_ERR "map/no_next_object_id.xml", ParseError::NoMapNextObjectId),

  _with(YAML_ERR "map/no_tileset_first_tile_id.yaml", ParseError::NoTilesetFirstTileId),
  _with(JSON_ERR "map/no_tileset_first_tile_id.json", ParseError::NoTilesetFirstTileId),

  // YAML only
  _with(YAML_ERR "map/no_external_tileset_path.yaml", ParseError::NoExternalTilesetPath),

  _with(YAML_ERR "map/external_tileset_does_not_exist.yaml", ParseError::ExternalTilesetDoesNotExist),
  _with(JSON_ERR "map/external_tileset_does_not_exist.json", ParseError::ExternalTilesetDoesNotExist),
  _with(XML_ERR "map/external_tileset_does_not_exist.xml", ParseError::ExternalTilesetDoesNotExist),

  _with(YAML_ERR "map/unknown_external_tileset_error.yaml", ParseError::UnknownExternalTilesetError),
  _with(JSON_ERR "map/unknown_external_tileset_error.json", ParseError::UnknownExternalTilesetError),
  _with(XML_ERR "map/unknown_external_tileset_error.xml", ParseError::UnknownExternalTilesetError),

  _with(YAML_ERR "layer/unsupported_layer_type.yaml", ParseError::UnsupportedLayerType),
  _with(JSON_ERR "layer/unsupported_layer_type.json", ParseError::UnsupportedLayerType),

  _with(YAML_ERR "layer/no_layer_id.yaml", ParseError::NoLayerId),
  _with(JSON_ERR "layer/no_layer_id.json", ParseError::NoLayerId),
  _with(XML_ERR "layer/no_layer_id.xml", ParseError::NoLayerId),

  _with(YAML_ERR "layer/no_layer_type.yaml", ParseError::NoLayerType),
  _with(JSON_ERR "layer/no_layer_type.json", ParseError::NoLayerType),

  _with(YAML_ERR "layer/no_tile_layer_data.yaml", ParseError::NoTileLayerData),
  _with(JSON_ERR "layer/no_tile_layer_data.json", ParseError::NoTileLayerData),
  _with(XML_ERR "layer/no_tile_layer_data.xml", ParseError::NoTileLayerData),

  _with(YAML_ERR "layer/corrupt_tile_layer_data.yaml", ParseError::CorruptTileLayerData),
  _with(JSON_ERR "layer/corrupt_tile_layer_data.json", ParseError::CorruptTileLayerData),
  _with(XML_ERR "layer/corrupt_tile_layer_data.xml", ParseError::CorruptTileLayerData),

  _with(YAML_ERR "tileset/no_tileset_tile_width.yaml", ParseError::NoTilesetTileWidth),
  _with(JSON_ERR "tileset/no_tileset_tile_width.json", ParseError::NoTilesetTileWidth),
  _with(XML_ERR "tileset/no_tileset_tile_width.xml", ParseError::NoTilesetTileWidth),

  _with(YAML_ERR "tileset/no_tileset_tile_height.yaml", ParseError::NoTilesetTileHeight),
  _with(JSON_ERR "tileset/no_tileset_tile_height.json", ParseError::NoTilesetTileHeight),
  _with(XML_ERR "tileset/no_tileset_tile_height.xml", ParseError::NoTilesetTileHeight),

  _with(YAML_ERR "tileset/no_tileset_name.yaml", ParseError::NoTilesetName),
  _with(JSON_ERR "tileset/no_tileset_name.json", ParseError::NoTilesetName),
  _with(XML_ERR "tileset/no_tileset_name.xml", ParseError::NoTilesetName),

  _with(YAML_ERR "tileset/tileset_image_does_not_exist.yaml", ParseError::TilesetImageDoesNotExist),
  _with(JSON_ERR "tileset/tileset_image_does_not_exist.json", ParseError::TilesetImageDoesNotExist),
  _with(XML_ERR "tileset/tileset_image_does_not_exist.xml", ParseError::TilesetImageDoesNotExist),

  _with(YAML_ERR "tileset/no_tileset_image_path.yaml", ParseError::NoTilesetImagePath),
  _with(JSON_ERR "tileset/no_tileset_image_path.json", ParseError::NoTilesetImagePath),
  _with(XML_ERR "tileset/no_tileset_image_path.xml", ParseError::NoTilesetImagePath),

  _with(YAML_ERR "tileset/no_tileset_image_width.yaml", ParseError::NoTilesetImageWidth),
  _with(JSON_ERR "tileset/no_tileset_image_width.json", ParseError::NoTilesetImageWidth),
  _with(XML_ERR "tileset/no_tileset_image_width.xml", ParseError::NoTilesetImageWidth),

  _with(YAML_ERR "tileset/no_tileset_image_height.yaml", ParseError::NoTilesetImageHeight),
  _with(JSON_ERR "tileset/no_tileset_image_height.json", ParseError::NoTilesetImageHeight),
  _with(XML_ERR "tileset/no_tileset_image_height.xml", ParseError::NoTilesetImageHeight),

  _with(YAML_ERR "tileset/no_tileset_tile_count.yaml", ParseError::NoTilesetTileCount),
  _with(JSON_ERR "tileset/no_tileset_tile_count.json", ParseError::NoTilesetTileCount),
  _with(XML_ERR "tileset/no_tileset_tile_count.xml", ParseError::NoTilesetTileCount),

  _with(YAML_ERR "tileset/no_tileset_column_count.yaml", ParseError::NoTilesetColumnCount),
  _with(JSON_ERR "tileset/no_tileset_column_count.json", ParseError::NoTilesetColumnCount),
  _with(XML_ERR "tileset/no_tileset_column_count.xml", ParseError::NoTilesetColumnCount),

  // YAML only
  _with(YAML_ERR "tileset/no_tileset_version.yaml", ParseError::NoTilesetVersion),
  _with(YAML_ERR "tileset/unsupported_tileset_version.yaml", ParseError::UnsupportedTilesetVersion),

  _with(YAML_ERR "property/corrupt_property_value.yaml", ParseError::CorruptPropertyValue),
  _with(JSON_ERR "property/corrupt_property_value.json", ParseError::CorruptPropertyValue),
  _with(XML_ERR "property/corrupt_property_value.xml", ParseError::CorruptPropertyValue),

  _with(YAML_ERR "property/unsupported_property_type.yaml", ParseError::UnsupportedPropertyType),
  _with(JSON_ERR "property/unsupported_property_type.json", ParseError::UnsupportedPropertyType),
  _with(XML_ERR "property/unsupported_property_type.xml", ParseError::UnsupportedPropertyType),

  _with(YAML_ERR "property/no_property_name.yaml", ParseError::NoPropertyName),
  _with(JSON_ERR "property/no_property_name.json", ParseError::NoPropertyName),
  _with(XML_ERR "property/no_property_name.xml", ParseError::NoPropertyName),

  _with(YAML_ERR "property/no_property_type.yaml", ParseError::NoPropertyType),
  _with(JSON_ERR "property/no_property_type.json", ParseError::NoPropertyType),

  _with(YAML_ERR "object/no_object_id.yaml", ParseError::NoObjectId),
  _with(JSON_ERR "object/no_object_id.json", ParseError::NoObjectId),
  _with(XML_ERR "object/no_object_id.xml", ParseError::NoObjectId),

  // YAML only
  _with(YAML_ERR "object/no_object_type.yaml", ParseError::NoObjectType),
  _with(YAML_ERR "object/unsupported_object_type.yaml", ParseError::UnsupportedObjectType),

  _with(YAML_ERR "fancy-tile/no_fancy_tile_id.yaml", ParseError::NoFancyTileId),
  _with(JSON_ERR "fancy-tile/no_fancy_tile_id.json", ParseError::NoFancyTileId),
  _with(XML_ERR "fancy-tile/no_fancy_tile_id.xml", ParseError::NoFancyTileId),

  _with(YAML_ERR "fancy-tile/no_animation_frame_tile.yaml", ParseError::NoAnimationFrameTile),
  _with(JSON_ERR "fancy-tile/no_animation_frame_tile.json", ParseError::NoAnimationFrameTile),
  _with(XML_ERR "fancy-tile/no_animation_frame_tile.xml", ParseError::NoAnimationFrameTile),

  _with(YAML_ERR "fancy-tile/no_animation_frame_duration.yaml", ParseError::NoAnimationFrameDuration),
  _with(JSON_ERR "fancy-tile/no_animation_frame_duration.json", ParseError::NoAnimationFrameDuration),
  _with(XML_ERR "fancy-tile/no_animation_frame_duration.xml", ParseError::NoAnimationFrameDuration),

  // YAML only
  _with(YAML_ERR "component/no_component_def_name.yaml", ParseError::NoComponentDefName),
  _with(YAML_ERR "component/no_component_def_attribute_name.yaml", ParseError::NoComponentDefAttributeName),
  _with(YAML_ERR "component/no_component_def_attribute_type.yaml", ParseError::NoComponentDefAttributeType),
  _with(YAML_ERR "component/unsupported_component_def_attribute_type.yaml", ParseError::UnsupportedComponentDefAttributeType),
  _with(YAML_ERR "component/corrupt_component_def_attribute_value.yaml", ParseError::CorruptComponentDefAttributeValue),
  _with(YAML_ERR "component/no_component_type.yaml", ParseError::NoComponentType),
  _with(YAML_ERR "component/no_component_attribute_name.yaml", ParseError::NoComponentAttributeName),
  _with(YAML_ERR "component/no_component_attribute_value.yaml", ParseError::NoComponentAttributeValue),
  _with(YAML_ERR "component/corrupt_component_attribute_value.yaml", ParseError::CorruptComponentAttributeValue)
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