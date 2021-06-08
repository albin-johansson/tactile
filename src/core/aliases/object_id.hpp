#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace tactile {

namespace tags {
struct object_id_tag;
}  // namespace tags

using object_id = nenya::strong_type<int, tags::object_id_tag>;

[[nodiscard]] constexpr auto operator""_obj(const ulonglong value) noexcept
    -> object_id
{
  return object_id{static_cast<object_id::value_type>(value)};
}

}  // namespace tactile
