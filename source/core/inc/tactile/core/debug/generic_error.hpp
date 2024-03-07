// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // to_underlying

#include "tactile/base/container/expected.hpp"

namespace tactile {

/** Provides error codes for generic high-level errors. */
enum class GenericError : int {  // NOLINT(*-enum-size)
  kUnknown,
  kUnsupported,
  kOutOfMemory,
  kInvalidParam,
  kInvalidState,
  kInvalidFile,
  kNoSuchFile,
};

/**
 * Returns the error category for generic errors.
 *
 * \return An error category.
 */
[[nodiscard]]
auto get_generic_error_category() noexcept -> const ErrorCategory&;

/**
 * Creates a generic error code.
 *
 * \param error An error code.
 *
 * \return An error code.
 */
inline auto make_error(const GenericError error) noexcept -> ErrorCode
{
  return ErrorCode {std::to_underlying(error), get_generic_error_category()};
}

}  // namespace tactile
