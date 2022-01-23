#ifndef TACTILE_IO_PARSE_ERROR_HPP_
#define TACTILE_IO_PARSE_ERROR_HPP_

#include <ostream>  // ostream

#include "api.hpp"
#include "ir_common.hpp"

namespace tactile::IO {

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
enum class ParseError {
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

  ComponentDefMissingName,            // Tactile YAML only
  ComponentDefAttributeMissingName,   // Tactile YAML only
  ComponentDefAttributeMissingType,   // Tactile YAML only
  ComponentDefAttributeInvalidType,   // Tactile YAML only
  ComponentDefAttributeInvalidValue,  // Tactile YAML only

  ComponentMissingType,                  // Tactile YAML only
  ComponentMissingValues,                // Tactile YAML only
  ComponentAttributeMissingName,         // Tactile YAML only
  ComponentAttributeMissingValue,        // Tactile YAML only
  CouldNotParseComponentAttributeValue,  // Tactile YAML only

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
TACTILE_IO_API_QUERY
auto GetCause(ParseError error) -> CStr;

/**
 * \brief Prints a textual representation of a parse error enumerator.
 *
 * \param stream the output stream that will be used.
 * \param error the error code that will be printed.
 *
 * \return the used stream.
 */
TACTILE_IO_API
auto operator<<(std::ostream& stream, ParseError error) -> std::ostream&;

/// \} End of group io

}  // namespace tactile::IO

#endif  // TACTILE_IO_PARSE_ERROR_HPP_
