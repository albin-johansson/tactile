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

#include "model/cmd/layer/rename_layer.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "core/layer/group_layer.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(RenameLayer, Constructor)
{
  ASSERT_THROW(cmd::RenameLayer(nullptr, make_uuid(), ""), TactileError);
}

TEST(RenameLayer, RedoUndo)
{
  UUID layer_id;

  auto document = MapBuilder::build().with_tile_layer(&layer_id).result();
  auto map = document->get_map_ptr();

  auto layer = map->invisible_root().get_layer_ptr(layer_id);
  layer->get_ctx().set_name("barfoo");

  cmd::RenameLayer cmd {map, layer_id, "foobar"};

  cmd.redo();
  ASSERT_EQ("foobar", layer->get_ctx().name());

  cmd.undo();
  ASSERT_EQ("barfoo", layer->get_ctx().name());
}

}  // namespace tactile::test