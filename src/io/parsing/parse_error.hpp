#pragma once

#include <ostream>  // ostream

#include "aliases/czstring.hpp"

namespace Tactile::IO {

enum class ParseError
{
  None,
  Unknown,

  MapUnsupportedExtension,
  MapUnsupportedOrientation,
  MapUnsupportedInfinite,
  MapDoesNotExist,
  MapMissingNextLayerId,
  MapMissingNextObjectId,
  MapMissingTilesets,
  MapMissingLayers,
  MapMissingTileWidth,
  MapMissingTileHeight,

  LayerMissingWidth,
  LayerMissingHeight,
  LayerMissingType,
  LayerMissingId,
  LayerUnknownType,

  CouldNotParseTiles,
  UnsupportedTileEncoding,

  CouldNotReadExternalTileset,
  TilesetMissingFirstGid,
  TilesetMissingTileWidth,
  TilesetMissingTileHeight,
  TilesetMissingImagePath,
  TilesetMissingName,
  TilesetImageDoesNotExist,

  CouldNotParseProperty,
  PropertyMissingName,
  PropertyUnknownType,

  ObjectMissingId
};

[[nodiscard]] auto GetCause(ParseError error) -> czstring;

auto operator<<(std::ostream& stream, ParseError error) -> std::ostream&;

}  // namespace Tactile::IO
