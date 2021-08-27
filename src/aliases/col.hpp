#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace Tags {
struct ColumnTag;
}  // namespace Tags

using col_t = nenya::strong_type<int, Tags::ColumnTag>;

[[nodiscard]] constexpr auto AsColumn(const auto value) noexcept -> col_t
{
  return col_t{static_cast<col_t::value_type>(value)};
}

[[nodiscard]] constexpr auto operator""_col(const ulonglong value) noexcept -> col_t
{
  return AsColumn(value);
}

}  // namespace Tactile
