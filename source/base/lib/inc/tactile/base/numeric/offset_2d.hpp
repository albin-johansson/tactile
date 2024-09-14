// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>     // strong_ordering
#include <cstddef>     // size_t, ptrdiff_t
#include <format>      // formatter, format_to, formattable
#include <functional>  // hash

#include "tactile/base/prelude.hpp"
#include "tactile/base/util/hash.hpp"

namespace tactile {

/**
 * Represents a two-dimensional offset.
 */
struct Offset2D final
{
  using value_type = std::ptrdiff_t;

  value_type x;
  value_type y;

  [[nodiscard]]
  constexpr auto operator==(const Offset2D&) const noexcept -> bool = default;

  [[nodiscard]]
  constexpr auto operator<=>(const Offset2D&) const noexcept -> std::strong_ordering = default;
};

}  // namespace tactile

template <>
struct std::hash<tactile::Offset2D> final
{
  [[nodiscard]]
  auto operator()(const tactile::Offset2D& offset) const noexcept -> std::size_t
  {
    return tactile::hash_combine(offset.x, offset.y);
  }
};

template <>
struct std::formatter<tactile::Offset2D> final
{
  template <typename FormatParseContext>
  constexpr auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::Offset2D& offset, FormatContext& ctx) const
  {
    return std::format_to(ctx.out(), "(dx: {}, dy: {})", offset.x, offset.y);
  }
};
