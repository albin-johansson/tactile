// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>     // strong_ordering
#include <cstddef>     // size_t
#include <format>      // formatter, format_to, formattable
#include <functional>  // hash

#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/hash.hpp"

namespace tactile {

/**
 * Represents a non-negative index in a two-dimensional matrix.
 */
struct Index2D final
{
  using value_type = std::size_t;

  value_type x;
  value_type y;

  /**
   * Creates a two-dimensional index from a one-dimensional index and column count.
   *
   * \tparam T Any integral type.
   *
   * \param index        The raw element index.
   * \param column_count The number of columns in the underlying matrix.
   *
   * \return
   * A two-dimensional index.
   */
  template <std::integral T>
  [[nodiscard]] constexpr static auto from_1d(const T index,
                                              const T column_count) noexcept -> Index2D
  {
    return {
      .x = saturate_cast<value_type>(index % column_count),
      .y = saturate_cast<value_type>(index / column_count),
    };
  }

  [[nodiscard]]
  constexpr auto operator==(const Index2D&) const noexcept -> bool = default;

  [[nodiscard]]
  constexpr auto operator<=>(const Index2D&) const noexcept -> std::strong_ordering = default;
};

[[nodiscard]]
constexpr auto to_int2(const Index2D& index) noexcept -> Int2
{
  return {
    static_cast<Int2::value_type>(index.x),
    static_cast<Int2::value_type>(index.y),
  };
}

[[nodiscard]]
constexpr auto to_float2(const Index2D& index) noexcept -> Float2
{
  return {
    static_cast<Float2::value_type>(index.x),
    static_cast<Float2::value_type>(index.y),
  };
}

}  // namespace tactile

template <>
struct std::hash<tactile::Index2D> final
{
  [[nodiscard]]
  auto operator()(const tactile::Index2D& index) const noexcept -> std::size_t
  {
    return tactile::hash_combine(index.x, index.y);
  }
};

template <>
struct std::formatter<tactile::Index2D> final
{
  template <typename FormatParseContext>
  constexpr static auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::Index2D& index, FormatContext& ctx) const
  {
    return std::format_to(ctx.out(), "(x{}, y{})", index.x, index.y);
  }
};

static_assert(std::formattable<tactile::Index2D, char>);
