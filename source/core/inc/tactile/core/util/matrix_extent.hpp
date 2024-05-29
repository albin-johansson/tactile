// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <format>   // formatter, format_to, formattable
#include <ostream>  // ostream

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents the size of a two-dimensional matrix.
 */
struct MatrixExtent final
{
  using value_type = ssize;

  /** The number of rows. */
  value_type rows;

  /** The number of columns. */
  value_type cols;

  [[nodiscard]]
  auto operator==(const MatrixExtent&) const noexcept -> bool = default;
};

/**
 * Outputs a matrix extent to a stream.
 *
 * \param stream The output stream.
 * \param extent The matrix extent to emit.
 *
 * \return
 * The provided stream.
 */
auto operator<<(std::ostream& stream,
                const MatrixExtent& extent) -> std::ostream&;

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
