#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace tactile {

namespace tags {
struct tile_width_tag;
}  // namespace tags

using tile_width = nenya::strong_type<int, tags::tile_width_tag>;

[[nodiscard]] constexpr auto operator""_tw(const ulonglong value) noexcept
    -> tile_width
{
  return tile_width{static_cast<tile_width::value_type>(value)};
}

}  // namespace tactile
