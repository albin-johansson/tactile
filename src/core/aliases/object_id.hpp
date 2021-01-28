#pragma once

#include <nenya.hpp>

namespace tactile {
namespace detail {
struct object_id_t;
}

using object_id = nenya::mirror_type<int, detail::object_id_t>;

constexpr auto operator""_obj(unsigned long long value) noexcept -> object_id
{
  return object_id{static_cast<object_id::value_type>(value)};
}

}  // namespace tactile
