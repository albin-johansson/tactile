#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace tactile {

namespace tags {
struct tile_height_tag;
}  // namespace tags

using tile_height = nenya::strong_type<int, tags::tile_height_tag>;

[[nodiscard]] constexpr auto operator""_th(const ulonglong value) noexcept
    -> tile_height
{
  return tile_height{static_cast<tile_height::value_type>(value)};
}

}  // namespace tactile
