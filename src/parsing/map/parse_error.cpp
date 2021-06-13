#include "parse_error.hpp"

namespace tactile {

auto ToMessage(const ParseError error) -> QString
{
  switch (error)
  {
    case ParseError::None:
      return QStringLiteral(u"No error!");

    case ParseError::MapFileNotFound:
      return QStringLiteral(u"Didn't find map file!");

    case ParseError::CouldNotParseFile:
      return QStringLiteral(u"Couldn't parse file!");

    case ParseError::MapMissingNextLayerId:
      return QStringLiteral(u"Map didn't feature next layer ID property!");

    case ParseError::MapMissingTilesets:
      return QStringLiteral(u"Map didn't feature tilesets property!");

    case ParseError::MapMissingLayers:
      return QStringLiteral(u"Map didn't feature layers property!");

    case ParseError::LayerMissingWidth:
      return QStringLiteral(u"Layer object was missing width!");

    case ParseError::LayerMissingHeight:
      return QStringLiteral(u"Layer object was missing height!");

    case ParseError::LayerMissingType:
      return QStringLiteral(u"Layer object was missing type!");

    case ParseError::LayerMissingId:
      return QStringLiteral(u"Layer object was missing ID!");

    case ParseError::LayerCouldNotParseTile:
      return QStringLiteral(u"Couldn't parse tile ID in layer!");

    case ParseError::TilesetMissingFirstGid:
      return QStringLiteral(u"Tileset didn't feature first GID property!");

    case ParseError::TilesetMissingTileWidth:
      return QStringLiteral(u"Tileset didn't feature tile width property!");

    case ParseError::TilesetMissingTileHeight:
      return QStringLiteral(u"Tileset didn't feature tile height property!");

    case ParseError::TilesetMissingImagePath:
      return QStringLiteral(u"Tileset didn't feature image property!");

    case ParseError::TilesetMissingName:
      return QStringLiteral(u"Tileset didn't feature name property!");

    case ParseError::CouldNotCreateTileset:
      return QStringLiteral(u"Couldn't create tileset object!");

    case ParseError::CouldNotReadExternalTileset:
      return QStringLiteral(u"Couldn't read external tileset file!");

    case ParseError::ExternalTilesetDoesNotExist:
      return QStringLiteral(u"External tileset file doesn't exist!");

    case ParseError::UnknownPropertyType:
      return QStringLiteral(u"Couldn't recognize property type!");

    case ParseError::UnknownLayerType:
      return QStringLiteral(u"Couldn't recognize layer type!");

    case ParseError::ObjectMissingId:
      return QStringLiteral(u"Object was missing ID!");

    default:
      return QStringLiteral(u"Unknown!");
  }
}

}  // namespace tactile
