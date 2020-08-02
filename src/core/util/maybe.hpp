#pragma once
#include <optional>

namespace tactile {

/**
 * An alias for <code>std::optional</code>, because Haskell is awesome.
 *
 * @since 0.1.0
 */
template <typename T>
using Maybe = std::optional<T>;

/**
 * A constant that can be used instead of <code>std::nullopt</code>.
 *
 * @since 0.1.0
 */
[[deprecated("Use std::nullopt instead!")]] inline constexpr auto std::nullopt =
    std::nullopt;

}  // namespace tactile