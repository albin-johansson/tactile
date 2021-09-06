#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace Tags {
struct TileIdTag;
}  // namespace Tags

using tile_id = nenya::strong_type<int, Tags::TileIdTag>;
using TileID = tile_id;

constexpr tile_id empty_tile{0};

[[nodiscard]] constexpr auto operator""_tile(const ulonglong value) noexcept
    -> tile_id
{
  return tile_id{static_cast<tile_id::value_type>(value)};
}

}  // namespace Tactile
