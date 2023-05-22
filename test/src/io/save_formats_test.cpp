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

#include "io/save_formats.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"

namespace tactile::test {

TEST_SUITE("Save formats")
{
  TEST_CASE("Human-readable save format names")
  {
    CHECK(get_human_readable_name(SaveFormat::TactileYaml) == "YAML");
    CHECK(get_human_readable_name(SaveFormat::TiledJson) == "TMJ (JSON)");
    CHECK(get_human_readable_name(SaveFormat::TiledXml) == "TMX (XML)");

    CHECK_THROWS_AS(get_human_readable_name(SaveFormat {42}), TactileError);
  }

  TEST_CASE("Default file extensions")
  {
    CHECK(get_file_extension(SaveFormat::TactileYaml) == ".yml");
    CHECK(get_file_extension(SaveFormat::TiledJson) == ".tmj");
    CHECK(get_file_extension(SaveFormat::TiledXml) == ".tmx");

    CHECK_THROWS_AS(get_file_extension(SaveFormat {100}), TactileError);
  }

  TEST_CASE("Tactile YAML file extensions")
  {
    CHECK(has_supported_tactile_yaml_extension("foo.yml"));
    CHECK(has_supported_tactile_yaml_extension("foo.yaml"));

    CHECK(!has_supported_tactile_yaml_extension(""));
    CHECK(!has_supported_tactile_yaml_extension("."));
    CHECK(!has_supported_tactile_yaml_extension("foo"));
    CHECK(!has_supported_tactile_yaml_extension("foo.ymll"));
    CHECK(!has_supported_tactile_yaml_extension("foo.xml"));
    CHECK(!has_supported_tactile_yaml_extension("foo.json"));
    CHECK(!has_supported_tactile_yaml_extension("foo.tmy"));
  }

  TEST_CASE("Tiled JSON file extensions")
  {
    CHECK(has_supported_tiled_json_extension("foo.tmj"));
    CHECK(has_supported_tiled_json_extension("foo.json"));

    CHECK(!has_supported_tiled_json_extension(""));
    CHECK(!has_supported_tiled_json_extension("."));
    CHECK(!has_supported_tiled_json_extension("foo"));
    CHECK(!has_supported_tiled_json_extension("foo.jsn"));
    CHECK(!has_supported_tiled_json_extension("foo.yaml"));
    CHECK(!has_supported_tiled_json_extension("foo.xml"));
    CHECK(!has_supported_tiled_json_extension("foo.tmx"));
  }

  TEST_CASE("Tiled XML file extensions")
  {
    CHECK(has_supported_tiled_xml_extension("foo.tmx"));
    CHECK(has_supported_tiled_xml_extension("foo.xml"));

    CHECK(!has_supported_tiled_xml_extension(""));
    CHECK(!has_supported_tiled_xml_extension("."));
    CHECK(!has_supported_tiled_xml_extension("foo"));
    CHECK(!has_supported_tiled_xml_extension("foo.json"));
    CHECK(!has_supported_tiled_xml_extension("foo.yaml"));
    CHECK(!has_supported_tiled_xml_extension("foo.tmz"));
    CHECK(!has_supported_tiled_xml_extension("foo.xm"));
  }

  TEST_CASE("Vector serialization")
  {
    CHECK(serialize_to_save_format(Int2 {12, 34}) == "12;34");
    CHECK(serialize_to_save_format(Int3 {12, 34, 56}) == "12;34;56");
    CHECK(serialize_to_save_format(Int4 {12, 34, 56, 78}) == "12;34;56;78");

    CHECK(serialize_to_save_format(Float2 {1.2f, 3.4f}) == "1.2;3.4");
    CHECK(serialize_to_save_format(Float3 {1.2f, 3.4f, 5.6f}) == "1.2;3.4;5.6");
    CHECK(serialize_to_save_format(Float4 {1.2f, 3.4f, 5.6f, 7.8f}) == "1.2;3.4;5.6;7.8");

    CHECK(serialize_to_save_format(Float4 {10, 20, 30, 40}) == "10;20;30;40");
  }

  TEST_CASE("Attribute type serialization")
  {
    CHECK(serialize_to_save_format(AttributeType::String) == "string");
    CHECK(serialize_to_save_format(AttributeType::Object) == "object");
    CHECK(serialize_to_save_format(AttributeType::Color) == "color");
    CHECK(serialize_to_save_format(AttributeType::Bool) == "bool");

    CHECK(serialize_to_save_format(AttributeType::Int) == "int");
    CHECK(serialize_to_save_format(AttributeType::Int2) == "int2");
    CHECK(serialize_to_save_format(AttributeType::Int3) == "int3");
    CHECK(serialize_to_save_format(AttributeType::Int4) == "int4");

    CHECK(serialize_to_save_format(AttributeType::Float) == "float");
    CHECK(serialize_to_save_format(AttributeType::Float2) == "float2");
    CHECK(serialize_to_save_format(AttributeType::Float3) == "float3");
    CHECK(serialize_to_save_format(AttributeType::Float4) == "float4");
  }

  TEST_CASE("Attribute type parsing")
  {
    CHECK(parse_attr_type("string") == AttributeType::String);
    CHECK(parse_attr_type("object") == AttributeType::Object);
    CHECK(parse_attr_type("color") == AttributeType::Color);
    CHECK(parse_attr_type("bool") == AttributeType::Bool);

    CHECK(parse_attr_type("path") == AttributeType::Path);
    CHECK(parse_attr_type("file") == AttributeType::Path);

    CHECK(parse_attr_type("int") == AttributeType::Int);
    CHECK(parse_attr_type("int2") == AttributeType::Int2);
    CHECK(parse_attr_type("int3") == AttributeType::Int3);
    CHECK(parse_attr_type("int4") == AttributeType::Int4);

    CHECK(parse_attr_type("float") == AttributeType::Float);
    CHECK(parse_attr_type("float2") == AttributeType::Float2);
    CHECK(parse_attr_type("float3") == AttributeType::Float3);
    CHECK(parse_attr_type("float4") == AttributeType::Float4);

    CHECK(!parse_attr_type("").has_value());
    CHECK(!parse_attr_type("strings").has_value());
    CHECK(!parse_attr_type("int5").has_value());
  }
}

}  // namespace tactile::test