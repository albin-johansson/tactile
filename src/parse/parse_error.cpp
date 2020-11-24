#include "parse_error.hpp"

namespace tactile {

auto to_message(const parse_error error) -> QString
{
  switch (error) {
    case parse_error::none:
      return QStringLiteral(u"No error!");

    case parse_error::map_file_not_found:
      return QStringLiteral(u"Did not find map file!");

    case parse_error::could_not_parse_json:
      return QStringLiteral(u"Could not parse JSON file!");

    case parse_error::could_not_parse_tmx:
      return QStringLiteral(u"Could not parse TMX file!");

    case parse_error::map_missing_next_layer_id:
      return QStringLiteral(u"Map did not feature next layer ID property!");

    case parse_error::map_missing_tilesets:
      return QStringLiteral(u"Map did not feature tilesets property!");

    case parse_error::map_missing_layers:
      return QStringLiteral(u"Map did not feature layers property!");

    case parse_error::layer_missing_width:
      return QStringLiteral(u"Layer object was missing width!");

    case parse_error::layer_missing_height:
      return QStringLiteral(u"Layer object was missing height!");

    case parse_error::layer_missing_type:
      return QStringLiteral(u"Layer object was missing type!");

    case parse_error::layer_missing_id:
      return QStringLiteral(u"Layer object was missing ID!");

    case parse_error::layer_could_not_parse_tile:
      return QStringLiteral(u"Could not parse tile ID in layer!");

    case parse_error::tileset_missing_first_gid:
      return QStringLiteral(u"Tileset did not feature first GID property!");

    case parse_error::tileset_missing_tile_width:
      return QStringLiteral(u"Tileset did not feature tile width property!");

    case parse_error::tileset_missing_tile_height:
      return QStringLiteral(u"Tileset did not feature tile height property!");

    case parse_error::tileset_missing_image_path:
      return QStringLiteral(u"Tileset did not feature image property!");

    case parse_error::tileset_missing_name:
      return QStringLiteral(u"Tileset did not feature name property!");

    case parse_error::could_not_create_tileset:
      return QStringLiteral(u"Could not create tileset object!");

    case parse_error::could_not_read_external_tileset:
      return QStringLiteral(u"Could not read external tileset file!");

    case parse_error::external_tileset_does_not_exist:
      return QStringLiteral(u"External tileset file does not exist!");

    default:
      return QStringLiteral(u"Unknown!");
  }
}

}  // namespace tactile
