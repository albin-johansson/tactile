// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/container/string.hpp"

#include <algorithm>     // count
#include <charconv>      // from_chars
#include <concepts>      // integral, floating_point, same_as
#include <sstream>       // stringstream
#include <string>        // getline, erase
#include <system_error>  // errc
#include <utility>       // move

#include <fast_float/fast_float.h>

#if TACTILE_OS_WINDOWS
  #include <windows.h>
#endif

#include "tactile/core/debug/assert.hpp"

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
  const auto expected_count = std::ranges::count(str_begin, str_end, separator) + 1;

  Vector<T> numbers;
  numbers.reserve(static_cast<usize>(expected_count));

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

auto make_native_string(const char* str) -> Maybe<NativeString>
{
  if (!str) {
    return kNone;
  }

#if TACTILE_OS_WINDOWS
  // Windows is the only platform that we support that uses wchar_t filesystem paths
  static_assert(std::same_as<NativeChar, wchar_t>);

  // Figure out the required size of the converted string
  const auto wide_char_count = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);

  // Create string of appropriate size
  NativeString wide_str;
  wide_str.resize(wide_char_count);

  // Finally convert the input string
  if (MultiByteToWideChar(CP_UTF8, 0, str, -1, wide_str.data(), wide_char_count) != 0) {
    // The conversion was successful, but the string features an additional
    // null-terminator which we need to get rid of.
    TACTILE_ASSERT(!wide_str.empty());
    TACTILE_ASSERT(wide_str.back() == '\0');
    wide_str.pop_back();
    return wide_str;
  }
  else {
    return kNone;
  }
#else
  return NativeString {str};
#endif  // TACTILE_OS_WINDOWS
}

auto str_split(const StringView str, const char separator) -> Vector<String>
{
  std::stringstream stream;
  stream << str;

  Vector<String> tokens;

  String token;
  while (std::getline(stream, token, separator)) {
    std::erase(token, '\n');

    tokens.push_back(std::move(token));
    token.clear();
  }

  return tokens;
}

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
  // Note, the base parameter is ignored for floating point types.
  return _str_to_multiple_generic<float32>(str, separator, 10);
}

auto str_to_u32(const StringView str, const int base) -> Maybe<uint32>
{
  return _parse_int<uint32>(str, base);
}

auto str_to_i32(const StringView str, const int base) -> Maybe<int32>
{
  return _parse_int<int32>(str, base);
}

auto str_to_f32(const StringView str) -> Maybe<float32>
{
  return _parse_float<float32>(str);
}

}  // namespace tactile
