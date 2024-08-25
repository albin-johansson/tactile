// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>     // strong_ordering
#include <concepts>    // integral
#include <cstddef>     // size_t
#include <format>      // formatter, format_to, formattable
#include <functional>  // hash

#include "tactile/base/int.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/base/numeric/vec.hpp"
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

  [[nodiscard]]
  auto operator<=>(const MatrixIndex&) const noexcept -> std::strong_ordering = default;
};

/**
 * Converts a raw element index and column count into the corresponding matrix index.
 *
 * \tparam T Any integral type.
 *
 * \param index        The raw element index.
 * \param column_count The number of columns in the underlying matrix.
 *
 * \return
 */
template <std::integral T>
[[nodiscard]] constexpr auto make_matrix_index(const T index,
                                               const T column_count) noexcept -> MatrixIndex
{
  return {saturate_cast<MatrixIndex::value_type>(index / column_count),
          saturate_cast<MatrixIndex::value_type>(index % column_count)};
}

[[nodiscard]]
constexpr auto to_int2(const MatrixIndex& index) noexcept -> Int2
{
  return {
    static_cast<Int2::value_type>(index.col),
    static_cast<Int2::value_type>(index.row),
  };
}

[[nodiscard]]
constexpr auto to_float2(const MatrixIndex& index) noexcept -> Float2
{
  return {
    static_cast<Float2::value_type>(index.col),
    static_cast<Float2::value_type>(index.row),
  };
}

}  // namespace tactile

template <>
struct std::hash<tactile::MatrixIndex> final
{
  [[nodiscard]]
  auto operator()(const tactile::MatrixIndex& index) const noexcept -> std::size_t
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
