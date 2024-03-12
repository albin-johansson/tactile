// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <ostream>

#include "tactile/base/container/string.hpp"

namespace tactile {

/// Represents various possible errors related to map parsing.
///
/// \see to_cause(ParseError)
enum class ParseError {
  None,
  Unknown,

  CouldNotReadFile,

  MapDoesNotExist,
  UnsupportedMapExtension,
  UnsupportedMapOrientation,
  UnsupportedInfiniteMap,
  NoMapWidth,
  NoMapHeight,
  NoMapTileWidth,
  NoMapTileHeight,
  NoMapNextLayerId,
  NoMapNextObjectId,

  UnsupportedLayerType,  // YAML/JSON only
  NoLayerId,
  NoLayerType,  // YAML/JSON only
  NoTileLayerData,
  CorruptTileLayerData,
  UnsupportedTileLayerEncoding,

  UnknownExternalTilesetError,
  ExternalTilesetDoesNotExist,
  NoTilesetFirstTileId,
  NoTilesetTileWidth,
  NoTilesetTileHeight,
  NoTilesetName,  // TODO warning only?
  NoTilesetImagePath,
  NoTilesetImageWidth,
  NoTilesetImageHeight,
  NoTilesetTileCount,
  NoTilesetColumnCount,
  NoTilesetVersion,       // Tactile YAML only
  NoExternalTilesetPath,  // Tactile YAML only
  TilesetImageDoesNotExist,
  UnsupportedTilesetVersion,  // Tactile YAML only

  CorruptPropertyValue,
  NoPropertyName,
  NoPropertyType,  // Not used by Tiled XML
  UnsupportedPropertyType,

  NoComponentDefName,                    // Tactile YAML only
  NoComponentDefAttributeName,           // Tactile YAML only
  NoComponentDefAttributeType,           // Tactile YAML only
  UnsupportedComponentDefAttributeType,  // Tactile YAML only
  CorruptComponentDefAttributeValue,     // Tactile YAML only

  NoComponentType,                 // Tactile YAML only
  NoComponentAttributeName,        // Tactile YAML only
  NoComponentAttributeValue,       // Tactile YAML only
  CorruptComponentAttributeValue,  // Tactile YAML only

  NoObjectId,
  NoObjectType,           // Tactile YAML only
  UnsupportedObjectType,  // Tactile YAML only

  NoFancyTileId,
  NoAnimationFrameTile,
  NoAnimationFrameDuration,

  BadTileFormatEncoding,
  BadTileFormatCompression,
  BadZlibCompressionLevel,
  BadZstdCompressionLevel,
  PlainEncodingWithCompression
};

/// Returns a (translated) human-readable message with an explanation of an error.
[[nodiscard]] auto to_cause(ParseError error) -> StringView;

auto operator<<(std::ostream& stream, ParseError error) -> std::ostream&;

}  // namespace tactile
