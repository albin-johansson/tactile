#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace Tags {
struct LayerID;
}  // namespace Tags

using LayerID = nenya::strong_type<int, Tags::LayerID>;

[[nodiscard]] constexpr auto operator""_layer(const ulonglong value) noexcept -> LayerID
{
  return LayerID{static_cast<LayerID::value_type>(value)};
}

}  // namespace Tactile
