// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>  // strong_ordering

/**
 * \brief Simple macro useful for defining simple (but effective) strong types.
 */
#define TACTILE_STRONG_TYPE(Name, UnderlyingType)                            \
  struct Name final {                                                        \
    UnderlyingType value;                                                    \
                                                                             \
    constexpr auto operator==(const Name&) const noexcept -> bool = default; \
    constexpr auto operator<=>(const Name&) const noexcept                   \
        -> std::strong_ordering = default;                                   \
  }
