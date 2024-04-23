// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/string_ops.hpp"

#include <algorithm>  // find_if
#include <cctype>     // isspace
#include <locale>     // locale

#if TACTILE_OS_WINDOWS
  #include <windows.h>
#endif

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/generic_error.hpp"

namespace tactile {

auto make_native_string(const char* str) -> Result<NativeString>
{
  if (!str) {
    return unexpected(make_error(GenericError::kInvalidParam));
  }

#if TACTILE_OS_WINDOWS
  // Windows is the only platform that we support that uses wchar_t filesystem paths.
  static_assert(std::same_as<NativeChar, wchar_t>);

  // Figure out the required size of the converted string.
  const auto wide_char_count = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);

  // Create string of appropriate size.
  NativeString wide_str {};
  wide_str.resize(wide_char_count);

  // Finally convert the input string.
  if (MultiByteToWideChar(CP_UTF8, 0, str, -1, wide_str.data(), wide_char_count) != 0) {
    // The conversion was successful, but the string features an additional
    // null-terminator which we need to get rid of.
    TACTILE_ASSERT(!wide_str.empty());
    TACTILE_ASSERT(wide_str.back() == '\0');
    wide_str.pop_back();
    return wide_str;
  }

  return unexpected(make_error(GenericError::kUnknown));
#else
  return NativeString {str};
#endif  // TACTILE_OS_WINDOWS
}

auto trim_string(const StringView str) -> String
{
  String copy {str};

  const auto find_first_non_space = [](const auto begin, const auto end) {
    return std::find_if(begin, end, [](const char ch) {
      return !std::isspace(ch, std::locale::classic());
    });
  };

  const auto left_iter = find_first_non_space(copy.begin(), copy.end());
  if (left_iter != copy.end()) {
    copy.erase(copy.begin(), left_iter);
  }

  const auto right_iter = find_first_non_space(copy.rbegin(), copy.rend());
  if (right_iter != copy.rend()) {
    copy.erase(right_iter.base(), copy.end());
  }

  return copy;
}

}  // namespace tactile
