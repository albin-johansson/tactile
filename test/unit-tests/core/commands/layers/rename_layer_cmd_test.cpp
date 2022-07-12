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

#include "core/cmd/layer/rename_layer_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RenameLayerCmd, Constructor)
{
  ASSERT_THROW(RenameLayerCmd(nullptr, make_uuid(), ""), TactileError);
}

TEST(RenameLayerCmd, RedoUndo)
{
  UUID layerId;

  auto document = MapBuilder::build().with_tile_layer(&layerId).result();
  auto map = document->get_map_ptr();

  auto layer = map->get_layer(layerId);
  layer->set_name("barfoo");

  RenameLayerCmd cmd{map, layerId, "foobar"};

  cmd.redo();
  ASSERT_EQ("foobar", layer->get_name());

  cmd.undo();
  ASSERT_EQ("barfoo", layer->get_name());
}

}  // namespace tactile::test