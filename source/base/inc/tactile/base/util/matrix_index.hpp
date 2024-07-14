// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>     // size_t
#include <format>      // formatter, format_to, formattable
#include <functional>  // hash

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/hash.hpp"

namespace tactile {

/**
 * Represents a location in a two-dimensional matrix.
 */
struct MatrixIndex final
{
  using value_type = ssize;

  /** The row index. */
  value_type row;

  /** The column index. */
  value_type col;

  [[nodiscard]] auto operator==(const MatrixIndex&) const noexcept
      -> bool = default;
};

}  // namespace tactile

template <>
struct std::hash<tactile::MatrixIndex> final
{
  [[nodiscard]]
  auto operator()(const tactile::MatrixIndex& index) const noexcept
      -> std::size_t
  {
    return tactile::hash_combine(index.row, index.col);
  }
};

template <>
struct std::formatter<tactile::MatrixIndex> final
{
  template <typename FormatParseContext>
  constexpr auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::MatrixIndex& index, FormatContext& ctx) const
  {
    return std::format_to(ctx.out(), "({}, {})", index.row, index.col);
  }
};

static_assert(std::formattable<tactile::MatrixIndex, char>);
