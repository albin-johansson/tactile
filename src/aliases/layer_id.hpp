#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace tactile {

namespace tags {
struct layer_id_tag;
}  // namespace tags

using layer_id = nenya::strong_type<int, tags::layer_id_tag>;

[[nodiscard]] constexpr auto operator""_layer(const ulonglong value) noexcept
    -> layer_id
{
  return layer_id{static_cast<layer_id::value_type>(value)};
}

}  // namespace tactile
