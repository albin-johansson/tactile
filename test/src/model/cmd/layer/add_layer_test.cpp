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

#include "model/cmd/layer/add_layer.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"
#include "core/layer/group_layer.hpp"

namespace tactile::test {

TEST_SUITE("cmd::AddLayer")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::AddLayer(nullptr, LayerType::TileLayer), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto& map = map_document->get_map();
    auto& root_layer = map.get_invisible_root();
    auto& context_manager = map_document->get_contexts();

    REQUIRE(1u == context_manager.context_count());
    REQUIRE(context_manager.has_context(map.get_uuid()));

    cmd::AddLayer cmd {map_document.get(), LayerType::TileLayer};

    cmd.redo();
    REQUIRE(1u == root_layer.layer_count());
    REQUIRE(2u == context_manager.context_count());
    REQUIRE(map.get_active_layer_id().has_value());

    const auto layer_id = map.get_active_layer_id().value();
    REQUIRE(context_manager.has_context(layer_id));

    cmd.undo();
    REQUIRE(0u == root_layer.layer_count());
    REQUIRE(1u == context_manager.context_count());
    REQUIRE(!map.get_active_layer_id().has_value());
    REQUIRE(!context_manager.has_context(layer_id));
  }
}

}  // namespace tactile::test
