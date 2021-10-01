#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace Tags {
struct TileID;
}  // namespace Tags

using TileID = nenya::strong_type<int, Tags::TileID>;

constexpr TileID empty_tile{0};

[[nodiscard]] constexpr auto operator""_tile(const ulonglong value) noexcept -> TileID
{
  return TileID{static_cast<TileID::value_type>(value)};
}

}  // namespace Tactile
