#pragma once

#include <QString>

namespace tactile::parse {

/**
 * \enum parse_error
 *
 * \brief Provides values for with potential map parsing errors.
 *
 * \note Remember to add an error message for each new enumerator in
 * `to_message()`!
 *
 * \since 0.1.0
 */
enum class parse_error
{
  none,  ///< No error occurred.

  map_file_not_found,    ///< The main map file was not found.
  could_not_parse_file,  ///< Couldn't parse the contents of the map file.

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
  external_tileset_does_not_exist,  ///< External tileset file does not exist.

  unknown_property_type,  ///< Did not recognize property type.
  unknown_layer_type,     ///< Did not recognize layer type.

  object_missing_id  ///< Object did not feature ID attribute.
};

/**
 * \brief Returns an error message for an error code, intended for the user.
 *
 * \param error the error code to obtain an error message for.
 *
 * \return an error message that explains the error.
 *
 * \since 0.2.0
 */
[[nodiscard]] auto to_message(parse_error error) -> QString;

}  // namespace tactile::parse
