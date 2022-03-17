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

#include "parse_data.hpp"

namespace tactile::parsing {

void ParseData::set_path(const std::filesystem::path& path)
{
  mPath = std::filesystem::absolute(path);
}

void ParseData::set_error(const ParseError error)
{
  mError = error;
}

auto ParseData::path() const -> const std::filesystem::path&
{
  return mPath;
}

auto ParseData::error() const -> ParseError
{
  return mError;
}

auto ParseData::data() -> ir::MapData&
{
  return mData;
}

auto ParseData::data() const -> const ir::MapData&
{
  return mData;
}

}  // namespace tactile::parsing