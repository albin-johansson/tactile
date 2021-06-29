#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace tags {
struct col_tag;
}  // namespace tags

using col_t = nenya::strong_type<int, tags::col_tag>;

[[nodiscard]] constexpr auto operator""_col(const ulonglong value) noexcept
    -> col_t
{
  return col_t{static_cast<col_t::value_type>(value)};
}

}  // namespace Tactile
