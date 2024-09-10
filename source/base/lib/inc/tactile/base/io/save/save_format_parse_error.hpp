// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>      // uint8_t
#include <string_view>  // string_view

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Provides identifiers for common save format parse errors.
 */
enum class SaveFormatParseError : std::uint8_t
{
  kNoPropertyName,
  kNoPropertyType,
  kNoPropertyValue,
  kBadPropertyType,
  kBadPropertyValue,

  kNoObjectId,
  kNoObjectName,
  kNoObjectX,
  kNoObjectY,
  kNoObjectWidth,
  kNoObjectHeight,
  kNoObjectVisibility,

  kNoLayerId,
  kNoLayerName,
  kNoLayerOpacity,
  kNoLayerVisibility,
  kNoLayerType,
  kBadLayerType,

  kNoTileLayerWidth,
  kNoTileLayerHeight,
  kNoTileLayerData,
  kBadTileLayerEncoding,
  kBadTileLayerData,

  kNoGroupLayerData,

  kNoSuchCompressionFormat,

  kNoTilesetFirstTileId,
  kNoTilesetName,
  kNoTilesetTileWidth,
  kNoTilesetTileHeight,
  kNoTilesetTileCount,
  kNoTilesetColumns,
  kNoTilesetImageWidth,
  kNoTilesetImageHeight,
  kNoTilesetImage,
  kNoSuchTilesetImage,

  kNoTileIndex,

  kNoAnimationFrameTileIndex,
  kNoAnimationFrameDuration,

  kNoMapOrientation,
  kBadMapOrientation,
  kNoMapWidth,
  kNoMapHeight,
  kNoMapTileWidth,
  kNoMapTileHeight,
  kNoMapNextLayerId,
  kNoMapNextObjectId,
};

/**
 * Returns a short human-readable description of a parse error.
 *
 * \param error The save format parse error.
 *
 * \return
 * An error description.
 */
[[nodiscard]]
constexpr auto to_string(const SaveFormatParseError error) noexcept -> std::string_view
{
  switch (error) {
    case SaveFormatParseError::kNoPropertyName:          return "missing property name";
    case SaveFormatParseError::kNoPropertyType:          return "missing property type";
    case SaveFormatParseError::kNoPropertyValue:         return "missing property value";
    case SaveFormatParseError::kBadPropertyType:         return "bad property type";
    case SaveFormatParseError::kBadPropertyValue:        return "bad property value";
    case SaveFormatParseError::kNoObjectId:              return "missing object identifier";
    case SaveFormatParseError::kNoObjectName:            return "missing object name";
    case SaveFormatParseError::kNoObjectX:               return "missing object x-coordinate";
    case SaveFormatParseError::kNoObjectY:               return "missing object y-coordinate";
    case SaveFormatParseError::kNoObjectWidth:           return "missing object width";
    case SaveFormatParseError::kNoObjectHeight:          return "missing object height";
    case SaveFormatParseError::kNoObjectVisibility:      return "missing object visibility";
    case SaveFormatParseError::kNoLayerId:               return "missing layer identifier";
    case SaveFormatParseError::kNoLayerName:             return "missing layer name";
    case SaveFormatParseError::kNoLayerOpacity:          return "missing layer opacity";
    case SaveFormatParseError::kNoLayerVisibility:       return "missing layer visibility";
    case SaveFormatParseError::kNoLayerType:             return "missing layer type";
    case SaveFormatParseError::kBadLayerType:            return "bad layer type";
    case SaveFormatParseError::kNoTileLayerWidth:        return "missing tile layer width";
    case SaveFormatParseError::kNoTileLayerHeight:       return "missing tile layer height";
    case SaveFormatParseError::kNoTileLayerData:         return "missing tile layer data";
    case SaveFormatParseError::kBadTileLayerEncoding:    return "bad tile layer encoding";
    case SaveFormatParseError::kBadTileLayerData:        return "bad tile layer data";
    case SaveFormatParseError::kNoGroupLayerData:        return "missing group layer data";
    case SaveFormatParseError::kNoSuchCompressionFormat: return "no such compression format";
    case SaveFormatParseError::kNoTilesetFirstTileId:
      return "missing first tileset tile identifier";
    case SaveFormatParseError::kNoTilesetName:             return "missing tileset name";
    case SaveFormatParseError::kNoTilesetTileWidth:        return "missing tileset tile width";
    case SaveFormatParseError::kNoTilesetTileHeight:       return "missing tileset tile height";
    case SaveFormatParseError::kNoTilesetTileCount:        return "missing tileset tile count";
    case SaveFormatParseError::kNoTilesetColumns:          return "missing tileset column count";
    case SaveFormatParseError::kNoTilesetImageWidth:       return "missing tileset image width";
    case SaveFormatParseError::kNoTilesetImageHeight:      return "missing tileset image height";
    case SaveFormatParseError::kNoTilesetImage:            return "missing tileset image";
    case SaveFormatParseError::kNoSuchTilesetImage:        return "no such tileset image";
    case SaveFormatParseError::kNoTileIndex:               return "missing tile index";
    case SaveFormatParseError::kNoAnimationFrameTileIndex: return "missing frame tile index";
    case SaveFormatParseError::kNoAnimationFrameDuration:  return "missing frame duration";
    case SaveFormatParseError::kNoMapOrientation:          return "missing orientation";
    case SaveFormatParseError::kBadMapOrientation:         return "bad orientation";
    case SaveFormatParseError::kNoMapWidth:                return "missing map width";
    case SaveFormatParseError::kNoMapHeight:               return "missing map height";
    case SaveFormatParseError::kNoMapTileWidth:            return "missing map tile width";
    case SaveFormatParseError::kNoMapTileHeight:           return "missing map tile height";
    case SaveFormatParseError::kNoMapNextLayerId:          return "missing map next layer identifier";
    case SaveFormatParseError::kNoMapNextObjectId:         return "missing map next object identifier";
  }

  return "?";
}

}  // namespace tactile
