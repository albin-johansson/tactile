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

#include "set_tile_format_encoding.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

SetTileFormatEncoding::SetTileFormatEncoding(Shared<Map> map, const TileEncoding encoding)
    : mMap {std::move(map)},
      mNewEncoding {encoding}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void SetTileFormatEncoding::undo()
{
  auto& format = mMap->get_tile_format();

  format.set_encoding(mOldEncoding.value());
  format.set_compression(mOldCompression.value());

  mOldEncoding.reset();
  mOldCompression.reset();
}

void SetTileFormatEncoding::redo()
{
  auto& format = mMap->get_tile_format();

  mOldEncoding = format.encoding();
  mOldCompression = format.compression();

  format.set_encoding(mNewEncoding);
}

auto SetTileFormatEncoding::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.set_tile_format_encoding;
}

}  // namespace tactile
