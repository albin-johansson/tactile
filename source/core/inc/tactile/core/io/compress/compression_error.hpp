// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code, error_category
#include <utility>       // to_underlying

#include "tactile/base/int.hpp"

namespace tactile {

/**
 * Provides error codes for errors related to compression providers.
 */
enum class CompressionError : int
{
  kBadInit,
  kBadCleanup,
  kBadState,
  kCouldNotCompress,
  kCouldNotDecompress,
};

/**
 * Returns the error category for compression errors.
 *
 * \return
 * An error category.
 */
[[nodiscard]]
auto get_compression_error_category() noexcept -> const std::error_category&;

/**
 * Creates a generic error code.
 *
 * \param error An error code.
 *
 * \return
 * An error code.
 */
inline auto make_error(const CompressionError error) noexcept -> std::error_code
{
  return std::error_code {std::to_underlying(error), get_compression_error_category()};
}

}  // namespace tactile
