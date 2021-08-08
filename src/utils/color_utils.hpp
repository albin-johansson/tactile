#pragma once

#include <array>          // array
#include <centurion.hpp>  // color

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
  const auto r = static_cast<float>(color.red()) / 255.0f;
  const auto g = static_cast<float>(color.green()) / 255.0f;
  const auto b = static_cast<float>(color.blue()) / 255.0f;
  const auto a = static_cast<float>(color.alpha()) / 255.0f;
  return {r, g, b, a};
}

}  // namespace Tactile
