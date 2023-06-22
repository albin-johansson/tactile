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

#pragma once

#include "common/type/expected.hpp"
#include "common/type/ostream.hpp"
#include "common/type/string.hpp"
#include "model/i18n/language_components.hpp"

namespace tactile {

/**
 * Represents various possible errors related to map parsing.
 *
 * \see to_cause(ParseError)
 */
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
  NoLayerType,           // YAML/JSON only
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
  NoTilesetVersion,           // Tactile YAML only
  NoExternalTilesetPath,      // Tactile YAML only
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

  NoComponentType,                       // Tactile YAML only
  NoComponentAttributeName,              // Tactile YAML only
  NoComponentAttributeValue,             // Tactile YAML only
  CorruptComponentAttributeValue,        // Tactile YAML only

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

template <typename T>
using Parsed = Expected<T, ParseError>;

/// Returns a (translated) human-readable message with an explanation of an error.
[[nodiscard]] auto to_cause(const Strings& strings, ParseError error) -> StringView;

auto operator<<(OStream& stream, ParseError error) -> OStream&;

}  // namespace tactile