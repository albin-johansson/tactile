/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>       // basic_string, string
#include <string_view>  // basic_string_view, string_view, wstring_view

#include "core/type/path.hpp"

namespace tactile {

using String = std::string;
using StringView = std::string_view;

using WString = std::wstring;
using WStringView = std::wstring_view;

using OsStrChar = Path::value_type;
using OsString = std::basic_string<OsStrChar>;
using OsStringView = std::basic_string_view<OsStrChar>;

}  // namespace tactile
