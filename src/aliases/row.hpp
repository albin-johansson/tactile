#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace Tags {
struct row_tag;
}  // namespace Tags

using row_t = nenya::strong_type<int, Tags::row_tag>;

[[nodiscard]] constexpr auto AsRow(const auto value) noexcept -> row_t
{
  return row_t{static_cast<row_t::value_type>(value)};
}

[[nodiscard]] constexpr auto operator""_row(const ulonglong value) noexcept -> row_t
{
  return AsRow(value);
}

}  // namespace Tactile
