// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/emit/tmj_object_emitter.hpp"

#include <gtest/gtest.h>

using namespace tactile;

class ParameterizedTmjObjectEmitterTest : public testing::TestWithParam<ObjectType> {};

INSTANTIATE_TEST_SUITE_P(AllObjectTypes,
                         ParameterizedTmjObjectEmitterTest,
                         testing::Values(ObjectType::kRect,
                                         ObjectType::kEllipse,
                                         ObjectType::kPoint));

/// \tests tactile::tmj::emit_object
TEST_P(ParameterizedTmjObjectEmitterTest, EmitObject)
{
  const ObjectType object_type = GetParam();

  const ir::Object object {
    .meta =
        {
          .properties =
              {
                ir::NamedAttribute {.name = "P1", .value = 42},
                ir::NamedAttribute {.name = "P2", .value = "XYZ"},
              },
          .components = {},
        },
    .name = "ObjectName",
    .id = ObjectID {72},
    .type = object_type,
    .x = 83,
    .y = -21,
    .width = 129,
    .height = 310,
    .tag = "Object::tag",
    .visible = true,
  };

  const auto object_json = tmj::emit_object(object);
  ASSERT_TRUE(object_json.is_object());

  EXPECT_EQ(object_json.at("name"), object.name);
  EXPECT_EQ(object_json.at("id"), object.id.value);
  EXPECT_EQ(object_json.at("x"), object.x);
  EXPECT_EQ(object_json.at("y"), object.y);
  EXPECT_EQ(object_json.at("width"), object.width);
  EXPECT_EQ(object_json.at("height"), object.height);
  EXPECT_EQ(object_json.at("type"), object.tag);
  EXPECT_EQ(object_json.at("visible"), object.visible);

  if (object_type == ObjectType::kPoint) {
    EXPECT_TRUE(object_json.contains("point"));
    EXPECT_FALSE(object_json.contains("ellipse"));
  }
  else if (object_type == ObjectType::kEllipse) {
    EXPECT_FALSE(object_json.contains("point"));
    EXPECT_TRUE(object_json.contains("ellipse"));
  }
  else {
    EXPECT_FALSE(object_json.contains("point"));
    EXPECT_FALSE(object_json.contains("ellipse"));
  }

  ASSERT_TRUE(object_json.contains("properties"));
  ASSERT_EQ(object_json.at("properties").size(), object.meta.properties.size());

  EXPECT_EQ(object_json.at("properties").at(0).at("name"), "P1");
  EXPECT_EQ(object_json.at("properties").at(0).at("type"), "int");
  EXPECT_EQ(object_json.at("properties").at(0).at("value"), 42);

  EXPECT_EQ(object_json.at("properties").at(1).at("name"), "P2");
  EXPECT_EQ(object_json.at("properties").at(1).at("type"), "string");
  EXPECT_EQ(object_json.at("properties").at(1).at("value"), "XYZ");
}
