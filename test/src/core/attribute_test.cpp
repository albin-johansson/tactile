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

#include "core/attribute.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"

using namespace std::string_literals;

namespace tactile::test {

TEST_SUITE("Attribute")
{
  TEST_CASE("Defaults")
  {
    const Attribute value;
    REQUIRE(AttributeType::String == value.type());

    REQUIRE(value.is_string());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());

    REQUIRE("" == value.as_string());
    REQUIRE_THROWS_AS(value.as_int(), TactileError);
    REQUIRE_THROWS_AS(value.as_float(), TactileError);
    REQUIRE_THROWS_AS(value.as_bool(), TactileError);
    REQUIRE_THROWS_AS(value.as_color(), TactileError);
    REQUIRE_THROWS_AS(value.as_path(), TactileError);
    REQUIRE_THROWS_AS(value.as_object(), TactileError);
  }

  TEST_CASE("Int attribute")
  {
    const Attribute value {123};
    REQUIRE(123 == value.as_int());

    REQUIRE(value.is_int());
    REQUIRE(value.try_as_int());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Float attribute")
  {
    const Attribute value {12.3f};
    REQUIRE(12.3f == value.as_float());

    REQUIRE(value.is_float());
    REQUIRE(value.try_as_float());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("String attribute")
  {
    const Attribute value {"foo"s};
    REQUIRE("foo" == value.as_string());

    REQUIRE(value.is_string());
    REQUIRE(value.try_as_string());

    REQUIRE(!value.is_int());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Bool attribute")
  {
    const Attribute value {false};
    REQUIRE(!value.as_bool());

    REQUIRE(value.is_bool());
    REQUIRE(value.try_as_bool());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("File attribute")
  {
    const std::filesystem::path file {"resources/foo.txt"};
    const Attribute value {file};

    REQUIRE(value.is_path());
    REQUIRE(value.try_as_path());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Object attribute")
  {
    const Attribute value {object_t {7}};

    REQUIRE(value.is_object());
    REQUIRE(value.try_as_object());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
  }

  TEST_CASE("Color attribute")
  {
    const Attribute value {Color {0xFF, 0, 0}};

    REQUIRE(value.is_color());
    REQUIRE(value.try_as_color());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("set_value")
  {
    Attribute value;

    value.set_value(10);
    REQUIRE(value.is_int());
    REQUIRE(10 == value.as_int());

    value.set_value(93.2f);
    REQUIRE(value.is_float());
    REQUIRE(93.2f == value.as_float());

    value.set_value(true);
    REQUIRE(value.is_bool());
    REQUIRE(value.as_bool());

    value.set_value("foo"s);
    REQUIRE(value.is_string());
    REQUIRE("foo" == value.as_string());
  }
}

}  // namespace tactile::test
