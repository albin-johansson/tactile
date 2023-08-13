/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <filesystem>   // path, directory_iterator, recursive_directory_iterator
#include <string>       // basic_string
#include <string_view>  // basic_string_view

namespace tactile {

namespace fs = std::filesystem;

using Path = fs::path;
using DirectoryIterator = fs::directory_iterator;
using RecursiveDirectoryIterator = fs::recursive_directory_iterator;

using NativeChar = Path::value_type;
using NativeString = std::basic_string<NativeChar>;
using NativeStringView = std::basic_string_view<NativeChar>;

}  // namespace tactile
