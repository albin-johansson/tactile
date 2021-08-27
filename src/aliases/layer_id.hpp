#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace Tags {
struct LayerIdTag;
}  // namespace Tags

using layer_id = nenya::strong_type<int, Tags::LayerIdTag>;

[[nodiscard]] constexpr auto operator""_layer(const ulonglong value) noexcept
    -> layer_id
{
  return layer_id{static_cast<layer_id::value_type>(value)};
}

}  // namespace Tactile
