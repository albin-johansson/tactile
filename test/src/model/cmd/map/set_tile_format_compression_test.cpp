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

#include "model/cmd/map/set_tile_format_compression.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "core/debug/panic.hpp"

namespace tactile::test {

TEST(SetTileFormatCompression, Constructor)
{
  ASSERT_THROW(SetTileFormatCompression(nullptr, TileCompression::None), TactileError);
}

TEST(SetTileFormatCompression, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& format = map->tile_format();
  format.set_encoding(TileEncoding::Base64);

  ASSERT_EQ(TileEncoding::Base64, format.encoding());
  ASSERT_EQ(TileCompression::None, format.compression());

  SetTileFormatCompression cmd {map, TileCompression::Zlib};

  cmd.redo();
  ASSERT_EQ(TileCompression::Zlib, format.compression());
  ASSERT_EQ(TileEncoding::Base64, format.encoding());

  cmd.undo();
  ASSERT_EQ(TileCompression::None, format.compression());
  ASSERT_EQ(TileEncoding::Base64, format.encoding());
}

}  // namespace tactile::test
