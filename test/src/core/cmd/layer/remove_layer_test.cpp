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

#include "core/cmd/layer/remove_layer.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(RemoveLayer, Constructor)
{
  ASSERT_THROW(cmd::RemoveLayer(nullptr, make_uuid()), TactileError);
}

TEST(RemoveLayer, RedoUndo)
{
  UUID layer_id;

  auto document = MapBuilder::build()  //
                      .with_object_layer(&layer_id)
                      .result();

  auto& map = document->get_map();
  auto& root = map.invisible_root();
  auto& contexts = document->get_contexts();

  ASSERT_TRUE(contexts.contains(layer_id));

  cmd::RemoveLayer cmd {document.get(), layer_id};

  cmd.redo();
  ASSERT_EQ(0, root.size());
  ASSERT_FALSE(contexts.contains(layer_id));

  cmd.undo();
  ASSERT_EQ(1, root.size());
  ASSERT_TRUE(contexts.contains(layer_id));
}

}  // namespace tactile::test
