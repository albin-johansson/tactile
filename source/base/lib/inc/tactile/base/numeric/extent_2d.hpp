// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>      // strong_ordering
#include <cstddef>      // size_t
#include <format>       // formatter, format_to, formattable
#include <functional>   // hash
#include <type_traits>  // is_unsigned_v

#include "tactile/base/numeric/index_2d.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/hash.hpp"

namespace tactile {

/**
 * Represents a non-negative two-dimensional extent.
 */
struct Extent2D final
{
  using value_type = std::size_t;

  value_type rows;
  value_type cols;

  /**
   * Indicates whether an index is within the interval [{0, 0}, {rows, cols}).
   *
   * \param index The index to check.
   *
   * \return
   * True if the index is within the region; false otherwise.
   */
  [[nodiscard]]
  constexpr auto contains(const Index2D& index) const noexcept -> bool
  {
    static_assert(std::is_unsigned_v<Index2D::value_type>);
    return index.y < rows && index.x < cols;
  }

  [[nodiscard]]
  constexpr auto operator==(const Extent2D&) const noexcept -> bool = default;

  [[nodiscard]]
  constexpr auto operator<=>(const Extent2D&) const noexcept -> std::strong_ordering = default;
};

}  // namespace tactile

template <>
struct std::hash<tactile::Extent2D> final
{
  [[nodiscard]]
  auto operator()(const tactile::Extent2D& extent) const noexcept -> std::size_t
  {
    return tactile::hash_combine(extent.rows, extent.cols);
  }
};

template <>
struct std::formatter<tactile::Extent2D> final
{
  template <typename FormatParseContext>
  constexpr static auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::Extent2D& extent, FormatContext& ctx) const
  {
    return std::format_to(ctx.out(), "(rows: {}, cols: {})", extent.rows, extent.cols);
  }
};

static_assert(std::formattable<tactile::Extent2D, char>);
