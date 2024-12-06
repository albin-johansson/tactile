// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/common/numeric/charconv.hpp"

#include <charconv>

#include <fast_float/fast_float.h>

namespace tactile::common {

auto parse_int(const std::string_view str, const int base) noexcept
    -> std::optional<std::int64_t>
{
  const auto* const begin = str.data();
  const auto* const end = begin + str.size();

  std::int64_t value {};
  const auto [ptr, err] = std::from_chars(begin, end, value, base);

  if (err == std::errc {} && ptr == end) {
    return value;
  }

  return std::nullopt;
}

auto parse_uint(const std::string_view str, const int base) noexcept
    -> std::optional<std::uint64_t>
{
  const auto* const begin = str.data();
  const auto* const end = begin + str.size();

  std::uint64_t value {};
  const auto [ptr, err] = std::from_chars(begin, end, value, base);

  if (err == std::errc {} && ptr == end) {
    return value;
  }

  return std::nullopt;
}

auto parse_double(const std::string_view str) noexcept -> std::optional<double>
{
  const auto* const begin = str.data();
  const auto* const end = begin + str.size();

  double value {};
  const auto [ptr, err] = fast_float::from_chars(begin, end, value);

  if (err == std::errc {} && ptr == end) {
    return value;
  }

  return std::nullopt;
}

}  // namespace tactile::common
