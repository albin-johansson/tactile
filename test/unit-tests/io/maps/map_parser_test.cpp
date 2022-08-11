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

/**
 * This test suite does not perform thorough validation of the resulting IR from parsing,
 * but focuses on the returned error codes. See the round trip test suite for tests
 * related to validating the behavior of the emitters and parsers.
 */

#include <string_view>  // string_view
#include <utility>      // pair

#include <gtest/gtest.h>

#include "io/map/parse/parse_map.hpp"

#define YAML_DIR "test-resources/yaml/"
#define YAML_ERR "test-resources/yaml/errors/"

#define JSON_DIR "test-resources/json/"
#define JSON_ERR "test-resources/json/errors/"

#define XML_DIR "test-resources/xml/"
#define XML_ERR "test-resources/xml/errors/"

namespace tactile::test {

using PE = io::ParseError;
using TestData = std::pair<std::string_view, PE>;

namespace {

[[nodiscard]] constexpr auto with(const std::string_view path, const PE expected)
    -> TestData
{
  return {path, expected};
}

// clang-format off

const auto cases = testing::Values(
  with("test-resources/dummy.txt", PE::UnsupportedMapExtension),

  with("foo.yaml", PE::MapDoesNotExist),
  with("foo.json", PE::MapDoesNotExist),
  with("foo.tmx", PE::MapDoesNotExist),

  with(YAML_DIR "minimal.yaml", PE::None),
  with(JSON_DIR "minimal.json", PE::None),
  with(XML_DIR "minimal.xml", PE::None),

  with(YAML_DIR "normal.yaml", PE::None),

  with(YAML_ERR "map/no_map_width.yaml", PE::NoMapWidth),
  with(JSON_ERR "map/no_map_width.json", PE::NoMapWidth),
  with(XML_ERR "map/no_map_width.xml", PE::NoMapWidth),

  with(YAML_ERR "map/no_map_height.yaml", PE::NoMapHeight),
  with(JSON_ERR "map/no_map_height.json", PE::NoMapHeight),
  with(XML_ERR "map/no_map_height.xml", PE::NoMapHeight),

  with(YAML_ERR "map/no_map_tile_width.yaml", PE::NoMapTileWidth),
  with(JSON_ERR "map/no_map_tile_width.json", PE::NoMapTileWidth),
  with(XML_ERR "map/no_map_tile_width.xml", PE::NoMapTileWidth),

  with(YAML_ERR "map/no_map_tile_height.yaml", PE::NoMapTileHeight),
  with(JSON_ERR "map/no_map_tile_height.json", PE::NoMapTileHeight),
  with(XML_ERR "map/no_map_tile_height.xml", PE::NoMapTileHeight),

  with(YAML_ERR "map/no_next_layer_id.yaml", PE::NoMapNextLayerId),
  with(JSON_ERR "map/no_next_layer_id.json", PE::NoMapNextLayerId),
  with(XML_ERR "map/no_next_layer_id.xml", PE::NoMapNextLayerId),

  with(YAML_ERR "map/no_next_object_id.yaml", PE::NoMapNextObjectId),
  with(JSON_ERR "map/no_next_object_id.json", PE::NoMapNextObjectId),
  with(XML_ERR "map/no_next_object_id.xml", PE::NoMapNextObjectId),

  with(YAML_ERR "map/no_tileset_first_tile_id.yaml", PE::NoTilesetFirstTileId),
  with(JSON_ERR "map/no_tileset_first_tile_id.json", PE::NoTilesetFirstTileId),

  // YAML only
  with(YAML_ERR "map/no_external_tileset_path.yaml", PE::NoExternalTilesetPath),

  with(YAML_ERR "map/external_tileset_does_not_exist.yaml", PE::ExternalTilesetDoesNotExist),
  with(JSON_ERR "map/external_tileset_does_not_exist.json", PE::ExternalTilesetDoesNotExist),
  with(XML_ERR "map/external_tileset_does_not_exist.xml", PE::ExternalTilesetDoesNotExist),

  with(YAML_ERR "map/unknown_external_tileset_error.yaml", PE::UnknownExternalTilesetError),
  with(JSON_ERR "map/unknown_external_tileset_error.json", PE::UnknownExternalTilesetError),
  with(XML_ERR "map/unknown_external_tileset_error.xml", PE::UnknownExternalTilesetError),

  with(YAML_ERR "layer/unsupported_layer_type.yaml", PE::UnsupportedLayerType),
  with(JSON_ERR "layer/unsupported_layer_type.json", PE::UnsupportedLayerType),

  with(YAML_ERR "layer/no_layer_id.yaml", PE::NoLayerId),
  with(JSON_ERR "layer/no_layer_id.json", PE::NoLayerId),
  with(XML_ERR "layer/no_layer_id.xml", PE::NoLayerId),

  with(YAML_ERR "layer/no_layer_type.yaml", PE::NoLayerType),
  with(JSON_ERR "layer/no_layer_type.json", PE::NoLayerType),

  with(YAML_ERR "layer/no_tile_layer_data.yaml", PE::NoTileLayerData),
  with(JSON_ERR "layer/no_tile_layer_data.json", PE::NoTileLayerData),
  with(XML_ERR "layer/no_tile_layer_data.xml", PE::NoTileLayerData),

  with(YAML_ERR "layer/corrupt_tile_layer_data.yaml", PE::CorruptTileLayerData),
  with(JSON_ERR "layer/corrupt_tile_layer_data.json", PE::CorruptTileLayerData),
  with(XML_ERR "layer/corrupt_tile_layer_data.xml", PE::CorruptTileLayerData),

  with(YAML_ERR "tileset/no_tileset_tile_width.yaml", PE::NoTilesetTileWidth),
  with(JSON_ERR "tileset/no_tileset_tile_width.json", PE::NoTilesetTileWidth),
  with(XML_ERR "tileset/no_tileset_tile_width.xml", PE::NoTilesetTileWidth),

  with(YAML_ERR "tileset/no_tileset_tile_height.yaml", PE::NoTilesetTileHeight),
  with(JSON_ERR "tileset/no_tileset_tile_height.json", PE::NoTilesetTileHeight),
  with(XML_ERR "tileset/no_tileset_tile_height.xml", PE::NoTilesetTileHeight),

  with(YAML_ERR "tileset/no_tileset_name.yaml", PE::NoTilesetName),
  with(JSON_ERR "tileset/no_tileset_name.json", PE::NoTilesetName),
  with(XML_ERR "tileset/no_tileset_name.xml", PE::NoTilesetName),

  with(YAML_ERR "tileset/tileset_image_does_not_exist.yaml", PE::TilesetImageDoesNotExist),
  with(JSON_ERR "tileset/tileset_image_does_not_exist.json", PE::TilesetImageDoesNotExist),
  with(XML_ERR "tileset/tileset_image_does_not_exist.xml", PE::TilesetImageDoesNotExist),

  with(YAML_ERR "tileset/no_tileset_image_path.yaml", PE::NoTilesetImagePath),
  with(JSON_ERR "tileset/no_tileset_image_path.json", PE::NoTilesetImagePath),
  with(XML_ERR "tileset/no_tileset_image_path.xml", PE::NoTilesetImagePath),

  with(YAML_ERR "tileset/no_tileset_image_width.yaml", PE::NoTilesetImageWidth),
  with(JSON_ERR "tileset/no_tileset_image_width.json", PE::NoTilesetImageWidth),
  with(XML_ERR "tileset/no_tileset_image_width.xml", PE::NoTilesetImageWidth),

  with(YAML_ERR "tileset/no_tileset_image_height.yaml", PE::NoTilesetImageHeight),
  with(JSON_ERR "tileset/no_tileset_image_height.json", PE::NoTilesetImageHeight),
  with(XML_ERR "tileset/no_tileset_image_height.xml", PE::NoTilesetImageHeight),

  with(YAML_ERR "tileset/no_tileset_tile_count.yaml", PE::NoTilesetTileCount),
  with(JSON_ERR "tileset/no_tileset_tile_count.json", PE::NoTilesetTileCount),
  with(XML_ERR "tileset/no_tileset_tile_count.xml", PE::NoTilesetTileCount),

  with(YAML_ERR "tileset/no_tileset_column_count.yaml", PE::NoTilesetColumnCount),
  with(JSON_ERR "tileset/no_tileset_column_count.json", PE::NoTilesetColumnCount),
  with(XML_ERR "tileset/no_tileset_column_count.xml", PE::NoTilesetColumnCount),

  // YAML only
  with(YAML_ERR "tileset/no_tileset_version.yaml", PE::NoTilesetVersion),
  with(YAML_ERR "tileset/unsupported_tileset_version.yaml", PE::UnsupportedTilesetVersion),

  with(YAML_ERR "property/corrupt_property_value.yaml", PE::CorruptPropertyValue),
  with(JSON_ERR "property/corrupt_property_value.json", PE::CorruptPropertyValue),
  with(XML_ERR "property/corrupt_property_value.xml", PE::CorruptPropertyValue),

  with(YAML_ERR "property/unsupported_property_type.yaml", PE::UnsupportedPropertyType),
  with(JSON_ERR "property/unsupported_property_type.json", PE::UnsupportedPropertyType),
  with(XML_ERR "property/unsupported_property_type.xml", PE::UnsupportedPropertyType),

  with(YAML_ERR "property/no_property_name.yaml", PE::NoPropertyName),
  with(JSON_ERR "property/no_property_name.json", PE::NoPropertyName),
  with(XML_ERR "property/no_property_name.xml", PE::NoPropertyName),

  with(YAML_ERR "property/no_property_type.yaml", PE::NoPropertyType),
  with(JSON_ERR "property/no_property_type.json", PE::NoPropertyType),

  with(YAML_ERR "object/no_object_id.yaml", PE::NoObjectId),
  with(JSON_ERR "object/no_object_id.json", PE::NoObjectId),
  with(XML_ERR "object/no_object_id.xml", PE::NoObjectId),

  // YAML only
  with(YAML_ERR "object/no_object_type.yaml", PE::NoObjectType),
  with(YAML_ERR "object/unsupported_object_type.yaml", PE::UnsupportedObjectType),

  with(YAML_ERR "fancy-tile/no_fancy_tile_id.yaml", PE::NoFancyTileId),
  with(JSON_ERR "fancy-tile/no_fancy_tile_id.json", PE::NoFancyTileId),
  with(XML_ERR "fancy-tile/no_fancy_tile_id.xml", PE::NoFancyTileId),

  with(YAML_ERR "fancy-tile/no_animation_frame_tile.yaml", PE::NoAnimationFrameTile),
  with(JSON_ERR "fancy-tile/no_animation_frame_tile.json", PE::NoAnimationFrameTile),
  with(XML_ERR "fancy-tile/no_animation_frame_tile.xml", PE::NoAnimationFrameTile),

  with(YAML_ERR "fancy-tile/no_animation_frame_duration.yaml", PE::NoAnimationFrameDuration),
  with(JSON_ERR "fancy-tile/no_animation_frame_duration.json", PE::NoAnimationFrameDuration),
  with(XML_ERR "fancy-tile/no_animation_frame_duration.xml", PE::NoAnimationFrameDuration),

  // YAML only
  with(YAML_ERR "component/no_component_def_name.yaml", PE::NoComponentDefName),
  with(YAML_ERR "component/no_component_def_attribute_name.yaml", PE::NoComponentDefAttributeName),
  with(YAML_ERR "component/no_component_def_attribute_type.yaml", PE::NoComponentDefAttributeType),
  with(YAML_ERR "component/unsupported_component_def_attribute_type.yaml", PE::UnsupportedComponentDefAttributeType),
  with(YAML_ERR "component/corrupt_component_def_attribute_value.yaml", PE::CorruptComponentDefAttributeValue),
  with(YAML_ERR "component/no_component_type.yaml", PE::NoComponentType),
  with(YAML_ERR "component/no_component_attribute_name.yaml", PE::NoComponentAttributeName),
  with(YAML_ERR "component/no_component_attribute_value.yaml", PE::NoComponentAttributeValue),
  with(YAML_ERR "component/corrupt_component_attribute_value.yaml", PE::CorruptComponentAttributeValue),

  with(YAML_ERR "tile-format/bad_tile_format_encoding.yaml", PE::BadTileFormatEncoding),
  with(YAML_ERR "tile-format/bad_tile_format_compression.yaml", PE::BadTileFormatCompression),
  with(YAML_ERR "tile-format/bad_tile_format_endianness.yaml", PE::BadTileFormatEndianness),
  with(YAML_ERR "tile-format/bad_zlib_compression_level.yaml", PE::BadZlibCompressionLevel),
  with(YAML_ERR "tile-format/plain_encoding_with_compression.yaml", PE::PlainEncodingWithCompression)
);

// clang-format on

}  // namespace

struct MapParserTest : testing::TestWithParam<TestData>
{};

TEST_P(MapParserTest, Parsing)
{
  const auto [path, expected] = GetParam();

  const auto result = io::parse_map(path);
  ASSERT_EQ(expected, result.error());
}

INSTANTIATE_TEST_SUITE_P(MapParserTests, MapParserTest, cases);

}  // namespace tactile::test
