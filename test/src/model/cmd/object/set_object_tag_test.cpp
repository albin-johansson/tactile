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

#include "model/cmd/object/set_object_tag.hpp"

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::SetObjectTag")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::SetObjectTag(nullptr, ""), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    Shared<Object> object;
    const auto map_document [[maybe_unused]] = test::MapBuilder::build()  //
                                                   .with_object(ObjectType::Rect, &object)
                                                   .result();

    const String old_tag {"tag1"};
    const String new_tag {"tag2"};

    object->set_tag(old_tag);
    cmd::SetObjectTag cmd {object, new_tag};

    cmd.redo();
    REQUIRE(new_tag == object->get_tag());

    cmd.undo();
    REQUIRE(old_tag == object->get_tag());
  }

  TEST_CASE("merge_with")
  {
    Shared<Object> object;
    const auto map_document [[maybe_unused]] = test::MapBuilder::build()  //
                                                   .with_object(ObjectType::Rect, &object)
                                                   .result();

    const String initial_tag {"abc"};
    object->set_tag(initial_tag);

    cmd::SetObjectTag a {object, "a"};
    const cmd::SetObjectTag b {object, "b"};

    REQUIRE(a.merge_with(&b));

    a.redo();
    REQUIRE("b" == object->get_tag());

    a.undo();
    REQUIRE(initial_tag == object->get_tag());
  }
}

}  // namespace tactile::test
