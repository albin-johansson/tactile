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

#include "model/cmd/object/remove_object.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"

namespace tactile::test {

TEST_SUITE("cmd::RemoveObject")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::RemoveObject(nullptr, make_uuid(), make_uuid()), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    UUID layer_id;
    Shared<Object> object;

    auto map_document = MapBuilder::build()  //
                            .with_object(ObjectType::Rect, &object, &layer_id)
                            .result();

    auto map = map_document->get_map_ptr();
    auto& context_manager = map_document->get_contexts();

    auto& layer = map->invisible_root().get_object_layer(layer_id);
    cmd::RemoveObject cmd {map_document.get(), layer_id, object->get_uuid()};

    cmd.redo();
    REQUIRE(!context_manager.contains(object->get_uuid()));
    REQUIRE(!layer.has_object(object->get_uuid()));

    cmd.undo();
    REQUIRE(context_manager.contains(object->get_uuid()));
    REQUIRE(layer.has_object(object->get_uuid()));
  }
}

}  // namespace tactile::test