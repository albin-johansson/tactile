// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // invocable
#include <expected>  // expected, unexpected
#include <utility>   // move

#include "tactile/core/basic/errc.hpp"
#include "tactile/core/basic/vector.hpp"

namespace tactile {

template <typename T>
using Result = std::expected<T, Errc>;

/// Returns a valid void result.
[[nodiscard]]
constexpr auto ok() noexcept -> Result<void>
{
  return Result<void> {};
}

/// Creates an unexpected result from a given error code.
[[nodiscard]]
constexpr auto error(const Errc errc) noexcept -> std::unexpected<Errc>
{
  return std::unexpected {errc};
}

/// Propagates the error stored in a result.
template <typename T>
[[nodiscard]] constexpr auto propagate_error(const Result<T>& result) noexcept
    -> std::unexpected<Errc>
{
  return error(result.error());
}

/// Invokes a fallible callable for each element in a container, storing the results.
template <typename T,
          typename Container,
          std::invocable<typename Container::const_reference> Action>
[[nodiscard]] constexpr auto collect_results(const Container& container,
                                             const Action& action)
    -> Result<Vector<T>>
{
  Vector<T> values {};
  values.reserve(container.size());

  for (const auto& container_entry : container) {
    Result<T> value = action(container_entry);
    if (value.has_value()) {
      values.push_back(std::move(*value));
    }
    else {
      return propagate_error(value);
    }
  }

  return values;
}

}  // namespace tactile
