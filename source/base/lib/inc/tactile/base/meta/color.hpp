// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>
#include <cstdint>

namespace tactile {

/**
 * Represents an 8-bit RGBA color.
 *
 * \see FColor
 */
struct UColor final
{
  using value_type = std::uint8_t;

  value_type red;
  value_type green;
  value_type blue;
  value_type alpha;

  [[nodiscard]]
  constexpr auto operator==(const UColor&) const noexcept -> bool = default;
};

/**
 * Represents a normalized floating-point RGBA color.
 *
 * \see UColor
 */
struct FColor final
{
  using value_type = float;

  value_type red;
  value_type green;
  value_type blue;
  value_type alpha;

  [[nodiscard]]
  constexpr auto operator==(const FColor&) const noexcept -> bool = default;
};

[[nodiscard]]
constexpr auto to_ucolor(const FColor& color) -> UColor
{
  const auto to_u8 = [](const float value) {
    return static_cast<std::uint8_t>(std::clamp(value, 0.0f, 1.0f) * 255.0f);
  };

  return {to_u8(color.red), to_u8(color.green), to_u8(color.blue), to_u8(color.alpha)};
}

[[nodiscard]]
constexpr auto to_fcolor(const UColor& color) -> FColor
{
  const auto to_f = [](const std::uint8_t value) {
    return static_cast<float>(value) / 255.0f;
  };

  return {to_f(color.red), to_f(color.green), to_f(color.blue), to_f(color.alpha)};
}

/**
 * Encodes the color as an ABGR color packed into a 32-bit integer.
 *
 * \return
 * An 32-bit integer, where each octet represents a different color channel.
 */
[[nodiscard]]
constexpr auto to_uint32_abgr(const UColor& color) -> std::uint32_t
{
  const auto a = static_cast<std::uint32_t>(color.alpha) << std::uint32_t {24};
  const auto b = static_cast<std::uint32_t>(color.blue) << std::uint32_t {16};
  const auto g = static_cast<std::uint32_t>(color.green) << std::uint32_t {8};
  const auto r = static_cast<std::uint32_t>(color.red) << std::uint32_t {0};
  return a | b | g | r;
}

inline constexpr UColor kColorBlack {0x00, 0x00, 0x00, 0xFF};
inline constexpr UColor kColorWhite {0xFF, 0xFF, 0xFF, 0xFF};

}  // namespace tactile
