#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace tactile {

namespace tags {
struct tile_id_tag;
}  // namespace tags

using tile_id = nenya::strong_type<int, tags::tile_id_tag>;

inline constexpr tile_id empty_tile{0};

[[nodiscard]] constexpr auto operator""_tile(const ulonglong value) noexcept
    -> tile_id
{
  return tile_id{static_cast<tile_id::value_type>(value)};
}

}  // namespace tactile
