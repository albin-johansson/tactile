/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <doctest/doctest.h>

#include "io/map/parse/parse_map.hpp"

using namespace tactile;

namespace {

void _check_parsed_properties(const char* path)
{
  const auto result = parse_map(path);
  REQUIRE(result.has_value());

  const auto& ir_map = result.value();
  REQUIRE(3u == ir_map.context.properties.size());

  const auto& color = ir_map.context.properties.at("empty-color");
  REQUIRE(color.is_color());
  REQUIRE(color.has_default_value());

  const auto& file = ir_map.context.properties.at("empty-file");
  REQUIRE(file.is_path());
  REQUIRE(file.has_default_value());

  const auto& obj = ir_map.context.properties.at("empty-object");
  REQUIRE(obj.is_object());
  REQUIRE(obj.has_default_value());
}

}  // namespace

TEST_SUITE("Tiled format property special cases")
{
  TEST_CASE("Tiled JSON")
  {
    _check_parsed_properties("resources/json/properties.tmj");
  }

  TEST_CASE("Tiled XML")
  {
    _check_parsed_properties("resources/xml/properties.tmx");
  }
}
