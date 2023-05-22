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

#include "parse_error.hpp"

#include <magic_enum.hpp>

#include "common/debug/panic.hpp"

namespace tactile {

auto to_cause(const Strings& strings, const ParseError error) -> StringView
{
  switch (error) {
    case ParseError::None:
      return strings.parse_error.none;

    case ParseError::Unknown:
      return strings.parse_error.unknown;

    case ParseError::CouldNotReadFile:
      return strings.parse_error.could_not_read_file;

    case ParseError::MapDoesNotExist:
      return strings.parse_error.map_does_not_exist;

    case ParseError::UnsupportedMapExtension:
      return strings.parse_error.unsupported_map_format;

    case ParseError::UnsupportedMapOrientation:
      return strings.parse_error.unsupported_map_orientation;

    case ParseError::UnsupportedInfiniteMap:
      return strings.parse_error.unsupported_infinite_map;

    case ParseError::NoMapWidth:
      return strings.parse_error.no_map_width;

    case ParseError::NoMapHeight:
      return strings.parse_error.no_map_height;

    case ParseError::NoMapTileWidth:
      return strings.parse_error.no_map_tile_width;

    case ParseError::NoMapTileHeight:
      return strings.parse_error.no_map_tile_height;

    case ParseError::NoMapNextLayerId:
      return strings.parse_error.no_next_layer_id;

    case ParseError::NoMapNextObjectId:
      return strings.parse_error.no_next_object_id;

    case ParseError::UnsupportedLayerType:
      return strings.parse_error.invalid_layer_type;

    case ParseError::NoLayerId:
      return strings.parse_error.no_next_layer_id;

    case ParseError::NoLayerType:
      return strings.parse_error.no_layer_type;

    case ParseError::NoTileLayerData:
      return strings.parse_error.no_tile_layer_data;

    case ParseError::CorruptTileLayerData:
      return strings.parse_error.corrupt_tile_layer_data;

    case ParseError::UnsupportedTileLayerEncoding:
      return strings.parse_error.unsupported_tile_layer_encoding;

    case ParseError::ExternalTilesetDoesNotExist:
      return strings.parse_error.external_tileset_does_not_exist;

    case ParseError::UnknownExternalTilesetError:
      return strings.parse_error.unknown_external_tileset_error;

    case ParseError::NoTilesetFirstTileId:
      return strings.parse_error.no_first_tileset_tile_id;

    case ParseError::NoTilesetTileWidth:
      return strings.parse_error.no_tileset_tile_width;

    case ParseError::NoTilesetTileHeight:
      return strings.parse_error.no_tileset_tile_height;

    case ParseError::NoTilesetName:
      return strings.parse_error.no_tileset_name;

    case ParseError::NoTilesetImagePath:
      return strings.parse_error.no_tileset_image_path;

    case ParseError::NoTilesetImageWidth:
      return strings.parse_error.no_tileset_image_width;

    case ParseError::NoTilesetImageHeight:
      return strings.parse_error.no_tileset_image_height;

    case ParseError::NoTilesetTileCount:
      return strings.parse_error.no_tileset_tile_count;

    case ParseError::NoTilesetColumnCount:
      return strings.parse_error.no_tileset_column_count;

    case ParseError::NoTilesetVersion:
      return strings.parse_error.no_tileset_version;

    case ParseError::NoExternalTilesetPath:
      return strings.parse_error.no_external_tileset_source;

    case ParseError::TilesetImageDoesNotExist:
      return strings.parse_error.tileset_image_does_not_exist;

    case ParseError::UnsupportedTilesetVersion:
      return strings.parse_error.unsupported_tileset_version;

    case ParseError::CorruptPropertyValue:
      return strings.parse_error.corrupt_property_value;

    case ParseError::NoPropertyName:
      return strings.parse_error.no_property_name;

    case ParseError::NoPropertyType:
      return strings.parse_error.no_property_type;

    case ParseError::UnsupportedPropertyType:
      return strings.parse_error.unsupported_property_type;

    case ParseError::NoComponentDefName:
      return strings.parse_error.comp_def_no_name;

    case ParseError::NoComponentDefAttributeName:
      return strings.parse_error.comp_def_no_attr_name;

    case ParseError::NoComponentDefAttributeType:
      return strings.parse_error.comp_def_no_attr_type;

    case ParseError::UnsupportedComponentDefAttributeType:
      return strings.parse_error.comp_def_unsupported_attr_type;

    case ParseError::CorruptComponentDefAttributeValue:
      return strings.parse_error.comp_def_corrupt_attr_value;

    case ParseError::NoComponentType:
      return strings.parse_error.no_component_type;

    case ParseError::NoComponentAttributeName:
      return strings.parse_error.no_component_attr_name;

    case ParseError::NoComponentAttributeValue:
      return strings.parse_error.no_component_attr_value;

    case ParseError::CorruptComponentAttributeValue:
      return strings.parse_error.corrupt_component_attr_value;

    case ParseError::NoObjectId:
      return strings.parse_error.no_object_id;

    case ParseError::NoObjectType:
      return strings.parse_error.no_object_type;

    case ParseError::UnsupportedObjectType:
      return strings.parse_error.unsupported_object_type;

    case ParseError::NoFancyTileId:
      return strings.parse_error.tile_def_no_id;

    case ParseError::NoAnimationFrameTile:
      return strings.parse_error.tile_def_no_anim_frame_tile;

    case ParseError::NoAnimationFrameDuration:
      return strings.parse_error.tile_def_no_anim_frame_duration;

    case ParseError::BadTileFormatEncoding:
      return strings.parse_error.bad_tile_format_encoding;

    case ParseError::BadTileFormatCompression:
      return strings.parse_error.bad_tile_format_compression;

    case ParseError::BadZlibCompressionLevel:
      return strings.parse_error.bad_zlib_compression_level;

    case ParseError::BadZstdCompressionLevel:
      return strings.parse_error.bad_zstd_compression_level;

    case ParseError::PlainEncodingWithCompression:
      return strings.parse_error.plain_encoding_with_compression;

    default:
      throw TactileError {"Invalid parse error"};
  }
}

auto operator<<(OStream& stream, const ParseError error) -> OStream&
{
  return stream << magic_enum::enum_name(error);
}

}  // namespace tactile