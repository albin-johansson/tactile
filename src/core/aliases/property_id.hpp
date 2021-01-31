#pragma once

#include "nenya.hpp"

namespace tactile {
namespace detail {
struct property_id_t;
}

using property_id = nenya::mirror_type<int, detail::property_id_t>;

constexpr auto operator""_prop(unsigned long long value) noexcept -> property_id
{
  return property_id{static_cast<property_id::value_type>(value)};
}

}  // namespace tactile
