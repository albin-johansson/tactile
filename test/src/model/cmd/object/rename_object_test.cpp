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

#include "model/cmd/object/rename_object.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::RenameObject")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::RenameObject(nullptr, ""), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    Shared<Object> object;
    const auto map_document [[maybe_unused]] = test::MapBuilder::build()  //
                                                   .with_object(ObjectType::Rect, &object)
                                                   .result();

    const String old_name {"foo"};
    const String new_name {"bar"};

    object->get_ctx().set_name(old_name);
    cmd::RenameObject cmd {object, new_name};

    cmd.redo();
    REQUIRE(new_name == object->get_ctx().name());

    cmd.undo();
    REQUIRE(old_name == object->get_ctx().name());
  }

  TEST_CASE("merge_with")
  {
    Shared<Object> object;
    const auto map_document [[maybe_unused]] =
        test::MapBuilder::build()  //
            .with_object(ObjectType::Ellipse, &object)
            .result();

    const String initial_name {"initial"};
    object->get_ctx().set_name(initial_name);

    cmd::RenameObject a {object, "a"};
    const cmd::RenameObject b {object, "b"};
    const cmd::RenameObject c {object, "c"};

    REQUIRE(a.merge_with(&b));
    REQUIRE(a.merge_with(&c));

    a.redo();
    REQUIRE("c" == object->get_ctx().name());

    a.undo();
    REQUIRE(initial_name == object->get_ctx().name());
  }
}

}  // namespace tactile::test
