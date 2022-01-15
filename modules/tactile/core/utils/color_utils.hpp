#pragma once

#include <array>  // array

#include <centurion.hpp>  // color
#include <imgui.h>
#include <tactile_def.hpp>

namespace Tactile {

/**
 * \brief Converts a color into an array of normalized color components.
 *
 * \details The array values are stored in the order red/green/blue/alpha.
 *
 * \ingroup utils
 *
 * \param color the color that will be converted.
 *
 * \return an array of normalized color components.
 */
[[nodiscard]] constexpr auto ColorToArray(const cen::Color& color) noexcept
    -> std::array<float, 4>
{
  const auto r = color.GetRedNorm();
  const auto g = color.GetGreenNorm();
  const auto b = color.GetBlueNorm();
  const auto a = color.GetAlphaNorm();
  return {r, g, b, a};
}

[[nodiscard]] constexpr auto ColorToU32(const cen::Color& color) -> uint32
{
  return IM_COL32(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
}

[[nodiscard]] inline auto Darker(const cen::Color& color) -> cen::Color
{
  constexpr float factor = 0.8f;
  return cen::Color::FromNorm(factor * color.GetRedNorm(),
                              factor * color.GetGreenNorm(),
                              factor * color.GetBlueNorm(),
                              1.0f);
}

}  // namespace Tactile
