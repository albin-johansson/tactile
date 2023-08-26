// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>       // string, basic_string
#include <string_view>  // string_view, basic_string_view
#include <type_traits>  // conditional_t

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/maybe.hpp"
#include "tactile/core/type/vector.hpp"

namespace tactile {

using String = std::string;
using StringView = std::string_view;

using NativeChar = std::conditional_t<kIsWindows, wchar_t, char>;
using NativeString = std::basic_string<NativeChar>;
using NativeStringView = std::basic_string_view<NativeChar>;

/** Splits a string into a collection of tokens, delimited by a specific character. */
[[nodiscard]] TACTILE_CORE_API auto str_split(StringView str, char separator)
    -> Vector<String>;

/** Converts a string into an unsigned 32-bit integer. */
[[nodiscard]] TACTILE_CORE_API auto str_to_u32(StringView str, int base = 10)
    -> Maybe<uint32>;

/** Converts a string into a signed 32-bit integer. */
[[nodiscard]] TACTILE_CORE_API auto str_to_i32(StringView str, int base = 10)
    -> Maybe<int32>;

/** Converts a string into a 32-bit float. */
[[nodiscard]] TACTILE_CORE_API auto str_to_f32(StringView str) -> Maybe<float32>;

}  // namespace tactile
