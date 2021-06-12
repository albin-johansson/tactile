#pragma once

#include <QString>  // QString

namespace tactile {

/**
 * \enum ParseError
 *
 * \brief Provides values for with potential map parsing errors.
 *
 * \note Remember to add an error message for each new enumerator in
 * `ToMessage()`!
 *
 * \since 0.1.0
 */
enum class ParseError
{
  None,  ///< No error occurred.

  MapFileNotFound,    ///< The main map file was not found.
  CouldNotParseFile,  ///< Couldn't parse the contents of the map file.

  MapMissingNextLayerId,  ///< Map did not feature "nextlayerid".
  MapMissingTilesets,     ///< Map did not feature "tilesets".
  MapMissingLayers,       ///< Map did not feature "layers".

  LayerMissingWidth,       ///< Layer did not feature "width".
  LayerMissingHeight,      ///< Layer did not feature "height".
  LayerMissingType,        ///< Layer did not feature "type".
  LayerMissingId,          ///< Layer did not feature "id".
  LayerCouldNotParseTile,  ///< Failed to parse layer tile value.

  TilesetMissingFirstGid,       ///< Tileset did not feature "firstgid".
  TilesetMissingTileWidth,      ///< Tileset did not feature "tilewidth".
  TilesetMissingTileHeight,     ///< Tileset did not feature "tileheight".
  TilesetMissingImagePath,      ///< Tileset did not feature "image".
  TilesetMissingName,           ///< Tileset did not feature "name".
  CouldNotCreateTileset,        ///< Couldn't create `tileset` object.
  CouldNotReadExternalTileset,  ///< Could not read external tileset file.
  ExternalTilesetDoesNotExist,  ///< External tileset file does not exist.

  UnknownPropertyType,  ///< Did not recognize property type.
  UnknownLayerType,     ///< Did not recognize layer type.

  ObjectMissingId  ///< Object did not feature ID attribute.
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
[[nodiscard]] auto ToMessage(ParseError error) -> QString;

}  // namespace tactile
