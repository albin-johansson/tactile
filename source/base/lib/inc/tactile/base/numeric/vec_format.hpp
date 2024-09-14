// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <format>       // formatter, format_to, formattable
#include <type_traits>  // is_floating_point_v

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"

template <typename T>
struct std::formatter<tactile::Vec<T, 2>> final
{
  template <typename FormatParseContext>
  constexpr auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::Vec<T, 2>& vec, FormatContext& ctx) const
  {
    if constexpr (std::is_floating_point_v<T>) {
      return std::format_to(ctx.out(), "{{{:.3f}, {:.3f}}}", vec.x(), vec.y());
    }
    else {
      return std::format_to(ctx.out(), "{{{}, {}}}", vec.x(), vec.y());
    }
  }
};

template <typename T>
struct std::formatter<tactile::Vec<T, 3>> final
{
  template <typename FormatParseContext>
  constexpr auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::Vec<T, 3>& vec, FormatContext& ctx) const
  {
    if constexpr (std::is_floating_point_v<T>) {
      return std::format_to(ctx.out(),
                            "{{{:.3f}, {:.3f}, {:.3f}}}",
                            vec.x(),
                            vec.y(),
                            vec.z());
    }
    else {
      return std::format_to(ctx.out(), "{{{}, {}, {}}}", vec.x(), vec.y(), vec.z());
    }
  }
};

template <typename T>
struct std::formatter<tactile::Vec<T, 4>> final
{
  template <typename FormatParseContext>
  constexpr auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::Vec<T, 4>& vec, FormatContext& ctx) const
  {
    if constexpr (std::is_floating_point_v<T>) {
      return std::format_to(ctx.out(),
                            "{{{:.3f}, {:.3f}, {:.3f}, {:.3f}}}",
                            vec.x(),
                            vec.y(),
                            vec.z(),
                            vec.w());
    }
    else {
      return std::format_to(ctx.out(),
                            "{{{}, {}, {}, {}}}",
                            vec.x(),
                            vec.y(),
                            vec.z(),
                            vec.w());
    }
  }
};

static_assert(std::formattable<tactile::Float2, char>);
static_assert(std::formattable<tactile::Float3, char>);
static_assert(std::formattable<tactile::Float4, char>);
static_assert(std::formattable<tactile::Int2, char>);
static_assert(std::formattable<tactile::Int3, char>);
static_assert(std::formattable<tactile::Int4, char>);
