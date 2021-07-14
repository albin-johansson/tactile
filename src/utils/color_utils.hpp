#pragma once

#include <array>          // array
#include <centurion.hpp>  // color
#include <string_view>    // string_view

#include "aliases/maybe.hpp"

namespace Tactile {

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
