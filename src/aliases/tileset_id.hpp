#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace Tags {
struct TilesetID;
}  // namespace Tags

using TilesetID = nenya::strong_type<int, Tags::TilesetID>;

[[nodiscard]] constexpr auto operator""_tset(const ulonglong value) noexcept
    -> TilesetID
{
  return TilesetID{static_cast<TilesetID::value_type>(value)};
}

}  // namespace Tactile
