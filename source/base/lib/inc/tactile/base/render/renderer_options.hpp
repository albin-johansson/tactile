// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t
#include <format>   // formatter, format_to

#include "tactile/base/prelude.hpp"

namespace tactile {

enum class TextureFilterMode : std::uint8_t
{
  kNearest,
  kLinear
};

struct RendererOptions final
{
  /** The texture filtering mode to use with loaded textures. */
  TextureFilterMode texture_filter_mode;

  /** Generate mipmaps for loaded textures. */
  bool use_mipmaps;

  /** Synchronize image presentation with the monitor vertical blanking period. */
  bool use_vsync;

  /** Try to limit the frame rate to the monitor refresh rate.  */
  bool limit_fps;
};

}  // namespace tactile

template <>
struct std::formatter<tactile::TextureFilterMode> final
{
  template <typename FormatParseContext>
  constexpr static auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::TextureFilterMode& mode, FormatContext& ctx) const
  {
    switch (mode) {
      case tactile::TextureFilterMode::kNearest: return std::format_to(ctx.out(), "nearest");
      case tactile::TextureFilterMode::kLinear:  return std::format_to(ctx.out(), "linear");
      default:                                   return std::format_to(ctx.out(), "?");
    }
  }
};
