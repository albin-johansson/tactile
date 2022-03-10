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

#include "emit_info.hpp"

#include <utility>  // move

namespace tactile::emitter {

EmitInfo::EmitInfo(std::filesystem::path destination, ir::MapData data)
    : mDestinationFile{std::move(destination)}
    , mDestinationDir{mDestinationFile.parent_path()}
    , mData{std::move(data)}
{}

auto EmitInfo::destination_file() const -> const std::filesystem::path&
{
  return mDestinationFile;
}

auto EmitInfo::destination_dir() const -> const std::filesystem::path&
{
  return mDestinationDir;
}

auto EmitInfo::data() const -> const ir::MapData&
{
  return mData;
}

}  // namespace tactile::emitter
