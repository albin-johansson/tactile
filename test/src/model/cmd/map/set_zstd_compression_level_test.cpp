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

#include "model/cmd/map/set_zstd_compression_level.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(SetZstdCompressionLevel, Constructor)
{
  ASSERT_THROW(cmd::SetZstdCompressionLevel(nullptr, 0), TactileError);
}

TEST(SetZstdCompressionLevel, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  cmd::SetZstdCompressionLevel cmd {map, 16};

  cmd.redo();
  ASSERT_EQ(16, map->tile_format().zstd_compression_level());

  cmd.undo();
  ASSERT_EQ(3, map->tile_format().zstd_compression_level());
}

TEST(SetZstdCompressionLevel, MergeSupport)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  map->tile_format().set_zstd_compression_level(8);

  cmd::SetZstdCompressionLevel a {map, 5};
  const cmd::SetZstdCompressionLevel b {map, 12};
  const cmd::SetZstdCompressionLevel c {map, 18};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ(18, map->tile_format().zstd_compression_level());

  a.undo();
  ASSERT_EQ(8, map->tile_format().zstd_compression_level());
}

}  // namespace tactile::test
