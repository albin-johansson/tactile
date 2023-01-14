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

#include "model/cmd/layer/remove_layer.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"
#include "core/layer/group_layer.hpp"

namespace tactile::test {

TEST_SUITE("cmd::RemoveLayer")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::RemoveLayer(nullptr, make_uuid()), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    UUID layer_id;

    auto map_document = MapBuilder::build()  //
                            .with_object_layer(&layer_id)
                            .result();

    auto& map = map_document->get_map();
    auto& root = map.get_invisible_root();
    auto& contexts = map_document->get_contexts();

    REQUIRE(contexts.contains(layer_id));

    cmd::RemoveLayer cmd {map_document.get(), layer_id};

    cmd.redo();
    REQUIRE(0u == root.layer_count());
    REQUIRE(!contexts.contains(layer_id));

    cmd.undo();
    REQUIRE(1u == root.layer_count());
    REQUIRE(contexts.contains(layer_id));
  }
}

}  // namespace tactile::test
