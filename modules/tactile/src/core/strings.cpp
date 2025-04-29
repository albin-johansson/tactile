// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module;

#include <charconv>
#include <concepts>

#include <fast_float/fast_float.h>

module tactile.core;

namespace tactile {
namespace {

template <std::integral T>
auto _parse_int(const StringView str, const int base) noexcept -> Option<T>
{
  const auto* const begin = str.data();
  const auto* const end = begin + str.size();

  T value {};
  const auto [ptr, err] = std::from_chars(begin, end, value, base);

  if (err == std::errc {} && ptr == end) {
    return value;
  }

  return nullopt;
}

}  // namespace

auto parse_i64(const StringView str, const int base) noexcept -> Option<i64>
{
  return _parse_int<i64>(str, base);
}

auto parse_u64(const StringView str, const int base) noexcept -> Option<u64>
{
  return _parse_int<u64>(str, base);
}

auto parse_f64(const StringView str) noexcept -> Option<f64>
{
  const auto* const begin = str.data();
  const auto* const end = begin + str.size();

  f64 value {};
  const auto [ptr, err] = fast_float::from_chars(begin, end, value);

  if (err == std::errc {} && ptr == end) {
    return value;
  }

  return nullopt;
}

}  // namespace tactile
