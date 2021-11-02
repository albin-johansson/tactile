#include "parse_error.hpp"

#include <magic_enum.hpp>  // enum_name

namespace Tactile::IO {

auto GetCause(const ParseError error) -> CStr
{
  switch (error) {
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

    case ParseError::PropertyMissingType:
      return "The map featured a property without a type!";

    case ParseError::PropertyUnknownType:
      return "The map featured a property with an unknown type!";

    case ParseError::ObjectMissingId:
      return "The map featured an object layer containing an object without an ID!";

    case ParseError::ObjectInvalidType:
      return "The map featured an object layer with an object of an invalid type!";

    case ParseError::ObjectMissingType:
      return "The map featured an object layer with an object without a type!";

    case ParseError::MissingTilesetPath:
      return "The map featured an external tileset reference without a path!";

    case ParseError::ExternalTilesetDoesNotExist:
      return "The map featured a path to an external tileset that doesn't exist!";

    case ParseError::TilesetMissingVersion:
      return "The map featured a tileset without a version attribute!";

    case ParseError::TilesetInvalidVersion:
      return "The map featured a tileset with an invalid version attribute!";

    case ParseError::TilesetMissingTileCount:
      return "The map featured a tileset without a tile count attribute!";

    case ParseError::TilesetMissingColumnCount:
      return "The map featured a tileset without a column count attribute!";

    case ParseError::TilesetMissingImageWidth:
      return "The map featured a tileset without an image width attribute!";

    case ParseError::TilesetMissingImageHeight:
      return "The map featured a tileset without an image height attribute!";

    case ParseError::TileMissingId:
      return "The map featured a tileset with a tile definition without an ID!";

    case ParseError::AnimationFrameNoTile:
      return "The map featured a tileset with a tile animation frame without a tile ID!";

    case ParseError::AnimationFrameNoDuration:
      return "The map featured a tileset with a tile animation frame without a duration!";
  }
}

auto operator<<(std::ostream& stream, const ParseError error) -> std::ostream&
{
  return stream << magic_enum::enum_name(error);
}

}  // namespace Tactile::IO
