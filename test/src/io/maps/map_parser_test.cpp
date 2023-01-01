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

using PE = io::ParseError;

void parse_and_check_error(const char* path, const io::ParseError expected_error)
{
  const auto parse_result = io::parse_map(path);
  CHECK(expected_error == parse_result.error());
}

}  // namespace

// This test suite does not perform thorough validation of the resulting IR from parsing,
// but focuses on the returned error codes. See the round trip test suite for tests
// related to validating the behavior of the emitters and parsers.
TEST_SUITE("Map parser")
{
  TEST_CASE("Valid maps")
  {
    parse_and_check_error(YAML_DIR "minimal.yaml", PE::None);
    parse_and_check_error(JSON_DIR "minimal.json", PE::None);
    parse_and_check_error(XML_DIR "minimal.xml", PE::None);

    parse_and_check_error(YAML_DIR "normal.yaml", PE::None);
  }

  TEST_CASE("Invalid file types and missing maps")
  {
    parse_and_check_error("resources/dummy.txt", PE::UnsupportedMapExtension);
    parse_and_check_error("foo.yaml", PE::MapDoesNotExist);
    parse_and_check_error("foo.json", PE::MapDoesNotExist);
    parse_and_check_error("foo.tmx", PE::MapDoesNotExist);
  }

  TEST_CASE("No map width")
  {
    parse_and_check_error(YAML_ERR "map/no_map_width.yaml", PE::NoMapWidth);
    parse_and_check_error(JSON_ERR "map/no_map_width.json", PE::NoMapWidth);
    parse_and_check_error(XML_ERR "map/no_map_width.xml", PE::NoMapWidth);
  }

  TEST_CASE("No map height")
  {
    parse_and_check_error(YAML_ERR "map/no_map_height.yaml", PE::NoMapHeight);
    parse_and_check_error(JSON_ERR "map/no_map_height.json", PE::NoMapHeight);
    parse_and_check_error(XML_ERR "map/no_map_height.xml", PE::NoMapHeight);
  }

  TEST_CASE("No tile width")
  {
    parse_and_check_error(YAML_ERR "map/no_map_tile_width.yaml", PE::NoMapTileWidth);
    parse_and_check_error(JSON_ERR "map/no_map_tile_width.json", PE::NoMapTileWidth);
    parse_and_check_error(XML_ERR "map/no_map_tile_width.xml", PE::NoMapTileWidth);
  }

  TEST_CASE("No tile height")
  {
    parse_and_check_error(YAML_ERR "map/no_map_tile_height.yaml", PE::NoMapTileHeight);
    parse_and_check_error(JSON_ERR "map/no_map_tile_height.json", PE::NoMapTileHeight);
    parse_and_check_error(XML_ERR "map/no_map_tile_height.xml", PE::NoMapTileHeight);
  }

  TEST_CASE("No next layer ID")
  {
    parse_and_check_error(YAML_ERR "map/no_next_layer_id.yaml", PE::NoMapNextLayerId);
    parse_and_check_error(JSON_ERR "map/no_next_layer_id.json", PE::NoMapNextLayerId);
    parse_and_check_error(XML_ERR "map/no_next_layer_id.xml", PE::NoMapNextLayerId);
  }

  TEST_CASE("No next object ID")
  {
    parse_and_check_error(YAML_ERR "map/no_next_object_id.yaml", PE::NoMapNextObjectId);
    parse_and_check_error(JSON_ERR "map/no_next_object_id.json", PE::NoMapNextObjectId);
    parse_and_check_error(XML_ERR "map/no_next_object_id.xml", PE::NoMapNextObjectId);
  }

  TEST_CASE("No tileset first tile ID")
  {
    parse_and_check_error(YAML_ERR "map/no_tileset_first_tile_id.yaml",
                          PE::NoTilesetFirstTileId);
    parse_and_check_error(JSON_ERR "map/no_tileset_first_tile_id.json",
                          PE::NoTilesetFirstTileId);
  }

  TEST_CASE("External tileset does not exist")
  {
    parse_and_check_error(YAML_ERR "map/external_tileset_does_not_exist.yaml",
                          PE::ExternalTilesetDoesNotExist);
    parse_and_check_error(JSON_ERR "map/external_tileset_does_not_exist.json",
                          PE::ExternalTilesetDoesNotExist);
    parse_and_check_error(XML_ERR "map/external_tileset_does_not_exist.xml",
                          PE::ExternalTilesetDoesNotExist);
  }

  TEST_CASE("Unknown external tileset error")
  {
    parse_and_check_error(YAML_ERR "map/unknown_external_tileset_error.yaml",
                          PE::UnknownExternalTilesetError);
    parse_and_check_error(JSON_ERR "map/unknown_external_tileset_error.json",
                          PE::UnknownExternalTilesetError);
    parse_and_check_error(XML_ERR "map/unknown_external_tileset_error.xml",
                          PE::UnknownExternalTilesetError);
  }

  TEST_CASE("Unsupported layer type")
  {
    parse_and_check_error(YAML_ERR "layer/unsupported_layer_type.yaml",
                          PE::UnsupportedLayerType);
    parse_and_check_error(JSON_ERR "layer/unsupported_layer_type.json",
                          PE::UnsupportedLayerType);
  }

  TEST_CASE("No layer ID")
  {
    parse_and_check_error(YAML_ERR "layer/no_layer_id.yaml", PE::NoLayerId);
    parse_and_check_error(JSON_ERR "layer/no_layer_id.json", PE::NoLayerId);
    parse_and_check_error(XML_ERR "layer/no_layer_id.xml", PE::NoLayerId);
  }

  TEST_CASE("No layer type")
  {
    parse_and_check_error(YAML_ERR "layer/no_layer_type.yaml", PE::NoLayerType);
    parse_and_check_error(JSON_ERR "layer/no_layer_type.json", PE::NoLayerType);
  }

  TEST_CASE("No tile layer data")
  {
    parse_and_check_error(YAML_ERR "layer/no_tile_layer_data.yaml", PE::NoTileLayerData);
    parse_and_check_error(JSON_ERR "layer/no_tile_layer_data.json", PE::NoTileLayerData);
    parse_and_check_error(XML_ERR "layer/no_tile_layer_data.xml", PE::NoTileLayerData);
  }

  TEST_CASE("Corrupt tile layer data")
  {
    parse_and_check_error(YAML_ERR "layer/corrupt_tile_layer_data.yaml",
                          PE::CorruptTileLayerData);
    parse_and_check_error(JSON_ERR "layer/corrupt_tile_layer_data.json",
                          PE::CorruptTileLayerData);
    parse_and_check_error(XML_ERR "layer/corrupt_tile_layer_data.xml",
                          PE::CorruptTileLayerData);
  }

  TEST_CASE("No tileset tile width")
  {
    parse_and_check_error(YAML_ERR "tileset/no_tileset_tile_width.yaml",
                          PE::NoTilesetTileWidth);
    parse_and_check_error(JSON_ERR "tileset/no_tileset_tile_width.json",
                          PE::NoTilesetTileWidth);
    parse_and_check_error(XML_ERR "tileset/no_tileset_tile_width.xml",
                          PE::NoTilesetTileWidth);
  }

  TEST_CASE("No tileset tile height")
  {
    parse_and_check_error(YAML_ERR "tileset/no_tileset_tile_height.yaml",
                          PE::NoTilesetTileHeight);
    parse_and_check_error(JSON_ERR "tileset/no_tileset_tile_height.json",
                          PE::NoTilesetTileHeight);
    parse_and_check_error(XML_ERR "tileset/no_tileset_tile_height.xml",
                          PE::NoTilesetTileHeight);
  }

  TEST_CASE("No tileset name")
  {
    parse_and_check_error(YAML_ERR "tileset/no_tileset_name.yaml", PE::NoTilesetName);
    parse_and_check_error(JSON_ERR "tileset/no_tileset_name.json", PE::NoTilesetName);
    parse_and_check_error(XML_ERR "tileset/no_tileset_name.xml", PE::NoTilesetName);
  }

  TEST_CASE("Tileset image does not exist")
  {
    parse_and_check_error(YAML_ERR "tileset/tileset_image_does_not_exist.yaml",
                          PE::TilesetImageDoesNotExist);
    parse_and_check_error(JSON_ERR "tileset/tileset_image_does_not_exist.json",
                          PE::TilesetImageDoesNotExist);
    parse_and_check_error(XML_ERR "tileset/tileset_image_does_not_exist.xml",
                          PE::TilesetImageDoesNotExist);
  }

  TEST_CASE("No tileset image path")
  {
    parse_and_check_error(YAML_ERR "tileset/no_tileset_image_path.yaml",
                          PE::NoTilesetImagePath);
    parse_and_check_error(JSON_ERR "tileset/no_tileset_image_path.json",
                          PE::NoTilesetImagePath);
    parse_and_check_error(XML_ERR "tileset/no_tileset_image_path.xml",
                          PE::NoTilesetImagePath);
  }

  TEST_CASE("No tileset image width")
  {
    parse_and_check_error(YAML_ERR "tileset/no_tileset_image_width.yaml",
                          PE::NoTilesetImageWidth);
    parse_and_check_error(JSON_ERR "tileset/no_tileset_image_width.json",
                          PE::NoTilesetImageWidth);
    parse_and_check_error(XML_ERR "tileset/no_tileset_image_width.xml",
                          PE::NoTilesetImageWidth);
  }

  TEST_CASE("No tileset image height")
  {
    parse_and_check_error(YAML_ERR "tileset/no_tileset_image_height.yaml",
                          PE::NoTilesetImageHeight);
    parse_and_check_error(JSON_ERR "tileset/no_tileset_image_height.json",
                          PE::NoTilesetImageHeight);
    parse_and_check_error(XML_ERR "tileset/no_tileset_image_height.xml",
                          PE::NoTilesetImageHeight);
  }

  TEST_CASE("No tileset tile count")
  {
    parse_and_check_error(YAML_ERR "tileset/no_tileset_tile_count.yaml",
                          PE::NoTilesetTileCount);
    parse_and_check_error(JSON_ERR "tileset/no_tileset_tile_count.json",
                          PE::NoTilesetTileCount);
    parse_and_check_error(XML_ERR "tileset/no_tileset_tile_count.xml",
                          PE::NoTilesetTileCount);
  }

  TEST_CASE("No tileset column count")
  {
    parse_and_check_error(YAML_ERR "tileset/no_tileset_column_count.yaml",
                          PE::NoTilesetColumnCount);
    parse_and_check_error(JSON_ERR "tileset/no_tileset_column_count.json",
                          PE::NoTilesetColumnCount);
    parse_and_check_error(XML_ERR "tileset/no_tileset_column_count.xml",
                          PE::NoTilesetColumnCount);
  }

  TEST_CASE("Corrupt property value")
  {
    parse_and_check_error(YAML_ERR "property/corrupt_property_value.yaml",
                          PE::CorruptPropertyValue);
    parse_and_check_error(JSON_ERR "property/corrupt_property_value.json",
                          PE::CorruptPropertyValue);
    parse_and_check_error(XML_ERR "property/corrupt_property_value.xml",
                          PE::CorruptPropertyValue);
  }

  TEST_CASE("Unsupported property type")
  {
    parse_and_check_error(YAML_ERR "property/unsupported_property_type.yaml",
                          PE::UnsupportedPropertyType);
    parse_and_check_error(JSON_ERR "property/unsupported_property_type.json",
                          PE::UnsupportedPropertyType);
    parse_and_check_error(XML_ERR "property/unsupported_property_type.xml",
                          PE::UnsupportedPropertyType);
  }

  TEST_CASE("No property name")
  {
    parse_and_check_error(YAML_ERR "property/no_property_name.yaml", PE::NoPropertyName);
    parse_and_check_error(JSON_ERR "property/no_property_name.json", PE::NoPropertyName);
    parse_and_check_error(XML_ERR "property/no_property_name.xml", PE::NoPropertyName);
  }

  TEST_CASE("No property type")
  {
    parse_and_check_error(YAML_ERR "property/no_property_type.yaml", PE::NoPropertyType);
    parse_and_check_error(JSON_ERR "property/no_property_type.json", PE::NoPropertyType);
  }

  TEST_CASE("No object ID")
  {
    parse_and_check_error(YAML_ERR "object/no_object_id.yaml", PE::NoObjectId);
    parse_and_check_error(JSON_ERR "object/no_object_id.json", PE::NoObjectId);
    parse_and_check_error(XML_ERR "object/no_object_id.xml", PE::NoObjectId);
  }

  TEST_CASE("No fancy tile ID")
  {
    parse_and_check_error(YAML_ERR "fancy-tile/no_fancy_tile_id.yaml", PE::NoFancyTileId);
    parse_and_check_error(JSON_ERR "fancy-tile/no_fancy_tile_id.json", PE::NoFancyTileId);
    parse_and_check_error(XML_ERR "fancy-tile/no_fancy_tile_id.xml", PE::NoFancyTileId);
  }

  TEST_CASE("No animation frame tile")
  {
    parse_and_check_error(YAML_ERR "fancy-tile/no_animation_frame_tile.yaml",
                          PE::NoAnimationFrameTile);
    parse_and_check_error(JSON_ERR "fancy-tile/no_animation_frame_tile.json",
                          PE::NoAnimationFrameTile);
    parse_and_check_error(XML_ERR "fancy-tile/no_animation_frame_tile.xml",
                          PE::NoAnimationFrameTile);
  }

  TEST_CASE("No animation frame duration")
  {
    parse_and_check_error(YAML_ERR "fancy-tile/no_animation_frame_duration.yaml",
                          PE::NoAnimationFrameDuration);
    parse_and_check_error(JSON_ERR "fancy-tile/no_animation_frame_duration.json",
                          PE::NoAnimationFrameDuration);
    parse_and_check_error(XML_ERR "fancy-tile/no_animation_frame_duration.xml",
                          PE::NoAnimationFrameDuration);
  }

  TEST_CASE("YAML specific")
  {
    // Tilesets
    parse_and_check_error(YAML_ERR "map/no_external_tileset_path.yaml",
                          PE::NoExternalTilesetPath);
    parse_and_check_error(YAML_ERR "tileset/no_tileset_version.yaml",
                          PE::NoTilesetVersion);
    parse_and_check_error(YAML_ERR "tileset/unsupported_tileset_version.yaml",
                          PE::UnsupportedTilesetVersion);

    // Objects
    parse_and_check_error(YAML_ERR "object/no_object_type.yaml", PE::NoObjectType);
    parse_and_check_error(YAML_ERR "object/unsupported_object_type.yaml",
                          PE::UnsupportedObjectType);

    // Components
    parse_and_check_error(YAML_ERR "component/no_component_def_name.yaml",
                          PE::NoComponentDefName);
    parse_and_check_error(YAML_ERR "component/no_component_def_attribute_name.yaml",
                          PE::NoComponentDefAttributeName);
    parse_and_check_error(YAML_ERR "component/no_component_def_attribute_type.yaml",
                          PE::NoComponentDefAttributeType);
    parse_and_check_error(YAML_ERR
                          "component/unsupported_component_def_attribute_type.yaml",
                          PE::UnsupportedComponentDefAttributeType);
    parse_and_check_error(YAML_ERR "component/corrupt_component_def_attribute_value.yaml",
                          PE::CorruptComponentDefAttributeValue);
    parse_and_check_error(YAML_ERR "component/no_component_type.yaml",
                          PE::NoComponentType);
    parse_and_check_error(YAML_ERR "component/no_component_attribute_name.yaml",
                          PE::NoComponentAttributeName);
    parse_and_check_error(YAML_ERR "component/no_component_attribute_value.yaml",
                          PE::NoComponentAttributeValue);
    parse_and_check_error(YAML_ERR "component/corrupt_component_attribute_value.yaml",
                          PE::CorruptComponentAttributeValue);

    // Tile format
    parse_and_check_error(YAML_ERR "tile-format/bad_tile_format_encoding.yaml",
                          PE::BadTileFormatEncoding);
    parse_and_check_error(YAML_ERR "tile-format/bad_tile_format_compression.yaml",
                          PE::BadTileFormatCompression);
    parse_and_check_error(YAML_ERR "tile-format/bad_zlib_compression_level.yaml",
                          PE::BadZlibCompressionLevel);
    parse_and_check_error(YAML_ERR "tile-format/plain_encoding_with_compression.yaml",
                          PE::PlainEncodingWithCompression);
  }
}

}  // namespace tactile::test
