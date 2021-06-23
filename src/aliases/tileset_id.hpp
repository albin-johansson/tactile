#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace tactile {

namespace tags {
struct tileset_id_tag;
}  // namespace tags

using tileset_id = nenya::strong_type<int, tags::tileset_id_tag>;

[[nodiscard]] constexpr auto operator""_tset(const ulonglong value) noexcept
    -> tileset_id
{
  return tileset_id{static_cast<tileset_id::value_type>(value)};
}

}  // namespace tactile
