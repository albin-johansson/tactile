#pragma once

#include "nenya.hpp"

namespace tactile {
namespace detail {

struct map_id_t final
{};

}  // namespace detail

using map_id = nenya::mirror_type<int, detail::map_id_t>;

constexpr auto operator""_map(unsigned long long value) noexcept -> map_id
{
  return map_id{static_cast<map_id::value_type>(value)};
}

}  // namespace tactile
