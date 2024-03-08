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

#include "tactile/core/debug/exception.hpp"

using namespace std::string_literals;

namespace tactile::test {

TEST_SUITE("Attribute")
{
  TEST_CASE("Defaults")
  {
    const Attribute value;
    REQUIRE(AttributeType::kStr == value.get_type());

    REQUIRE(value.is_string());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());

    REQUIRE("" == value.as_string());
    REQUIRE_THROWS_AS(value.as_int(), Exception);
    REQUIRE_THROWS_AS(value.as_int2(), Exception);
    REQUIRE_THROWS_AS(value.as_int3(), Exception);
    REQUIRE_THROWS_AS(value.as_int4(), Exception);
    REQUIRE_THROWS_AS(value.as_float(), Exception);
    REQUIRE_THROWS_AS(value.as_float2(), Exception);
    REQUIRE_THROWS_AS(value.as_float3(), Exception);
    REQUIRE_THROWS_AS(value.as_float4(), Exception);
    REQUIRE_THROWS_AS(value.as_bool(), Exception);
    REQUIRE_THROWS_AS(value.as_color(), Exception);
    REQUIRE_THROWS_AS(value.as_path(), Exception);
    REQUIRE_THROWS_AS(value.as_object(), Exception);
  }

  TEST_CASE("Int attribute")
  {
    const Attribute value {123};
    REQUIRE(123 == value.as_int());

    REQUIRE(value.is_int());
    REQUIRE(value.try_as_int());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Int2 attribute")
  {
    const Attribute value {Int2 {1, 2}};
    REQUIRE(Int2 {1, 2} == value.as_int2());

    REQUIRE(value.is_int2());
    REQUIRE(value.try_as_int2());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Int3 attribute")
  {
    const Attribute value {Int3 {1, 2, 3}};
    REQUIRE(Int3 {1, 2, 3} == value.as_int3());

    REQUIRE(value.is_int3());
    REQUIRE(value.try_as_int3());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int4());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Int4 attribute")
  {
    const Attribute value {Int4 {1, 2, 3, 4}};
    REQUIRE(Int4 {1, 2, 3, 4} == value.as_int4());

    REQUIRE(value.is_int4());
    REQUIRE(value.try_as_int4());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
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
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Float2 attribute")
  {
    const Attribute value {Float2 {24, 48}};
    REQUIRE(Float2 {24, 48} == value.as_float2());

    REQUIRE(value.is_float2());
    REQUIRE(value.try_as_float2());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Float3 attribute")
  {
    const Attribute value {Float3 {24, 48, 64}};
    REQUIRE(Float3 {24, 48, 64} == value.as_float3());

    REQUIRE(value.is_float3());
    REQUIRE(value.try_as_float3());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_path());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Float4 attribute")
  {
    const Attribute value {Float4 {24, 48, 64, 96}};
    REQUIRE(Float4 {24, 48, 64, 96} == value.as_float4());

    REQUIRE(value.is_float4());
    REQUIRE(value.try_as_float4());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
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

    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
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
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
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
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
    REQUIRE(!value.is_bool());
    REQUIRE(!value.is_color());
    REQUIRE(!value.is_object());
  }

  TEST_CASE("Object attribute")
  {
    const Attribute value {ObjectRef {7}};

    REQUIRE(value.is_object());
    REQUIRE(value.try_as_object());

    REQUIRE(!value.is_string());
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
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
    REQUIRE(!value.is_float());
    REQUIRE(!value.is_float2());
    REQUIRE(!value.is_float3());
    REQUIRE(!value.is_float4());
    REQUIRE(!value.is_int());
    REQUIRE(!value.is_int2());
    REQUIRE(!value.is_int3());
    REQUIRE(!value.is_int4());
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

  TEST_CASE("is_any_vector")
  {
    const Attribute i {42};
    const Attribute i2 = Int2 {1, 2};
    const Attribute i3 = Int3 {1, 2, 3};
    const Attribute i4 = Int4 {1, 2, 3, 4};
    const Attribute f {1.0f};
    const Attribute f2 = Float2 {1.0f, 2.0f};
    const Attribute f3 = Float3 {1.0f, 2.0f, 3.0f};
    const Attribute f4 = Float4 {1.0f, 2.0f, 3.0f, 4.0f};
    const Attribute b {true};
    const Attribute c {kWhite};
    const Attribute p = Path {"foo.txt"};
    const Attribute o = ObjectRef {10};

    REQUIRE(!i.is_any_vector());
    REQUIRE(!f.is_any_vector());
    REQUIRE(!b.is_any_vector());
    REQUIRE(!c.is_any_vector());
    REQUIRE(!p.is_any_vector());
    REQUIRE(!o.is_any_vector());

    REQUIRE(i2.is_any_vector());
    REQUIRE(i3.is_any_vector());
    REQUIRE(i4.is_any_vector());

    REQUIRE(f2.is_any_vector());
    REQUIRE(f3.is_any_vector());
    REQUIRE(f4.is_any_vector());
  }
}

}  // namespace tactile::test
