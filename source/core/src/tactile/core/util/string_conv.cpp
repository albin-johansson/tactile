// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/string_conv.hpp"

#include <charconv>      // from_chars
#include <concepts>      // invocable, integral, floating_point
#include <system_error>  // errc

#include <fast_float/fast_float.h>

#include "tactile/core/debug/generic_error.hpp"

namespace tactile {
namespace {

template <typename T, std::invocable<const char*, const char*, T&> Parser>
[[nodiscard]] auto _parse_impl(const StringView str, const Parser& parser) -> Result<T>
{
  const auto* begin = str.data();
  const auto* end = begin + str.size();

  T number {};
  const auto [ptr, err] = parser(begin, end, number);

  if (err == std::errc {} && ptr == end) {
    return number;
  }

  return unexpected(make_error(GenericError::kInvalidParam));
}

template <std::integral T>
[[nodiscard]] auto _parse_number(const StringView str, const int base) -> Result<T>
{
  return _parse_impl<T>(str, [base](const char* begin, const char* end, T& number) {
    return std::from_chars(begin, end, number, base);
  });
}

template <std::floating_point T>
[[nodiscard]] auto _parse_number(const StringView str) -> Result<T>
{
  return _parse_impl<T>(str, [](const char* begin, const char* end, T& number) {
    return fast_float::from_chars(begin, end, number);
  });
}

}  // namespace

auto parse_int(const StringView str, const int base) -> Result<int64>
{
  return _parse_number<int64>(str, base);
}

auto parse_uint(const StringView str, const int base) -> Result<uint64>
{
  return _parse_number<uint64>(str, base);
}

auto parse_float(const StringView str) -> Result<double>
{
  return _parse_number<double>(str);
}

}  // namespace tactile
