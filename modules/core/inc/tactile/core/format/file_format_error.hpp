// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Error codes used when loading map and tileset files.
 */
enum class FileFormatError : uint8 {
  kNone = 0,                ///< No error occurred.
  kInvalidOperation,        ///< The format doesn't support the requested operation.
  kUnknown,                 ///< An unknown error occurred.
  kBadFile,                 ///< Something went wrong when loading or saving a file.
  kUnsupportedFormat,       ///< Tried to load/save file with unknown format.
  kMissingKey,              ///< A required attribute was not found in a save file.
  kUnsupportedOrientation,  ///< An unsupported map orientation was detected.
  kUnsupportedLayerType,    ///< An unsupported layer type was detected.
};

}  // namespace tactile
