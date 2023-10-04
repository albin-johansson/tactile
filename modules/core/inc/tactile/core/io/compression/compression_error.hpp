// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // to_underlying

#include "tactile/core/api.hpp"
#include "tactile/core/debug/error_code.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Provides error codes related to data compression/decompression.
 */
class CompressionErrorDomain final : public IErrorDomain {
 public:
  enum class Error : uint32 {
    kNoData,         ///< No data to process.
    kInternalError,  ///< An error occurred in an internal stage (such as a library call).
  };

  [[nodiscard]]
  TACTILE_CORE_API auto get_message(uint32 error_id) const noexcept
      -> StringView override;
};

using CompressionError = CompressionErrorDomain::Error;

/**
 * \brief The global compression error domain.
 */
inline constexpr CompressionErrorDomain kCompressionErrorDomain;

/**
 * \brief Convenience function for creating a compression error code.
 *
 * \param error a compression error identifier.
 *
 * \return an error code.
 */
[[nodiscard]] inline auto compression_error(const CompressionError error) noexcept
    -> ErrorCode
{
  return ErrorCode {&kCompressionErrorDomain, std::to_underlying(error)};
}

}  // namespace tactile
