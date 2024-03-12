// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>       // basic_string, string
#include <string_view>  // basic_string_view, string_view, wstring_view

#include "tactile/base/container/path.hpp"

namespace tactile {

using String = std::string;
using StringView = std::string_view;

using WString = std::wstring;
using WStringView = std::wstring_view;

using OsStrChar = Path::value_type;
using OsString = std::basic_string<OsStrChar>;
using OsStringView = std::basic_string_view<OsStrChar>;

}  // namespace tactile
