#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace Tags {
struct TilesetIdTag;
}  // namespace Tags

using tileset_id = nenya::strong_type<int, Tags::TilesetIdTag>;

[[nodiscard]] constexpr auto operator""_tset(const ulonglong value) noexcept
    -> tileset_id
{
  return tileset_id{static_cast<tileset_id::value_type>(value)};
}

}  // namespace Tactile
