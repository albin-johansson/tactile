#include "parse_error.hpp"

#include "core/tactile_error.hpp"

namespace Tactile::IO {

auto GetCause(const ParseError error) -> CStr
{
  switch (error)
  {
    default:
      [[fallthrough]];

    case ParseError::None:
      return "N/A";  // This shouldn't ever be used

    case ParseError::Unknown:
      return "Unknown.";

    case ParseError::CouldNotReadFile:
      return "Failed to read a file.";

    case ParseError::MapUnsupportedExtension:
      return "The map file featured an unsupported file extension.";

    case ParseError::MapUnsupportedOrientation:
      return "The map used an unsupported tile orientation.";

    case ParseError::MapUnsupportedInfinite:
      return "Infinite maps are not supported.";

    case ParseError::MapDoesNotExist:
      return "The specified map file did not exist.";

    case ParseError::MapMissingNextLayerId:
      return "The map was missing the next layer ID attribute!";

    case ParseError::MapMissingNextObjectId:
      return "The map was missing the next object ID attribute!";

    case ParseError::MapMissingTilesets:
      return "The map was missing the tilesets attribute!";

    case ParseError::MapMissingLayers:
      return "The map was missing the layers attribute!";

    case ParseError::MapMissingTileWidth:
      return "The map was missing a tile width attribute!";

    case ParseError::MapMissingTileHeight:
      return "The map was missing a tile height attribute!";

    case ParseError::MapMissingWidth:
      return "The map had no width attribute!";

    case ParseError::MapMissingHeight:
      return "The map had no height attribute!";

    case ParseError::LayerMissingWidth:
      return "The map featured a layer without a width!";

    case ParseError::LayerMissingHeight:
      return "The map featured a layer without a height!";

    case ParseError::LayerMissingType:
      return "The map featured a layer without a type!";

    case ParseError::LayerMissingId:
      return "The map featured a layer without an ID!";

    case ParseError::LayerUnknownType:
      return "The map featured a layer with an unknown type!";

    case ParseError::CouldNotParseTiles:
      return "The map featured a tile layer with corrupt tile data!";

    case ParseError::UnsupportedTileEncoding:
      return "The map featured a tile layer that used an unsupported tile encoding!";

    case ParseError::CouldNotReadExternalTileset:
      return "Could not read an external tileset!";

    case ParseError::TilesetMissingFirstGid:
      return "The map featured a tileset without a first global tile ID attribute!";

    case ParseError::TilesetMissingTileWidth:
      return "The map featured a tileset without a tile width attribute!";

    case ParseError::TilesetMissingTileHeight:
      return "The map featured a tileset without a tile height attribute!";

    case ParseError::TilesetMissingImagePath:
      return "The map featured a tileset without an image path attribute!";

    case ParseError::TilesetMissingName:
      return "The map featured a tileset without a name!";

    case ParseError::TilesetImageDoesNotExist:
      return "The map featured a tileset based on a non-existent image!";

    case ParseError::CouldNotParseProperty:
      return "The map featured a corrupt property!";

    case ParseError::PropertyMissingName:
      return "The map featured a property without a name!";

    case ParseError::PropertyUnknownType:
      return "The map featured a property with an unknown type!";

    case ParseError::ObjectMissingId:
      return "The map featured an object layer containing an object without an ID!";
  }
}

auto operator<<(std::ostream& stream, const ParseError error) -> std::ostream&
{
  switch (error)
  {
    case ParseError::None:
      return stream << "None";

    case ParseError::Unknown:
      return stream << "Unknown";

    case ParseError::CouldNotReadFile:
      return stream << "CouldNotReadFile";

    case ParseError::MapUnsupportedExtension:
      return stream << "MapUnsupportedExtension";

    case ParseError::MapUnsupportedOrientation:
      return stream << "MapUnsupportedOrientation";

    case ParseError::MapUnsupportedInfinite:
      return stream << "MapUnsupportedInfinite";

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

    case ParseError::MapMissingWidth:
      return stream << "MapMissingWidth";

    case ParseError::MapMissingHeight:
      return stream << "MapMissingHeight";

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
