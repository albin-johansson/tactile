// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>       // basic_string
#include <string_view>  // basic_string_view

#include "tactile/base/prelude.hpp"

#if TACTILE_OS_WINDOWS
  #define TACTILE_NATIVE_STR(Str) L##Str
#else
  #define TACTILE_NATIVE_STR(Str) Str
#endif

#define TACTILE_NATIVE_CHAR(Ch) TACTILE_NATIVE_STR(Ch)

namespace tactile {

using NativeString = std::basic_string<wchar_t>;
using NativeStringView = std::basic_string_view<wchar_t>;

}  // namespace tactile
