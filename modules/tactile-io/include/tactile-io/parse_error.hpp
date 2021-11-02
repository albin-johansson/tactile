#pragma once

#include <ostream>  // ostream

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/// \addtogroup io
/// \{

/**
 * \brief Provides various map parsing error codes.
 *
 * \details Enumerators of this type can subsequently be converted into human-readable
 * error descriptions for more information about possible causes of various errors.
 *
 * \see `GetCause()`
 */
enum class ParseError
{
  None,
  Unknown,
  CouldNotReadFile,

  MapUnsupportedExtension,
  MapUnsupportedOrientation,
  MapUnsupportedInfinite,
  MapDoesNotExist,
  MapMissingNextLayerId,
  MapMissingNextObjectId,
  MapMissingTilesets,
  MapMissingLayers,
  MapMissingTileWidth,
  MapMissingTileHeight,
  MapMissingWidth,
  MapMissingHeight,

  LayerMissingWidth,
  LayerMissingHeight,
  LayerMissingType,
  LayerMissingId,
  LayerUnknownType,

  CouldNotParseTiles,
  UnsupportedTileEncoding,

  ExternalTilesetDoesNotExist,
  CouldNotReadExternalTileset,
  TilesetMissingFirstGid,
  TilesetMissingTileWidth,
  TilesetMissingTileHeight,
  TilesetMissingImagePath,
  TilesetMissingName,
  TilesetImageDoesNotExist,

  CouldNotParseProperty,
  PropertyMissingName,
  PropertyMissingType,
  PropertyUnknownType,

  ObjectMissingId,
  ObjectInvalidType,  // Tactile YAML only
  ObjectMissingType,  // Tactile YAML only

  MissingTilesetPath,     // Tactile YAML only
  TilesetMissingVersion,  // Tactile YAML only
  TilesetInvalidVersion,  // Tactile YAML only
  TilesetMissingTileCount,
  TilesetMissingColumnCount,
  TilesetMissingImageWidth,
  TilesetMissingImageHeight,

  TileMissingId,
  AnimationFrameNoTile,
  AnimationFrameNoDuration
};

/**
 * \brief Returns a potential cause of the specified parse error.
 *
 * \details The returned string is meant to help the user to identify the potential
 * cause of a parse error.
 *
 * \param error the error code to obtain a potential cause for.
 *
 * \return a short explanation of the potential cause of the error.
 */
[[nodiscard]] TACTILE_IO_API auto GetCause(ParseError error) -> CStr;

/**
 * \brief Prints a textual representation of a parse error enumerator.
 *
 * \param stream the output stream that will be used.
 * \param error the error code that will be printed.
 *
 * \return the used stream.
 */
TACTILE_IO_API auto operator<<(std::ostream& stream, ParseError error) -> std::ostream&;

/// \} End of group io

}  // namespace Tactile::IO
