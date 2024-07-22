// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Provides identifiers for common save format parse errors.
 */
enum class SaveFormatParseError : uint8
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
};

}  // namespace tactile
