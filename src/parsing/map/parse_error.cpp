#include "parse_error.hpp"

#include "tactile_qstring.hpp"

namespace tactile::parse {

auto ToMessage(ParseError error) -> QString
{
  switch (error)
  {
    case ParseError::none:
      return TACTILE_QSTRING(u"No error!");

    case ParseError::map_file_not_found:
      return TACTILE_QSTRING(u"Didn't find map file!");

    case ParseError::could_not_parse_file:
      return TACTILE_QSTRING(u"Couldn't parse file!");

    case ParseError::map_missing_next_layer_id:
      return TACTILE_QSTRING(u"Map didn't feature next layer ID property!");

    case ParseError::map_missing_tilesets:
      return TACTILE_QSTRING(u"Map didn't feature tilesets property!");

    case ParseError::map_missing_layers:
      return TACTILE_QSTRING(u"Map didn't feature layers property!");

    case ParseError::layer_missing_width:
      return TACTILE_QSTRING(u"Layer object was missing width!");

    case ParseError::layer_missing_height:
      return TACTILE_QSTRING(u"Layer object was missing height!");

    case ParseError::layer_missing_type:
      return TACTILE_QSTRING(u"Layer object was missing type!");

    case ParseError::layer_missing_id:
      return TACTILE_QSTRING(u"Layer object was missing ID!");

    case ParseError::layer_could_not_parse_tile:
      return TACTILE_QSTRING(u"Couldn't parse tile ID in layer!");

    case ParseError::tileset_missing_first_gid:
      return TACTILE_QSTRING(u"Tileset didn't feature first GID property!");

    case ParseError::tileset_missing_tile_width:
      return TACTILE_QSTRING(u"Tileset didn't feature tile width property!");

    case ParseError::tileset_missing_tile_height:
      return TACTILE_QSTRING(u"Tileset didn't feature tile height property!");

    case ParseError::tileset_missing_image_path:
      return TACTILE_QSTRING(u"Tileset didn't feature image property!");

    case ParseError::tileset_missing_name:
      return TACTILE_QSTRING(u"Tileset didn't feature name property!");

    case ParseError::could_not_create_tileset:
      return TACTILE_QSTRING(u"Couldn't create tileset object!");

    case ParseError::could_not_read_external_tileset:
      return TACTILE_QSTRING(u"Couldn't read external tileset file!");

    case ParseError::external_tileset_does_not_exist:
      return TACTILE_QSTRING(u"External tileset file doesn't exist!");

    case ParseError::unknown_property_type:
      return TACTILE_QSTRING(u"Couldn't recognize property type!");

    case ParseError::unknown_layer_type:
      return TACTILE_QSTRING(u"Couldn't recognize layer type!");

    case ParseError::object_missing_id:
      return TACTILE_QSTRING(u"Object was missing ID!");

    default:
      return TACTILE_QSTRING(u"Unknown!");
  }
}

}  // namespace tactile::parse
