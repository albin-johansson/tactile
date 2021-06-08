#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace tactile {

namespace tags {
struct property_id_tag;
}  // namespace tags

using property_id = nenya::strong_type<int, tags::property_id_tag>;

[[nodiscard]] constexpr auto operator""_prop(const ulonglong value) noexcept
    -> property_id
{
  return property_id{static_cast<property_id::value_type>(value)};
}

}  // namespace tactile
