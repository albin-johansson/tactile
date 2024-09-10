// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>  // ptrdiff_t
#include <format>   // formatter, format_to, formattable

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents the size of a two-dimensional matrix.
 */
struct MatrixExtent final
{
  using value_type = std::ptrdiff_t;

  /** The number of rows. */
  value_type rows;

  /** The number of columns. */
  value_type cols;

  [[nodiscard]]
  auto operator==(const MatrixExtent&) const noexcept -> bool = default;
};

}  // namespace tactile

template <>
struct std::formatter<tactile::MatrixExtent> final
{
  template <typename FormatParseContext>
  constexpr auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::MatrixExtent& extent, FormatContext& ctx) const
  {
    return std::format_to(ctx.out(), "({}, {})", extent.rows, extent.cols);
  }
};

static_assert(std::formattable<tactile::MatrixExtent, char>);
