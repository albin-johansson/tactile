// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>  // strong_ordering

/**
 * \brief Simple macro useful for defining simple (but effective) strong types.
 */
#define TACTILE_STRONG_TYPE(Name, ValueType)                             \
  struct Name final {                                                    \
    using value_type = ValueType;                                        \
                                                                         \
    ValueType value;                                                     \
                                                                         \
    [[nodiscard]] constexpr auto operator==(const Name&) const noexcept  \
        -> bool = default;                                               \
                                                                         \
    [[nodiscard]] constexpr auto operator<=>(const Name&) const noexcept \
        -> std::strong_ordering = default;                               \
  }

#define TACTILE_STRONG_TYPE_ADD_BINARY_OP(Type, Op)                              \
  [[nodiscard]] constexpr auto operator Op(const Type t1, const Type t2) -> Type \
  {                                                                              \
    return Type {t1.value Op t2.value};                                          \
  }                                                                              \
  static_assert(true, "Don't forget the semicolon")