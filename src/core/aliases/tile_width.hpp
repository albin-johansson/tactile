#pragma once

#include "nenya.hpp"

namespace tactile {
namespace detail {

struct tile_width_t
{};

}  // namespace detail

using tile_width = nenya::mirror_type<int, detail::tile_width_t>;

constexpr auto operator""_tw(unsigned long long value) noexcept -> tile_width
{
  return tile_width{static_cast<tile_width::value_type>(value)};
}

}  // namespace tactile
