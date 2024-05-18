// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected, unexpected
#include <system_error>  // error_code, error_category
#include <type_traits>   // decay_t
#include <utility>       // forward

#include "tactile/base/int.hpp"

namespace tactile {

template <typename T, typename E>
using Expected = std::expected<T, E>;

template <typename E>
using Unexpected = std::unexpected<E>;

template <typename E>
[[nodiscard]] auto unexpected(E&& e) -> Unexpected<std::decay_t<E>>
{
  return std::unexpected {std::forward<E>(e)};
}

template <typename T, typename E>
[[nodiscard]] auto propagate_unexpected(const Expected<T, E>& e)
    -> Unexpected<E>
{
  return unexpected(e.error());
}

using ErrorCode = std::error_code;
using ErrorCategory = std::error_category;

template <typename T>
using Result = Expected<T, ErrorCode>;

inline const Result<void> kOK {};

}  // namespace tactile
