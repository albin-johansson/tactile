// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <system_error>  // error_category, error_code
#include <utility>       // to_underlying

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

enum class CompressionError : int {
  kNoData,         ///< No data to process.
  kInternalError,  ///< An error occurred in an internal stage (such as a library call).
  kInvalidMode,    ///< An invalid compression mode was used.
};

[[nodiscard]]
TACTILE_FOUNDATION_API auto get_compression_error_category() noexcept
    -> const std::error_category&;

[[nodiscard]]
inline auto make_compression_error(const CompressionError error) noexcept
    -> std::error_code
{
  return std::error_code {std::to_underlying(error), get_compression_error_category()};
}

}  // namespace tactile
