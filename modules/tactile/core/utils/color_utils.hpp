#pragma once

#include <array>  // array

#include <centurion.hpp>  // color
#include <imgui.h>
#include <tactile_def.hpp>

namespace tactile {

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
[[nodiscard]] constexpr auto ColorToArray(const cen::color& color) noexcept
    -> std::array<float, 4>
{
  const auto r = color.norm_red();
  const auto g = color.norm_green();
  const auto b = color.norm_blue();
  const auto a = color.norm_alpha();
  return {r, g, b, a};
}

[[nodiscard]] constexpr auto ColorToU32(const cen::color& color) -> uint32
{
  return IM_COL32(color.red(), color.green(), color.blue(), color.alpha());
}

[[nodiscard]] inline auto Darker(const cen::color& color) -> cen::color
{
  constexpr float factor = 0.8f;
  return cen::color::from_norm(factor * color.norm_red(),
                               factor * color.norm_green(),
                               factor * color.norm_blue(),
                               1.0f);
}

}  // namespace tactile
