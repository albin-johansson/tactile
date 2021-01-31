#pragma once

#include "nenya.hpp"

namespace tactile {
namespace detail {

struct tile_height_t
{};

}  // namespace detail

using tile_height = nenya::mirror_type<int, detail::tile_height_t>;

constexpr auto operator""_th(unsigned long long value) noexcept -> tile_height
{
  return tile_height{static_cast<tile_height::value_type>(value)};
}

}  // namespace tactile
