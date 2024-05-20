// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/string_conv.hpp"

#include <charconv>      // from_chars
#include <concepts>      // invocable, integral, floating_point, same_as
#include <system_error>  // errc

#if TACTILE_OS_WINDOWS
  #include <windows.h>
#endif

#include <fast_float/fast_float.h>

#include "tactile/core/debug/generic_error.hpp"

namespace tactile {
inline namespace string_conv {

#if TACTILE_OS_WINDOWS
// Windows is the only platform that we support that uses wchar_t paths.
static_assert(std::same_as<NativeChar, wchar_t>);
#endif  // TACTILE_OS_WINDOWS

template <typename T, std::invocable<const char*, const char*, T&> Parser>
[[nodiscard]] auto _parse_impl(const StringView str,
                               const Parser& parser) -> Result<T>
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
[[nodiscard]] auto _parse_number(const StringView str,
                                 const int base) -> Result<T>
{
  return _parse_impl<T>(str,
                        [base](const char* begin, const char* end, T& number) {
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

}  // namespace string_conv

auto to_native_string(const StringView str) -> Result<NativeString>
{
#if TACTILE_OS_WINDOWS
  if (str.empty()) {
    return TACTILE_NATIVE_STR("");
  }

  const auto* input_data = str.data();
  const auto input_size = narrow_cast<int>(str.size());

  // Figure out the required size of the converted string.
  const auto wide_char_count = MultiByteToWideChar(CP_UTF8,
                                                   MB_ERR_INVALID_CHARS,
                                                   input_data,
                                                   input_size,
                                                   nullptr,
                                                   0);
  if (wide_char_count <= 0) {
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  // Create string of appropriate size.
  NativeString wide_str {};
  wide_str.resize(wide_char_count);

  // Finally convert the input string.
  if (MultiByteToWideChar(CP_UTF8,
                          MB_ERR_INVALID_CHARS,
                          input_data,
                          input_size,
                          wide_str.data(),
                          wide_char_count) != 0) {
    return wide_str;
  }

  return unexpected(make_error(GenericError::kUnknown));
#else
  return NativeString {str};
#endif  // TACTILE_OS_WINDOWS
}

auto from_native_string(const NativeStringView str) -> Result<String>
{
#if TACTILE_OS_WINDOWS
  if (str.empty()) {
    return "";
  }

  const auto* input_data = str.data();
  const auto input_size = narrow_cast<int>(str.size());

  // Figure out the required size of the converted string.
  const auto char_count = WideCharToMultiByte(CP_UTF8,
                                              WC_ERR_INVALID_CHARS,
                                              input_data,
                                              input_size,
                                              nullptr,
                                              0,
                                              nullptr,
                                              nullptr);
  if (char_count <= 0) {
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  // Create string of appropriate size.
  String output_str {};
  output_str.resize(static_cast<usize>(char_count));

  if (WideCharToMultiByte(CP_UTF8,
                          WC_ERR_INVALID_CHARS,
                          input_data,
                          input_size,
                          output_str.data(),
                          char_count,
                          nullptr,
                          nullptr) != 0) {
    return output_str;
  }

  return unexpected(make_error(GenericError::kUnknown));
#else
  return String {str};
#endif  // TACTILE_OS_WINDOWS
}

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
