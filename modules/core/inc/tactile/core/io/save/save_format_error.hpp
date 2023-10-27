// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/debug/error_code.hpp"
#include "tactile/core/functional/expected.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Provides error codes used when loading and emitting save files.
 */
class TACTILE_CORE_API SaveFormatErrorDomain final : public IErrorDomain {
 public:
  enum class Error : uint32 {
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

  constexpr SaveFormatErrorDomain() noexcept = default;

  constexpr ~SaveFormatErrorDomain() noexcept override  // NOLINT: GCC workaround
  {}

  TACTILE_DEFAULT_COPY(SaveFormatErrorDomain);
  TACTILE_DEFAULT_MOVE(SaveFormatErrorDomain);

  [[nodiscard]]
  auto get_message(uint32 error_id) const noexcept -> StringView override;
};

using SaveFormatError = SaveFormatErrorDomain::Error;

/**
 * \brief Convenience function for creating a save format error code.
 *
 * \param error a save format error identifier.
 *
 * \return an error code.
 */
[[nodiscard]]
TACTILE_CORE_API auto error(SaveFormatError error) noexcept -> Unexpected<ErrorCode>;

}  // namespace tactile
