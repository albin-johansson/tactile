// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // to_underlying

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/functional/expected.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents error codes used when loading and emitting save files.
 */
enum class SaveFormatError : int {
  kInvalidOperation,            ///< The format doesn't support the requested operation.
  kUnknown,                     ///< An unknown error occurred.
  kBadFile,                     ///< Something went wrong when loading or saving a file.
  kUnsupportedFormat,           ///< Tried to load/save file with unknown format.
  kMissingKey,                  ///< A required attribute was not found in a save file.
  kUnsupportedOrientation,      ///< An unsupported map orientation was detected.
  kUnsupportedLayerType,        ///< An unsupported layer type was detected.
  kUnsupportedPropertyType,     ///< An unsupported property type was detected.
  kUnsupportedTileEncoding,     ///< An unsupported tile data encoding was detected.
  kUnsupportedCompressionMode,  ///< An unsupported compression mode was detected.
  kBadColorProperty,            ///< An invalid color property value was detected.
  kBadTileLayerData,            ///< Corrupt tile layer data was detected.
  kBadCompressionMode,          ///< An invalid compression mode was detected.
};

[[nodiscard]]
TACTILE_FOUNDATION_API auto get_save_format_error_category() noexcept
    -> const std::error_category&;

[[nodiscard]]
inline auto make_save_format_error(const SaveFormatError error) noexcept
    -> std::error_code
{
  return std::error_code {std::to_underlying(error), get_save_format_error_category()};
}

}  // namespace tactile
