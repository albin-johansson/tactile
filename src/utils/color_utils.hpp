#pragma once

#include <array>          // array
#include <centurion.hpp>  // color

#include "common/ints.hpp"

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
[[nodiscard]] constexpr auto ColorToArray(const cen::color& color) noexcept
    -> std::array<float, 4>
{
  const auto r = color.red_norm();
  const auto g = color.green_norm();
  const auto b = color.blue_norm();
  const auto a = color.alpha_norm();
  return {r, g, b, a};
}

[[nodiscard]] constexpr auto ColorToU32(const cen::color& color) -> uint32
{
  return IM_COL32(color.red(), color.green(), color.blue(), color.alpha());
}

}  // namespace Tactile
