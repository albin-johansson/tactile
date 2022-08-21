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

#include "core/cmd/map/set_zlib_compression_level.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(SetZlibCompressionLevel, Constructor)
{
  ASSERT_THROW(cmd::SetZlibCompressionLevel(nullptr, 0), TactileError);
}

TEST(SetZlibCompressionLevel, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  cmd::SetZlibCompressionLevel cmd {map, 4};

  cmd.redo();
  ASSERT_EQ(4, map->tile_format().zlib_compression_level());

  cmd.undo();
  ASSERT_EQ(-1, map->tile_format().zlib_compression_level());
}

TEST(SetZlibCompressionLevel, MergeSupport)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  map->tile_format().set_zlib_compression_level(7);

  cmd::SetZlibCompressionLevel       a {map, 6};
  const cmd::SetZlibCompressionLevel b {map, 4};
  const cmd::SetZlibCompressionLevel c {map, 9};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ(9, map->tile_format().zlib_compression_level());

  a.undo();
  ASSERT_EQ(7, map->tile_format().zlib_compression_level());
}

}  // namespace tactile::test
