#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace tactile {

namespace tags {
struct row_tag;
}  // namespace tags

using row_t = nenya::strong_type<int, tags::row_tag>;

[[nodiscard]] constexpr auto operator""_row(const ulonglong value) noexcept
    -> row_t
{
  return row_t{static_cast<row_t::value_type>(value)};
}

}  // namespace tactile
