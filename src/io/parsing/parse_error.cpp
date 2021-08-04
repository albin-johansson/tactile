#include "parse_error.hpp"

#include "core/tactile_error.hpp"

namespace Tactile::IO {

auto operator<<(std::ostream& stream, const ParseError error) -> std::ostream&
{
  switch (error)
  {
    case ParseError::None:
      return stream << "None";

    case ParseError::MapUnsupportedExtension:
      return stream << "MapUnsupportedExtension";

    case ParseError::MapUnsupportedOrientation:
      return stream << "MapUnsupportedOrientation";

    case ParseError::MapDoesNotExist:
      return stream << "MapDoesNotExist";

    case ParseError::MapMissingNextLayerId:
      return stream << "MapMissingNextLayerId";

    case ParseError::MapMissingNextObjectId:
      return stream << "MapMissingNextObjectId";

    case ParseError::MapMissingTilesets:
      return stream << "MapMissingTilesets";

    case ParseError::MapMissingLayers:
      return stream << "MapMissingLayers";

    case ParseError::MapMissingTileWidth:
      return stream << "MapMissingTileWidth";

    case ParseError::MapMissingTileHeight:
      return stream << "MapMissingTileHeight";

    case ParseError::LayerMissingWidth:
      return stream << "LayerMissingWidth";

    case ParseError::LayerMissingHeight:
      return stream << "LayerMissingHeight";

    case ParseError::LayerMissingType:
      return stream << "LayerMissingType";

    case ParseError::LayerMissingId:
      return stream << "LayerMissingId";

    case ParseError::LayerUnknownType:
      return stream << "LayerUnknownType";

    case ParseError::CouldNotParseTiles:
      return stream << "CouldNotParseTiles";

    case ParseError::UnsupportedTileEncoding:
      return stream << "UnsupportedTileEncoding";

    case ParseError::CouldNotReadExternalTileset:
      return stream << "CouldNotReadExternalTileset";

    case ParseError::TilesetMissingFirstGid:
      return stream << "TilesetMissingFirstGid";

    case ParseError::TilesetMissingTileWidth:
      return stream << "TilesetMissingTileWidth";

    case ParseError::TilesetMissingTileHeight:
      return stream << "TilesetMissingTileHeight";

    case ParseError::TilesetMissingImagePath:
      return stream << "TilesetMissingImagePath";

    case ParseError::TilesetMissingName:
      return stream << "TilesetMissingName";

    case ParseError::TilesetImageDoesNotExist:
      return stream << "TilesetImageDoesNotExist";

    case ParseError::CouldNotParseProperty:
      return stream << "CouldNotParseProperty";

    case ParseError::PropertyMissingName:
      return stream << "PropertyMissingName";

    case ParseError::PropertyUnknownType:
      return stream << "PropertyUnknownType";

    case ParseError::ObjectMissingId:
      return stream << "ObjectMissingId";

    default:
      throw TactileError{"Did not recognize parse error!"};
  }
}

}  // namespace Tactile::IO
