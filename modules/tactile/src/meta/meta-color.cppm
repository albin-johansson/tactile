// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

export module tactile.meta:color;

export import tactile.core;

export namespace tactile {

/// Represents an RGBA color with 8-bit precision.
struct Color final
{
  u8 red {0x00u};
  u8 green {0x00u};
  u8 blue {0x00u};
  u8 alpha {0xFFu};

  [[nodiscard]]
  constexpr auto operator==(const Color&) const noexcept -> bool = default;
};

}  // namespace tactile
