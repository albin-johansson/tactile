// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <system_error>  // error_code, error_category
#include <type_traits>   // decay_t
#include <utility>       // forward
#include <version>       // __cpp_lib_expected

#if __cpp_lib_expected >= 202211L
  #define TACTILE_HAS_STD_EXPECTED 1
#else
  #define TACTILE_HAS_STD_EXPECTED 0
#endif

#if TACTILE_HAS_STD_EXPECTED
  #include <expected>  // expected, unexpected
#else
  #include <tl/expected.hpp>
#endif

#include "tactile/base/int.hpp"

namespace tactile {

#if TACTILE_HAS_STD_EXPECTED

template <typename T, typename E>
using Expected = std::expected<T, E>;

template <typename E>
using Unexpected = std::unexpected<E>;

#else

template <typename T, typename E>
using Expected = tl::expected<T, E>;

template <typename E>
using Unexpected = tl::unexpected<E>;

#endif

template <typename E>
[[nodiscard]] auto unexpected(E&& e) -> Unexpected<std::decay_t<E>>
{
#if TACTILE_HAS_STD_EXPECTED
  return std::unexpected {std::forward<E>(e)};
#else
  return tl::make_unexpected(std::forward<E>(e));
#endif
}

template <typename T, typename E>
[[nodiscard]] auto propagate_unexpected(const Expected<T, E>& e) -> Unexpected<E>
{
  return unexpected(e.error());
}

using ErrorCode = std::error_code;
using ErrorCategory = std::error_category;

template <typename T>
using Result = Expected<T, ErrorCode>;

inline const Result<void> kOK {};

}  // namespace tactile
