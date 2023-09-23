// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>       // string, basic_string
#include <string_view>  // string_view, basic_string_view
#include <type_traits>  // conditional_t

#include "tactile/core/api.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/prelude.hpp"

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
 * \brief Creates a string using the native filesystem character type.
 *
 * \param str the source string, may be null
 *
 * \return the corresponding native string, or nothing if something went wrong.
 */
[[nodiscard]]
TACTILE_CORE_API auto make_native_string(const char* str) -> Maybe<NativeString>;

/**
 * \brief Splits a string into a collection of tokens, delimited by a specific character.
 *
 * \note Consider the `str_to_multiple_i32` family of functions over this one if the aim
 *       is to convert the tokens into integers.
 *
 * \param str       the source string.
 * \param separator the character used as a delimiter.
 *
 * \return the separated tokens.
 */
[[nodiscard]]
TACTILE_CORE_API auto str_split(StringView str, char separator) -> Vector<String>;

/**
 * \brief Converts a string into multiple integers.
 *
 * \details This function can be used to efficiently extract integers stored in a string
 *          that are delimited by a given character separator. For example, the string
 *          `"1:2:3:4"` can be converted to the vector `{1, 2, 3, 4}` using a single call
 *          to this function.
 *
 * \param str       the source string.
 * \param separator the character used to delimit each integer.
 * \param base      the numerical base.
 *
 * \return the parsed integers. An empty vector is return if an error occurred.
 */
[[nodiscard]]
TACTILE_CORE_API auto str_to_multiple_i32(StringView str, char separator, int base = 10)
    -> Vector<int32>;

/** \copydoc str_to_multiple_i32 */
[[nodiscard]]
TACTILE_CORE_API auto str_to_multiple_u32(StringView str, char separator, int base = 10)
    -> Vector<uint32>;

/**
 * \brief Converts a string into multiple floats.
 *
 * \param str       the source string.
 * \param separator the character used to delimit each float.
 *
 * \return the parsed numbers. An empty vector is return if an error occurred.
 *
 * \see `str_to_multiple_i32`
 * \see `str_to_multiple_u32`
 */
[[nodiscard]]
TACTILE_CORE_API auto str_to_multiple_f32(StringView str, char separator)
    -> Vector<float32>;

/**
 * \brief Converts a string into an unsigned 32-bit integer.
 *
 * \param str  the source string.
 * \param base the numerical base.
 *
 * \return the converted value, or nothing if an error occurred.
 */
[[nodiscard]]
TACTILE_CORE_API auto str_to_u32(StringView str, int base = 10) -> Maybe<uint32>;

/**
 * \brief Converts a string into a signed 32-bit integer.
 *
 * \param str  the source string.
 * \param base the numerical base.
 *
 * \return the converted value, or nothing if an error occurred.
 */
[[nodiscard]]
TACTILE_CORE_API auto str_to_i32(StringView str, int base = 10) -> Maybe<int32>;

/**
 * \brief Converts a string into a 32-bit float.
 *
 * \param str the source string.
 *
 * \return the converted value, or nothing if an error occurred.
 */
[[nodiscard]]
TACTILE_CORE_API auto str_to_f32(StringView str) -> Maybe<float32>;

}  // namespace tactile
