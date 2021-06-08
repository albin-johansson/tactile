#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace tactile {

namespace tags {
struct map_id_tag;
}  // namespace tags

using map_id = nenya::strong_type<int, tags::map_id_tag>;

[[nodiscard]] constexpr auto operator""_map(const ulonglong value) noexcept
    -> map_id
{
  return map_id{static_cast<map_id::value_type>(value)};
}

}  // namespace tactile
