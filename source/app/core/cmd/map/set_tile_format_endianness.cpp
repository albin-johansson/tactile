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

#include "set_tile_format_endianness.hpp"

#include <utility>  // move

#include "core/map.hpp"
#include "misc/panic.hpp"

namespace tactile {

SetTileFormatEndianness::SetTileFormatEndianness(Shared<Map>       map,
                                                 const std::endian endianness)
    : mMap {std::move(map)}
    , mNewEndianness {endianness}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void SetTileFormatEndianness::undo()
{
  auto& format = mMap->tile_format();

  format.set_endianness(mOldEndianness.value());
  mOldEndianness.reset();
}

void SetTileFormatEndianness::redo()
{
  auto& format = mMap->tile_format();

  mOldEndianness = format.endianness();
  format.set_endianness(mNewEndianness);
}

auto SetTileFormatEndianness::get_name() const -> std::string
{
  return "Update Tile Format Endianness";
}

}  // namespace tactile