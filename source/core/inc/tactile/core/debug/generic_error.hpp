// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code, error_category
#include <utility>       // to_underlying

namespace tactile {

/** Provides error codes for generic high-level errors. */
enum class GenericError : int
{
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
 * \return
 * An error category.
 */
[[nodiscard]]
auto get_generic_error_category() noexcept -> const std::error_category&;

/**
 * Creates a generic error code.
 *
 * \param error An error code.
 *
 * \return
 * An error code.
 */
inline auto make_error(const GenericError error) noexcept -> std::error_code
{
  return std::error_code {std::to_underlying(error), get_generic_error_category()};
}

}  // namespace tactile
