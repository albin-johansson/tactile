/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <doctest/doctest.h>

#include "io/map/parse/parse_map.hpp"

#define YAML_DIR "resources/yaml/"
#define YAML_ERR "resources/yaml/errors/"

#define JSON_DIR "resources/json/"
#define JSON_ERR "resources/json/errors/"

#define XML_DIR "resources/xml/"
#define XML_ERR "resources/xml/errors/"

namespace tactile::test {
namespace {

using PE = ParseError;

void _parse_and_check_error(const char* path, const ParseError expected_error)
{
  const auto parse_result = parse_map(path);
  if (expected_error == ParseError::None) {
    CHECK(parse_result.has_value());
  }
  else {
    CHECK(parse_result.error() == expected_error);
  }
}

}  // namespace

// This test suite does not perform thorough validation of the resulting IR from parsing,
// but focuses on the returned error codes. See the round trip test suite for tests
// related to validating the behavior of the emitters and parsers.
TEST_SUITE("Map parser")
{
  TEST_CASE("Valid maps")
  {
    _parse_and_check_error(YAML_DIR "minimal.yaml", PE::None);
    _parse_and_check_error(JSON_DIR "minimal.json", PE::None);
    _parse_and_check_error(XML_DIR "minimal.xml", PE::None);

    _parse_and_check_error(YAML_DIR "normal.yaml", PE::None);
  }

  TEST_CASE("Invalid file types and missing maps")
  {
    _parse_and_check_error("resources/dummy.txt", PE::UnsupportedMapExtension);
    _parse_and_check_error("foo.yaml", PE::MapDoesNotExist);
    _parse_and_check_error("foo.json", PE::MapDoesNotExist);
    _parse_and_check_error("foo.tmx", PE::MapDoesNotExist);
  }

  TEST_CASE("No map width")
  {
    _parse_and_check_error(YAML_ERR "map/no_map_width.yaml", PE::NoMapWidth);
    _parse_and_check_error(JSON_ERR "map/no_map_width.json", PE::NoMapWidth);
    _parse_and_check_error(XML_ERR "map/no_map_width.xml", PE::NoMapWidth);
  }

  TEST_CASE("No map height")
  {
    _parse_and_check_error(YAML_ERR "map/no_map_height.yaml", PE::NoMapHeight);
    _parse_and_check_error(JSON_ERR "map/no_map_height.json", PE::NoMapHeight);
    _parse_and_check_error(XML_ERR "map/no_map_height.xml", PE::NoMapHeight);
  }

  TEST_CASE("No tile width")
  {
    _parse_and_check_error(YAML_ERR "map/no_map_tile_width.yaml", PE::NoMapTileWidth);
    _parse_and_check_error(JSON_ERR "map/no_map_tile_width.json", PE::NoMapTileWidth);
    _parse_and_check_error(XML_ERR "map/no_map_tile_width.xml", PE::NoMapTileWidth);
  }

  TEST_CASE("No tile height")
  {
    _parse_and_check_error(YAML_ERR "map/no_map_tile_height.yaml", PE::NoMapTileHeight);
    _parse_and_check_error(JSON_ERR "map/no_map_tile_height.json", PE::NoMapTileHeight);
    _parse_and_check_error(XML_ERR "map/no_map_tile_height.xml", PE::NoMapTileHeight);
  }

  TEST_CASE("No next layer ID")
  {
    _parse_and_check_error(YAML_ERR "map/no_next_layer_id.yaml", PE::NoMapNextLayerId);
    _parse_and_check_error(JSON_ERR "map/no_next_layer_id.json", PE::NoMapNextLayerId);
    _parse_and_check_error(XML_ERR "map/no_next_layer_id.xml", PE::NoMapNextLayerId);
  }

  TEST_CASE("No next object ID")
  {
    _parse_and_check_error(YAML_ERR "map/no_next_object_id.yaml", PE::NoMapNextObjectId);
    _parse_and_check_error(JSON_ERR "map/no_next_object_id.json", PE::NoMapNextObjectId);
    _parse_and_check_error(XML_ERR "map/no_next_object_id.xml", PE::NoMapNextObjectId);
  }

  TEST_CASE("No tileset first tile ID")
  {
    _parse_and_check_error(YAML_ERR "map/no_tileset_first_tile_id.yaml",
                           PE::NoTilesetFirstTileId);
    _parse_and_check_error(JSON_ERR "map/no_tileset_first_tile_id.json",
                           PE::NoTilesetFirstTileId);
  }

  TEST_CASE("External tileset does not exist")
  {
    _parse_and_check_error(YAML_ERR "map/external_tileset_does_not_exist.yaml",
                           PE::ExternalTilesetDoesNotExist);
    _parse_and_check_error(JSON_ERR "map/external_tileset_does_not_exist.json",
                           PE::ExternalTilesetDoesNotExist);
    _parse_and_check_error(XML_ERR "map/external_tileset_does_not_exist.xml",
                           PE::ExternalTilesetDoesNotExist);
  }

  TEST_CASE("Unknown external tileset error")
  {
    _parse_and_check_error(YAML_ERR "map/unknown_external_tileset_error.yaml",
                           PE::UnknownExternalTilesetError);
    _parse_and_check_error(JSON_ERR "map/unknown_external_tileset_error.json",
                           PE::UnknownExternalTilesetError);
    _parse_and_check_error(XML_ERR "map/unknown_external_tileset_error.xml",
                           PE::UnknownExternalTilesetError);
  }

  TEST_CASE("Unsupported layer type")
  {
    _parse_and_check_error(YAML_ERR "layer/unsupported_layer_type.yaml",
                           PE::UnsupportedLayerType);
    _parse_and_check_error(JSON_ERR "layer/unsupported_layer_type.json",
                           PE::UnsupportedLayerType);
  }

  TEST_CASE("No layer ID")
  {
    _parse_and_check_error(YAML_ERR "layer/no_layer_id.yaml", PE::NoLayerId);
    _parse_and_check_error(JSON_ERR "layer/no_layer_id.json", PE::NoLayerId);
    _parse_and_check_error(XML_ERR "layer/no_layer_id.xml", PE::NoLayerId);
  }

  TEST_CASE("No layer type")
  {
    _parse_and_check_error(YAML_ERR "layer/no_layer_type.yaml", PE::NoLayerType);
    _parse_and_check_error(JSON_ERR "layer/no_layer_type.json", PE::NoLayerType);
  }

  TEST_CASE("No tile layer data")
  {
    _parse_and_check_error(YAML_ERR "layer/no_tile_layer_data.yaml", PE::NoTileLayerData);
    _parse_and_check_error(JSON_ERR "layer/no_tile_layer_data.json", PE::NoTileLayerData);
    _parse_and_check_error(XML_ERR "layer/no_tile_layer_data.xml", PE::NoTileLayerData);
  }

  TEST_CASE("Corrupt tile layer data")
  {
    _parse_and_check_error(YAML_ERR "layer/corrupt_tile_layer_data.yaml",
                           PE::CorruptTileLayerData);
    _parse_and_check_error(JSON_ERR "layer/corrupt_tile_layer_data.json",
                           PE::CorruptTileLayerData);
    _parse_and_check_error(XML_ERR "layer/corrupt_tile_layer_data.xml",
                           PE::CorruptTileLayerData);
  }

  TEST_CASE("No tileset tile width")
  {
    _parse_and_check_error(YAML_ERR "tileset/no_tileset_tile_width.yaml",
                           PE::NoTilesetTileWidth);
    _parse_and_check_error(JSON_ERR "tileset/no_tileset_tile_width.json",
                           PE::NoTilesetTileWidth);
    _parse_and_check_error(XML_ERR "tileset/no_tileset_tile_width.xml",
                           PE::NoTilesetTileWidth);
  }

  TEST_CASE("No tileset tile height")
  {
    _parse_and_check_error(YAML_ERR "tileset/no_tileset_tile_height.yaml",
                           PE::NoTilesetTileHeight);
    _parse_and_check_error(JSON_ERR "tileset/no_tileset_tile_height.json",
                           PE::NoTilesetTileHeight);
    _parse_and_check_error(XML_ERR "tileset/no_tileset_tile_height.xml",
                           PE::NoTilesetTileHeight);
  }

  TEST_CASE("No tileset name")
  {
    _parse_and_check_error(YAML_ERR "tileset/no_tileset_name.yaml", PE::NoTilesetName);
    _parse_and_check_error(JSON_ERR "tileset/no_tileset_name.json", PE::NoTilesetName);
    _parse_and_check_error(XML_ERR "tileset/no_tileset_name.xml", PE::NoTilesetName);
  }

  TEST_CASE("Tileset image does not exist")
  {
    _parse_and_check_error(YAML_ERR "tileset/tileset_image_does_not_exist.yaml",
                           PE::TilesetImageDoesNotExist);
    _parse_and_check_error(JSON_ERR "tileset/tileset_image_does_not_exist.json",
                           PE::TilesetImageDoesNotExist);
    _parse_and_check_error(XML_ERR "tileset/tileset_image_does_not_exist.xml",
                           PE::TilesetImageDoesNotExist);
  }

  TEST_CASE("No tileset image path")
  {
    _parse_and_check_error(YAML_ERR "tileset/no_tileset_image_path.yaml",
                           PE::NoTilesetImagePath);
    _parse_and_check_error(JSON_ERR "tileset/no_tileset_image_path.json",
                           PE::NoTilesetImagePath);
    _parse_and_check_error(XML_ERR "tileset/no_tileset_image_path.xml",
                           PE::NoTilesetImagePath);
  }

  TEST_CASE("No tileset image width")
  {
    _parse_and_check_error(YAML_ERR "tileset/no_tileset_image_width.yaml",
                           PE::NoTilesetImageWidth);
    _parse_and_check_error(JSON_ERR "tileset/no_tileset_image_width.json",
                           PE::NoTilesetImageWidth);
    _parse_and_check_error(XML_ERR "tileset/no_tileset_image_width.xml",
                           PE::NoTilesetImageWidth);
  }

  TEST_CASE("No tileset image height")
  {
    _parse_and_check_error(YAML_ERR "tileset/no_tileset_image_height.yaml",
                           PE::NoTilesetImageHeight);
    _parse_and_check_error(JSON_ERR "tileset/no_tileset_image_height.json",
                           PE::NoTilesetImageHeight);
    _parse_and_check_error(XML_ERR "tileset/no_tileset_image_height.xml",
                           PE::NoTilesetImageHeight);
  }

  TEST_CASE("No tileset tile count")
  {
    _parse_and_check_error(YAML_ERR "tileset/no_tileset_tile_count.yaml",
                           PE::NoTilesetTileCount);
    _parse_and_check_error(JSON_ERR "tileset/no_tileset_tile_count.json",
                           PE::NoTilesetTileCount);
    _parse_and_check_error(XML_ERR "tileset/no_tileset_tile_count.xml",
                           PE::NoTilesetTileCount);
  }

  TEST_CASE("No tileset column count")
  {
    _parse_and_check_error(YAML_ERR "tileset/no_tileset_column_count.yaml",
                           PE::NoTilesetColumnCount);
    _parse_and_check_error(JSON_ERR "tileset/no_tileset_column_count.json",
                           PE::NoTilesetColumnCount);
    _parse_and_check_error(XML_ERR "tileset/no_tileset_column_count.xml",
                           PE::NoTilesetColumnCount);
  }

  TEST_CASE("Corrupt property value")
  {
    _parse_and_check_error(YAML_ERR "property/corrupt_property_value.yaml",
                           PE::CorruptPropertyValue);
    _parse_and_check_error(JSON_ERR "property/corrupt_property_value.json",
                           PE::CorruptPropertyValue);
    _parse_and_check_error(XML_ERR "property/corrupt_property_value.xml",
                           PE::CorruptPropertyValue);
  }

  TEST_CASE("Unsupported property type")
  {
    _parse_and_check_error(YAML_ERR "property/unsupported_property_type.yaml",
                           PE::UnsupportedPropertyType);
    _parse_and_check_error(JSON_ERR "property/unsupported_property_type.json",
                           PE::UnsupportedPropertyType);
    _parse_and_check_error(XML_ERR "property/unsupported_property_type.xml",
                           PE::UnsupportedPropertyType);
  }

  TEST_CASE("No property name")
  {
    _parse_and_check_error(YAML_ERR "property/no_property_name.yaml", PE::NoPropertyName);
    _parse_and_check_error(JSON_ERR "property/no_property_name.json", PE::NoPropertyName);
    _parse_and_check_error(XML_ERR "property/no_property_name.xml", PE::NoPropertyName);
  }

  TEST_CASE("No property type")
  {
    _parse_and_check_error(YAML_ERR "property/no_property_type.yaml", PE::NoPropertyType);
    _parse_and_check_error(JSON_ERR "property/no_property_type.json", PE::NoPropertyType);
  }

  TEST_CASE("No object ID")
  {
    _parse_and_check_error(YAML_ERR "object/no_object_id.yaml", PE::NoObjectId);
    _parse_and_check_error(JSON_ERR "object/no_object_id.json", PE::NoObjectId);
    _parse_and_check_error(XML_ERR "object/no_object_id.xml", PE::NoObjectId);
  }

  TEST_CASE("No fancy tile ID")
  {
    _parse_and_check_error(YAML_ERR "fancy-tile/no_fancy_tile_id.yaml",
                           PE::NoFancyTileId);
    _parse_and_check_error(JSON_ERR "fancy-tile/no_fancy_tile_id.json",
                           PE::NoFancyTileId);
    _parse_and_check_error(XML_ERR "fancy-tile/no_fancy_tile_id.xml", PE::NoFancyTileId);
  }

  TEST_CASE("No animation frame tile")
  {
    _parse_and_check_error(YAML_ERR "fancy-tile/no_animation_frame_tile.yaml",
                           PE::NoAnimationFrameTile);
    _parse_and_check_error(JSON_ERR "fancy-tile/no_animation_frame_tile.json",
                           PE::NoAnimationFrameTile);
    _parse_and_check_error(XML_ERR "fancy-tile/no_animation_frame_tile.xml",
                           PE::NoAnimationFrameTile);
  }

  TEST_CASE("No animation frame duration")
  {
    _parse_and_check_error(YAML_ERR "fancy-tile/no_animation_frame_duration.yaml",
                           PE::NoAnimationFrameDuration);
    _parse_and_check_error(JSON_ERR "fancy-tile/no_animation_frame_duration.json",
                           PE::NoAnimationFrameDuration);
    _parse_and_check_error(XML_ERR "fancy-tile/no_animation_frame_duration.xml",
                           PE::NoAnimationFrameDuration);
  }

  TEST_CASE("YAML specific")
  {
    // Tilesets
    _parse_and_check_error(YAML_ERR "map/no_external_tileset_path.yaml",
                           PE::NoExternalTilesetPath);
    _parse_and_check_error(YAML_ERR "tileset/no_tileset_version.yaml",
                           PE::NoTilesetVersion);
    _parse_and_check_error(YAML_ERR "tileset/unsupported_tileset_version.yaml",
                           PE::UnsupportedTilesetVersion);

    // Objects
    _parse_and_check_error(YAML_ERR "object/no_object_type.yaml", PE::NoObjectType);
    _parse_and_check_error(YAML_ERR "object/unsupported_object_type.yaml",
                           PE::UnsupportedObjectType);

    // Components
    _parse_and_check_error(YAML_ERR "component/no_component_def_name.yaml",
                           PE::NoComponentDefName);
    _parse_and_check_error(YAML_ERR "component/no_component_def_attribute_name.yaml",
                           PE::NoComponentDefAttributeName);
    _parse_and_check_error(YAML_ERR "component/no_component_def_attribute_type.yaml",
                           PE::NoComponentDefAttributeType);
    _parse_and_check_error(YAML_ERR
                           "component/unsupported_component_def_attribute_type.yaml",
                           PE::UnsupportedComponentDefAttributeType);
    _parse_and_check_error(YAML_ERR
                           "component/corrupt_component_def_attribute_value.yaml",
                           PE::CorruptComponentDefAttributeValue);
    _parse_and_check_error(YAML_ERR "component/no_component_type.yaml",
                           PE::NoComponentType);
    _parse_and_check_error(YAML_ERR "component/no_component_attribute_name.yaml",
                           PE::NoComponentAttributeName);
    _parse_and_check_error(YAML_ERR "component/no_component_attribute_value.yaml",
                           PE::NoComponentAttributeValue);
    _parse_and_check_error(YAML_ERR "component/corrupt_component_attribute_value.yaml",
                           PE::CorruptComponentAttributeValue);

    // Tile format
    _parse_and_check_error(YAML_ERR "tile-format/bad_tile_format_encoding.yaml",
                           PE::BadTileFormatEncoding);
    _parse_and_check_error(YAML_ERR "tile-format/bad_tile_format_compression.yaml",
                           PE::BadTileFormatCompression);
    _parse_and_check_error(YAML_ERR "tile-format/bad_zlib_compression_level.yaml",
                           PE::BadZlibCompressionLevel);
    _parse_and_check_error(YAML_ERR "tile-format/plain_encoding_with_compression.yaml",
                           PE::PlainEncodingWithCompression);
  }
}

}  // namespace tactile::test
