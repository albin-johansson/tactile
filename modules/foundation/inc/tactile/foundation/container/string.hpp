// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>       // string, basic_string
#include <string_view>  // string_view, basic_string_view
#include <type_traits>  // conditional_t

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

#if TACTILE_OS_WINDOWS
  #define TACTILE_NATIVE_STR(Str) L##Str
#else
  #define TACTILE_NATIVE_STR(Str) Str
#endif

namespace tactile {

using String = std::string;
using StringView = std::string_view;

using NativeChar = std::conditional_t<kIsWindows, wchar_t, char>;
using NativeString = std::basic_string<NativeChar>;
using NativeStringView = std::basic_string_view<NativeChar>;

/**
 * Creates a string using the native filesystem character type.
 *
 * \param str the source string, may be null
 *
 * \return
 *    The corresponding native string, or nothing if something went wrong.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto make_native_string(const char* str) -> Maybe<NativeString>;

/**
 * Splits a string into a collection of tokens, delimited by a specific character.
 *
 * \note
 *    Consider the `str_to_multiple_i32` family of functions over this one if the aim
 *    is to convert the tokens into integers.
 *
 * \param str       The source string.
 * \param separator The character used as a delimiter.
 *
 * \return
 *    The separated tokens.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto str_split(StringView str, char separator) -> Vector<String>;

}  // namespace tactile
