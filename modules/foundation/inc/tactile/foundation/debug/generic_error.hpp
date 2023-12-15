// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <system_error>  // error_category, error_code
#include <utility>       // to_underlying

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Provides error codes for generic high-level errors.
 *
 * \see `get_generic_error_category`
 * \see `make_generic_error`
 */
enum class GenericError : int {  // NOLINT(performance-enum-size)
  kUnknown,
  kUnsupported,
  kInvalidArgument,
  kInvalidState,
  kInitFailure,
  kInvalidFile,
  kNoSuchFile,
  kIOError
};

[[nodiscard]]
TACTILE_FOUNDATION_API auto get_generic_error_category() noexcept
    -> const std::error_category&;

[[nodiscard]]
inline auto make_generic_error(const GenericError error) noexcept -> std::error_code
{
  return std::error_code {std::to_underlying(error), get_generic_error_category()};
}

}  // namespace tactile