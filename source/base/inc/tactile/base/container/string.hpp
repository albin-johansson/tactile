// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>       // basic_string, string
#include <string_view>  // basic_string_view, string_view, wstring_view
#include <type_traits>  // conditional_t

#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"

#if TACTILE_OS_WINDOWS
  #define TACTILE_NATIVE_STR(Str) L##Str
#else
  #define TACTILE_NATIVE_STR(Str) Str
#endif

#define TACTILE_NATIVE_CHAR(Ch) TACTILE_NATIVE_STR(Ch)

namespace tactile {

using String = std::string;
using StringView = std::string_view;

using NativeChar = std::conditional_t<kOnWindows, wchar_t, char>;
using NativeString = std::basic_string<NativeChar>;
using NativeStringView = std::basic_string_view<NativeChar>;

}  // namespace tactile
