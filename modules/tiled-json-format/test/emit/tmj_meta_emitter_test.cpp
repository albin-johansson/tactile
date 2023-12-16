// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-json-format/emit/tmj_meta_emitter.hpp"

#include <gtest/gtest.h>

#include "tactile/foundation/debug/exception.hpp"

using namespace tactile;
using namespace tactile::int_literals;
using namespace tactile::fs_literals;

/// \tests tactile::tmj::to_tmj_name
TEST(TmjMetaEmitter, ToTmjName)
{
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kInt), "int");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kFloat), "float");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kBool), "bool");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kStr), "string");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kPath), "file");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kColor), "color");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kObject), "object");

  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kInt2), "string");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kInt3), "string");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kInt4), "string");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kFloat2), "string");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kFloat3), "string");
  EXPECT_EQ(tmj::to_tmj_name(AttributeType::kFloat4), "string");

  EXPECT_THROW((void) tmj::to_tmj_name(AttributeType {100}), Exception);
}

/// \tests tactile::tmj::to_tmj_value
TEST(TmjMetaEmitter, ToTmjValue)
{
  EXPECT_EQ(tmj::to_tmj_value(42), 42);
  EXPECT_EQ(tmj::to_tmj_value(1.5f), 1.5f);
  EXPECT_EQ(tmj::to_tmj_value(true), true);
  EXPECT_EQ(tmj::to_tmj_value("foobar"), "foobar");
  EXPECT_EQ(tmj::to_tmj_value("foo/bar"_path), "foo/bar");
  EXPECT_EQ(tmj::to_tmj_value(UColor {0xDE, 0xAD, 0xBE, 0xEF}), "#EFDEADBE");
  EXPECT_EQ(tmj::to_tmj_value(ObjectRef {123}), 123);
}

/// \tests tactile::tmj::emit_property_array
TEST(TmjMetaEmitter, EmitPropertyArray)
{
  ir::Metadata meta {};
  meta.properties = {
    {.name = "A", .value = 10},
    {.name = "B", .value = 1.0f},
    {.name = "C", .value = false},
    {.name = "D", .value = "abc"},
    {.name = "E", .value = "a/b/c"_path},
    {.name = "F", .value = UColor {0xAA, 0xBB, 0xCC, 0xDD}},
    {.name = "G", .value = ObjectRef {99}},
  };

  const auto json_array = tmj::emit_property_array(meta);
  ASSERT_TRUE(json_array.is_array());
  ASSERT_EQ(json_array.size(), 7_uz);

  EXPECT_EQ(json_array.at(0).at("name"), "A");
  EXPECT_EQ(json_array.at(0).at("type"), "int");
  EXPECT_EQ(json_array.at(0).at("value"), 10);

  EXPECT_EQ(json_array.at(1).at("name"), "B");
  EXPECT_EQ(json_array.at(1).at("type"), "float");
  EXPECT_EQ(json_array.at(1).at("value"), 1.0f);

  EXPECT_EQ(json_array.at(2).at("name"), "C");
  EXPECT_EQ(json_array.at(2).at("type"), "bool");
  EXPECT_EQ(json_array.at(2).at("value"), false);

  EXPECT_EQ(json_array.at(3).at("name"), "D");
  EXPECT_EQ(json_array.at(3).at("type"), "string");
  EXPECT_EQ(json_array.at(3).at("value"), "abc");

  EXPECT_EQ(json_array.at(4).at("name"), "E");
  EXPECT_EQ(json_array.at(4).at("type"), "file");
  EXPECT_EQ(json_array.at(4).at("value"), "a/b/c");

  EXPECT_EQ(json_array.at(5).at("name"), "F");
  EXPECT_EQ(json_array.at(5).at("type"), "color");
  EXPECT_EQ(json_array.at(5).at("value"), "#DDAABBCC");

  EXPECT_EQ(json_array.at(6).at("name"), "G");
  EXPECT_EQ(json_array.at(6).at("type"), "object");
  EXPECT_EQ(json_array.at(6).at("value"), 99);
}
