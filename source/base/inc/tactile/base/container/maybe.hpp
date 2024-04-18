// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>  // optional, nullopt_t, nullopt, make_optional
#include <utility>   // forward

namespace tactile {

template <typename T>
using Maybe = std::optional<T>;

/**
 * Creates an optional with a given value.
 *
 * \tparam T The value type.
 *
 * \param value The value that will be stored.
 *
 * \return
 * The created optional.
 */
template <typename T>
[[nodiscard]] constexpr auto some(T&& value) -> Maybe<T>
{
  return std::make_optional(std::forward<T>(value));
}

/**
 * Creates an optional with a given value.
 *
 * \tparam T    The value type.
 * \tparam Args The argument types.
 *
 * \param args The arguments that will be forwarded to a constructor.
 *
 * \return
 * The created optional.
 */
template <typename T, typename... Args>
[[nodiscard]] constexpr auto some(Args&&... args) -> Maybe<T>
{
  return std::make_optional(std::forward<T>(args)...);
}

inline constexpr std::nullopt_t kNone = std::nullopt;
inline constexpr std::nullopt_t nothing = std::nullopt;  // NOLINT

}  // namespace tactile
