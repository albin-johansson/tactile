#pragma once

#include <array>  // array

#include <tactile_def.hpp>

#include <centurion.hpp>  // color
#include <imgui.h>

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

}  // namespace Tactile
