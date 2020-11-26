#pragma once

#include "nenya.hpp"

namespace tactile {
namespace detail {

struct tileset_id_t final
{};

}  // namespace detail

using tileset_id = nenya::mirror_type<int, detail::tileset_id_t>;

constexpr auto operator""_ts(unsigned long long value) noexcept -> tileset_id
{
  return tileset_id{static_cast<int>(value)};
}

}  // namespace tactile
