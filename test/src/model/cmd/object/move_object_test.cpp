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

#include "model/cmd/object/move_object.hpp"

#include <doctest/doctest.h>

#include "core/helpers/map_builder.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("cmd::MoveObject")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::MoveObject(nullptr, {}, {}), Exception);
  }

  TEST_CASE("redo/undo")
  {
    Shared<Object> object;

    auto map_document = MapBuilder::build()  //
                            .with_object(ObjectType::Rect, &object)
                            .result();

    const Float2 old_pos {843, 317};
    const Float2 new_pos {-835, 94};

    object->set_pos(old_pos);
    cmd::MoveObject cmd {object, old_pos, new_pos};

    cmd.redo();
    REQUIRE(new_pos == object->get_pos());

    cmd.undo();
    REQUIRE(old_pos == object->get_pos());
  }
}

}  // namespace tactile::test
