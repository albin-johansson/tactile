#pragma once

#include <QString>

namespace tactile {

/**
 * \enum parse_error
 *
 * \brief Provides values for with potential map parsing errors.
 *
 * \since 0.1.0
 */
enum class parse_error
{
  none,  ///< No error occurred.

  map_file_not_found,    ///< The main map file was not found.
  could_not_parse_json,  ///< Couldn't parse the contents of the JSON file.
  could_not_parse_tmx,   ///< Couldn't parse the contents of the TMX file.

  map_missing_next_layer_id,  ///< Map did not feature "nextlayerid".
  map_missing_tilesets,       ///< Map did not feature "tilesets".
  map_missing_layers,         ///< Map did not feature "layers".

  layer_missing_width,         ///< Layer did not feature "width".
  layer_missing_height,        ///< Layer did not feature "height".
  layer_missing_type,          ///< Layer did not feature "type".
  layer_missing_id,            ///< Layer did not feature "id".
  layer_could_not_parse_tile,  ///< Failed to parse layer tile value.

  tileset_missing_first_gid,        ///< Tileset did not feature "firstgid".
  tileset_missing_tile_width,       ///< Tileset did not feature "tilewidth".
  tileset_missing_tile_height,      ///< Tileset did not feature "tileheight".
  tileset_missing_image_path,       ///< Tileset did not feature "image".
  tileset_missing_name,             ///< Tileset did not feature "name".
  could_not_create_tileset,         ///< Couldn't create `tileset` object.
  could_not_read_external_tileset,  ///< Could not read external tileset file.
  external_tileset_does_not_exist   ///< External tileset file does not exist.
};

[[nodiscard]] auto to_message(parse_error error) -> QString;

}  // namespace tactile
