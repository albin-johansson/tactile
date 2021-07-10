#pragma once

#include <ostream>  // ostream

namespace Tactile::IO {

enum class ParseError
{
  None,

  MapUnsupportedExtension,
  MapDoesNotExist,
  MapMissingNextLayerId,
  MapMissingNextObjectId,
  MapMissingTilesets,
  MapMissingLayers,

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

auto operator<<(std::ostream& stream, ParseError error) -> std::ostream&;

}  // namespace Tactile::IO
