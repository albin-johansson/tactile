#pragma once

#include <ostream>  // ostream

#include "aliases/czstring.hpp"

namespace Tactile::IO {

// TODO there are many possible errors caused by unsupported Tiled features
enum class ParseError
{
  None,

  MapUnsupportedExtension,
  MapUnsupportedOrientation,
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
