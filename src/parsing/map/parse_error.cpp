#include "parse_error.hpp"

#include "tactile_qstring.hpp"

namespace tactile::parse {

auto ToMessage(ParseError error) -> QString
{
  switch (error)
  {
    case ParseError::None:
      return TACTILE_QSTRING(u"No error!");

    case ParseError::MapFileNotFound:
      return TACTILE_QSTRING(u"Didn't find map file!");

    case ParseError::CouldNotParseFile:
      return TACTILE_QSTRING(u"Couldn't parse file!");

    case ParseError::MapMissingNextLayerId:
      return TACTILE_QSTRING(u"Map didn't feature next layer ID property!");

    case ParseError::MapMissingTilesets:
      return TACTILE_QSTRING(u"Map didn't feature tilesets property!");

    case ParseError::MapMissingLayers:
      return TACTILE_QSTRING(u"Map didn't feature layers property!");

    case ParseError::LayerMissingWidth:
      return TACTILE_QSTRING(u"Layer object was missing width!");

    case ParseError::LayerMissingHeight:
      return TACTILE_QSTRING(u"Layer object was missing height!");

    case ParseError::LayerMissingType:
      return TACTILE_QSTRING(u"Layer object was missing type!");

    case ParseError::LayerMissingId:
      return TACTILE_QSTRING(u"Layer object was missing ID!");

    case ParseError::LayerCouldNotParseTile:
      return TACTILE_QSTRING(u"Couldn't parse tile ID in layer!");

    case ParseError::TilesetMissingFirstGid:
      return TACTILE_QSTRING(u"Tileset didn't feature first GID property!");

    case ParseError::TilesetMissingTileWidth:
      return TACTILE_QSTRING(u"Tileset didn't feature tile width property!");

    case ParseError::TilesetMissingTileHeight:
      return TACTILE_QSTRING(u"Tileset didn't feature tile height property!");

    case ParseError::TilesetMissingImagePath:
      return TACTILE_QSTRING(u"Tileset didn't feature image property!");

    case ParseError::TilesetMissingName:
      return TACTILE_QSTRING(u"Tileset didn't feature name property!");

    case ParseError::CouldNotCreateTileset:
      return TACTILE_QSTRING(u"Couldn't create tileset object!");

    case ParseError::CouldNotReadExternalTileset:
      return TACTILE_QSTRING(u"Couldn't read external tileset file!");

    case ParseError::ExternalTilesetDoesNotExist:
      return TACTILE_QSTRING(u"External tileset file doesn't exist!");

    case ParseError::UnknownPropertyType:
      return TACTILE_QSTRING(u"Couldn't recognize property type!");

    case ParseError::UnknownLayerType:
      return TACTILE_QSTRING(u"Couldn't recognize layer type!");

    case ParseError::ObjectMissingId:
      return TACTILE_QSTRING(u"Object was missing ID!");

    default:
      return TACTILE_QSTRING(u"Unknown!");
  }
}

}  // namespace tactile::parse
