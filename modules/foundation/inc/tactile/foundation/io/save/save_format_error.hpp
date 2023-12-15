// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <system_error>  // error_category, error_code
#include <utility>       // to_underlying

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/functional/expected.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents error codes used when loading and emitting save files.
 */
enum class SaveFormatError : int {  // NOLINT(performance-enum-size)
  kInvalidOperation,   ///< The format doesn't support the requested operation.
  kUnknown,            ///< An unknown error occurred.
  kBadFile,            ///< Something went wrong when loading or saving a file.
  kUnsupportedFormat,  ///< Tried to load/save file with unknown format.
  kMissingKey,         ///< A required attribute was not found in a save file.
  kUnsupportedVersion,
  kUnsupportedOrientation,
  kUnsupportedLayerType,
  kUnsupportedObjectType,
  kUnsupportedPropertyType,
  kUnsupportedTileEncoding,
  kUnsupportedCompression,
  kBadTileLayerData,
  kBadPropertyValue,
  kPropertyWithoutValue,
  kInvalidComponentType,
  kInvalidTileFormat
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
