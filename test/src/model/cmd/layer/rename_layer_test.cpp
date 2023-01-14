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

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"
#include "core/layer/group_layer.hpp"

namespace tactile::test {

TEST_SUITE("cmd::RenameLayer")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::RenameLayer(nullptr, make_uuid(), ""), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    UUID layer_id;

    auto map_document = MapBuilder::build().with_tile_layer(&layer_id).result();
    auto map = map_document->get_map_ptr();

    auto layer = map->get_invisible_root().find_shared_layer(layer_id);
    layer->get_ctx().set_name("barfoo");

    cmd::RenameLayer cmd {map, layer_id, "foobar"};

    cmd.redo();
    REQUIRE("foobar" == layer->get_ctx().name());

    cmd.undo();
    REQUIRE("barfoo" == layer->get_ctx().name());
  }
}

}  // namespace tactile::test