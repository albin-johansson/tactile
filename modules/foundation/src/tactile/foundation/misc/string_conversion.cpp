// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/string_conversion.hpp"

#include <algorithm>     // count
#include <charconv>      // from_chars
#include <concepts>      // integral, floating_point, invocable
#include <system_error>  // errc

#include <fast_float/fast_float.h>

#include "tactile/foundation/misc/conversion.hpp"

namespace tactile {
namespace {

template <typename T, std::invocable<const char*, const char*, T&> ParserType>
[[nodiscard]] auto _parse_generic(const StringView str, const ParserType& parse_fn)
    -> Maybe<T>
{
  const char* begin = str.data();
  const char* end = str.data() + str.size();

  T value {};
  const auto [ptr, err] = parse_fn(begin, end, value);

  // Require no error and fully valid string, e.g., "42 " is not accepted.
  if (err == std::errc {} && ptr == end) {
    return value;
  }
  else {
    return kNone;
  }
}

template <std::integral T>
[[nodiscard]] auto _parse_int(const StringView str, const int base) -> Maybe<T>
{
  return _parse_generic<T>(str, [base](const char* begin, const char* end, T& value) {
    return std::from_chars(begin, end, value, base);
  });
}

template <std::floating_point T>
[[nodiscard]] auto _parse_float(const StringView str) -> Maybe<T>
{
  return _parse_generic<T>(str, [](const char* begin, const char* end, T& value) {
    return fast_float::from_chars(begin, end, value);
  });
}

template <typename T>
[[nodiscard]] auto _str_to_multiple_generic(const StringView str,
                                            const char separator,
                                            const int base) -> Vector<T>
{
  const char* const str_begin = str.data();
  const char* const str_end = str.data() + str.size();

  // Estimate the amount of numbers in the string to avoid unnecessary allocations.
  const auto expected_count = std::count(str_begin, str_end, separator) + 1;

  Vector<T> numbers {};
  numbers.reserve(as_unsigned(expected_count));

  const char* current_begin = str_begin;
  const char* result_ptr = nullptr;
  std::errc result_err {};

  // Continue until we hit the end of the string or an error occurs.
  while (result_ptr != str_end && result_err == std::errc {}) {
    T value {};

    if constexpr (std::integral<T>) {
      const auto result = std::from_chars(current_begin, str_end, value, base);
      result_ptr = result.ptr;
      result_err = result.ec;
    }
    else {
      const auto result = fast_float::from_chars(current_begin, str_end, value);
      result_ptr = result.ptr;
      result_err = result.ec;
    }

    // Store the value if the conversion was successful.
    if (result_err == std::errc {}) {
      numbers.push_back(value);
    }

    // Advance the current starting point one past the encountered separator.
    current_begin = result_ptr + 1;
  }

  // Return an empty vector if something went wrong.
  if (result_err != std::errc {}) {
    return {};
  }

  return numbers;
}

}  // namespace

auto str_to_multiple_i32(const StringView str, const char separator, const int base)
    -> Vector<int32>
{
  return _str_to_multiple_generic<int32>(str, separator, base);
}

auto str_to_multiple_u32(const StringView str, const char separator, const int base)
    -> Vector<uint32>
{
  return _str_to_multiple_generic<uint32>(str, separator, base);
}

auto str_to_multiple_f32(const StringView str, const char separator) -> Vector<float32>
{
  // The base parameter is ignored for floating point types.
  return _str_to_multiple_generic<float32>(str, separator, 10);
}

auto str_to_u64(const StringView str, const int base) -> Maybe<uint64>
{
  return _parse_int<uint64>(str, base);
}

auto str_to_i64(const StringView str, const int base) -> Maybe<int64>
{
  return _parse_int<int64>(str, base);
}

auto str_to_f64(const StringView str) -> Maybe<float64>
{
  return _parse_float<float64>(str);
}

}  // namespace tactile
