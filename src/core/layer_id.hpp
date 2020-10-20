#pragma once

#include "nenya.hpp"

namespace tactile {
namespace detail {

struct layer_id_t final
{};

}  // namespace detail

using layer_id = nenya::mirror_type<int, detail::layer_id_t>;

constexpr auto operator""_layer(unsigned long long value) noexcept -> layer_id
{
  return layer_id{static_cast<int>(value)};
}

}  // namespace tactile
