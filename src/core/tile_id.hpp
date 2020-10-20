#pragma once

#include "nenya.hpp"

namespace tactile {
namespace detail {

struct tile_id_t final
{};

}  // namespace detail

using tile_id = nenya::mirror_type<int, detail::tile_id_t>;

constexpr auto operator""_t(unsigned long long value) noexcept -> tile_id
{
  return tile_id{static_cast<int>(value)};
}

inline constexpr tile_id empty{0};

}  // namespace tactile
