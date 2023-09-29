// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // to_underlying

#include "tactile/core/api.hpp"
#include "tactile/core/debug/error_code.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Provides error codes used when loading and emitting save files.
 */
class SaveFormatErrorDomain final : public IErrorDomain {
 public:
  enum class Error : uint32 {
    kInvalidOperation,        ///< The format doesn't support the requested operation.
    kUnknown,                 ///< An unknown error occurred.
    kBadFile,                 ///< Something went wrong when loading or saving a file.
    kUnsupportedFormat,       ///< Tried to load/save file with unknown format.
    kMissingKey,              ///< A required attribute was not found in a save file.
    kUnsupportedOrientation,  ///< An unsupported map orientation was detected.
    kUnsupportedLayerType,    ///< An unsupported layer type was detected.
  };

  [[nodiscard]]
  TACTILE_CORE_API auto get_message(uint32 error_id) const noexcept
      -> StringView override;
};

/**
 * \brief The global save format error domain.
 */
inline constexpr SaveFormatErrorDomain kSaveFormatErrorDomain;

using SaveFormatError = SaveFormatErrorDomain::Error;

/**
 * \brief Convenience function for creating a save format error code.
 *
 * \param error a save format error identifier.
 *
 * \return an error code.
 */
[[nodiscard]] inline auto save_format_error(const SaveFormatError error) noexcept
    -> ErrorCode
{
  return ErrorCode {&kSaveFormatErrorDomain, std::to_underlying(error)};
}

}  // namespace tactile
