// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/debug/error_code.hpp"
#include "tactile/foundation/functional/expected.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Provides error codes related to data compression/decompression.
 */
class TACTILE_FOUNDATION_API CompressionErrorDomain final : public IErrorDomain {
 public:
  enum class Error : uint32 {
    kNoData,         ///< No data to process.
    kInternalError,  ///< An error occurred in an internal stage (such as a library call).
    kInvalidMode,    ///< An invalid compression mode was used.
  };

  constexpr CompressionErrorDomain() noexcept = default;

  constexpr ~CompressionErrorDomain() noexcept override  // NOLINT: GCC workaround
  {}

  TACTILE_DEFAULT_COPY(CompressionErrorDomain);
  TACTILE_DEFAULT_MOVE(CompressionErrorDomain);

  [[nodiscard]]
  TACTILE_FOUNDATION_API auto get_message(uint32 error_id) const noexcept
      -> StringView override;
};

using CompressionError = CompressionErrorDomain::Error;

/**
 * \brief Convenience function for creating a compression error code.
 *
 * \param error a compression error identifier.
 *
 * \return an error code.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto error(CompressionError error) noexcept
    -> Unexpected<ErrorCode>;

}  // namespace tactile
