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

#include "common/attribute.hpp"

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"

using namespace std::string_literals;

using namespace tactile;

TEST_SUITE("Attribute")
{
  TEST_CASE("Defaults")
  {
    const Attribute value;
    CHECK(value.get_type() == AttributeType::String);

    CHECK(value.is_string());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());

    CHECK(value.as_string() == "");
    CHECK_THROWS_AS(value.as_int(), TactileError);
    CHECK_THROWS_AS(value.as_int2(), TactileError);
    CHECK_THROWS_AS(value.as_int3(), TactileError);
    CHECK_THROWS_AS(value.as_int4(), TactileError);
    CHECK_THROWS_AS(value.as_float(), TactileError);
    CHECK_THROWS_AS(value.as_float2(), TactileError);
    CHECK_THROWS_AS(value.as_float3(), TactileError);
    CHECK_THROWS_AS(value.as_float4(), TactileError);
    CHECK_THROWS_AS(value.as_bool(), TactileError);
    CHECK_THROWS_AS(value.as_color(), TactileError);
    CHECK_THROWS_AS(value.as_path(), TactileError);
    CHECK_THROWS_AS(value.as_object(), TactileError);
  }

  TEST_CASE("Int attribute")
  {
    const Attribute value {123};
    CHECK(value.as_int() == 123);

    CHECK(value.is_int());
    CHECK(value.try_as_int());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("Int2 attribute")
  {
    const Attribute value {Int2 {1, 2}};
    CHECK(value.as_int2() == Int2 {1, 2});

    CHECK(value.is_int2());
    CHECK(value.try_as_int2());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("Int3 attribute")
  {
    const Attribute value {Int3 {1, 2, 3}};
    CHECK(value.as_int3() == Int3 {1, 2, 3});

    CHECK(value.is_int3());
    CHECK(value.try_as_int3());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("Int4 attribute")
  {
    const Attribute value {Int4 {1, 2, 3, 4}};
    CHECK(value.as_int4() == Int4 {1, 2, 3, 4});

    CHECK(value.is_int4());
    CHECK(value.try_as_int4());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("Float attribute")
  {
    const Attribute value {12.3f};
    CHECK(value.as_float() == 12.3f);

    CHECK(value.is_float());
    CHECK(value.try_as_float());

    CHECK(!value.is_string());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("Float2 attribute")
  {
    const Attribute value {Float2 {24, 48}};
    CHECK(value.as_float2() == Float2 {24, 48});

    CHECK(value.is_float2());
    CHECK(value.try_as_float2());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("Float3 attribute")
  {
    const Attribute value {Float3 {24, 48, 64}};
    CHECK(value.as_float3() == Float3 {24, 48, 64});

    CHECK(value.is_float3());
    CHECK(value.try_as_float3());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("Float4 attribute")
  {
    const Attribute value {Float4 {24, 48, 64, 96}};
    CHECK(value.as_float4() == Float4 {24, 48, 64, 96});

    CHECK(value.is_float4());
    CHECK(value.try_as_float4());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("String attribute")
  {
    const Attribute value {"foo"s};
    CHECK(value.as_string() == "foo");

    CHECK(value.is_string());
    CHECK(value.try_as_string());

    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("Bool attribute")
  {
    const Attribute value {false};
    CHECK(!value.as_bool());

    CHECK(value.is_bool());
    CHECK(value.try_as_bool());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("File attribute")
  {
    const std::filesystem::path file {"resources/foo.txt"};
    const Attribute value {file};

    CHECK(value.is_path());
    CHECK(value.try_as_path());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_object());
  }

  TEST_CASE("Object attribute")
  {
    const Attribute value {ObjectRef {7}};

    CHECK(value.is_object());
    CHECK(value.try_as_object());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_color());
    CHECK(!value.is_path());
  }

  TEST_CASE("Color attribute")
  {
    const Attribute value {Color {0xFF, 0, 0}};

    CHECK(value.is_color());
    CHECK(value.try_as_color());

    CHECK(!value.is_string());
    CHECK(!value.is_float());
    CHECK(!value.is_float2());
    CHECK(!value.is_float3());
    CHECK(!value.is_float4());
    CHECK(!value.is_int());
    CHECK(!value.is_int2());
    CHECK(!value.is_int3());
    CHECK(!value.is_int4());
    CHECK(!value.is_bool());
    CHECK(!value.is_path());
    CHECK(!value.is_object());
  }

  TEST_CASE("set_value")
  {
    Attribute value;

    value.set_value(10);
    CHECK(value.is_int());
    CHECK(value.as_int() == 10);

    value.set_value(93.2f);
    CHECK(value.is_float());
    CHECK(value.as_float() == 93.2f);

    value.set_value(true);
    CHECK(value.is_bool());
    CHECK(value.as_bool());

    value.set_value("foo"s);
    CHECK(value.is_string());
    CHECK(value.as_string() == "foo");
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

    CHECK(!i.is_any_vector());
    CHECK(!f.is_any_vector());
    CHECK(!b.is_any_vector());
    CHECK(!c.is_any_vector());
    CHECK(!p.is_any_vector());
    CHECK(!o.is_any_vector());

    CHECK(i2.is_any_vector());
    CHECK(i3.is_any_vector());
    CHECK(i4.is_any_vector());

    CHECK(f2.is_any_vector());
    CHECK(f3.is_any_vector());
    CHECK(f4.is_any_vector());
  }
}
