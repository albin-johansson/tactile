#pragma once

#include <type_traits>

namespace tactile {

/**
 * Indicates whether or not a type is final, movable and copyable.
 *
 * @tparam T the type that will be checked.
 * @return true if the type is final, movable and copyable; false otherwise.
 * @since 0.1.0
 */
template <typename T>
inline constexpr auto validate() noexcept -> bool
{
  return std::is_final_v<T> && std::is_move_constructible_v<T> &&
         std::is_copy_constructible_v<T> && std::is_move_assignable_v<T> &&
         std::is_copy_assignable_v<T>;
}

/**
 * Indicates whether or not a type is final, nothrow movable and
 * nothrow copyable.
 *
 * @tparam T the type that will be checked.
 * @return true if the type is final, nothrow movable and nothrow copyable;
 * false
 * otherwise.
 * @since 0.1.0
 */
template <typename T>
inline constexpr auto validate_nothrow() noexcept -> bool
{
  return std::is_final_v<T> && std::is_nothrow_move_constructible_v<T> &&
         std::is_nothrow_copy_constructible_v<T> &&
         std::is_nothrow_move_assignable_v<T> &&
         std::is_nothrow_copy_assignable_v<T>;
}

}  // namespace tactile
