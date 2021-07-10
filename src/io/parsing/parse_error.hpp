#pragma once

#include <ostream>  // ostream

namespace Tactile::IO {

enum class ParseError
{
  None,
  CouldNotOpenFile,

  MapDoesNotExist,
  MapMissingNextLayerId,
  MapMissingNextObjectId,
  MapMissingTilesets,
  MapMissingLayers,

  LayerMissingWidth,
  LayerMissingHeight,
  LayerMissingType,
  LayerMissingId,

  CouldNotParseTiles,
  UnsupportedTileEncoding,

  TilesetMissingFirstGid,
  TilesetMissingTileWidth,
  TilesetMissingTileHeight,
  TilesetMissingImagePath,
  TilesetMissingName,
  CouldNotReadExternalTileset,
  TilesetImageDoesNotExist,

  CouldNotParseProperty,
  UnknownPropertyType,
  UnknownLayerType,

  ObjectMissingId
};

auto operator<<(std::ostream& stream, ParseError error) -> std::ostream&;

}  // namespace Tactile::IO
