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

#include "model/cmd/object/set_object_visible.hpp"

#include <doctest/doctest.h>

#include "core/helpers/map_builder.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("cmd::SetObjectVisible")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::SetObjectVisible(nullptr, false), Exception);
  }

  TEST_CASE("redo/undo")
  {
    Shared<Object> object;
    const auto map_document [[maybe_unused]] = test::MapBuilder::build()  //
                                                   .with_object(ObjectType::Rect, &object)
                                                   .result();

    cmd::SetObjectVisible cmd {object, false};

    cmd.redo();
    REQUIRE(!object->is_visible());

    cmd.undo();
    REQUIRE(object->is_visible());
  }
}

}  // namespace tactile::test
