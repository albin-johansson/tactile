// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_object_emitter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/test_util/document_view_mocks.hpp"

namespace tactile::test {

// tactile::emit_tiled_tmj_object
TEST(TmjFormatObjectEmitter, EmitRectangleObject)
{
  ObjectViewMock object {};

  EXPECT_CALL(object, get_parent_layer).Times(0);
  EXPECT_CALL(object, get_parent_tile).Times(0);
  EXPECT_CALL(object, get_type).WillOnce(testing::Return(ObjectType::kRect));
  EXPECT_CALL(object, get_id).WillOnce(testing::Return(42));
  EXPECT_CALL(object, get_position).WillOnce(testing::Return(Float2 {1.0f, 2.0f}));
  EXPECT_CALL(object, get_size).WillOnce(testing::Return(Float2 {3.0f, 4.0f}));
  EXPECT_CALL(object, get_tag).WillOnce(testing::Return("RectangleTag"));
  EXPECT_CALL(object, is_visible).WillOnce(testing::Return(false));
  EXPECT_CALL(object, get_meta);

  const auto object_json = emit_tiled_tmj_object(object);

  EXPECT_EQ(object_json["id"], 42);
  EXPECT_EQ(object_json["x"], 1.0f);
  EXPECT_EQ(object_json["y"], 2.0f);
  EXPECT_EQ(object_json["width"], 3.0f);
  EXPECT_EQ(object_json["height"], 4.0f);
  EXPECT_EQ(object_json["type"], "RectangleTag");
  EXPECT_EQ(object_json["visible"], false);
  EXPECT_EQ(object_json["rotation"], 0);
  EXPECT_FALSE(object_json.contains("ellipse"));
  EXPECT_FALSE(object_json.contains("point"));
  EXPECT_FALSE(object_json.contains("properties"));
}

// tactile::emit_tiled_tmj_object
TEST(TmjFormatObjectEmitter, EmitEllipseObject)
{
  using testing::AnyOf;
  using testing::Eq;

  ObjectViewMock object {};
  auto& meta = object.get_meta_view_mock();

  const String prop1_name {"P1"};
  const String prop2_name {"P2"};
  const Attribute prop1_value {42};
  const Attribute prop2_value {"demo"};

  EXPECT_CALL(object, get_parent_layer).Times(0);
  EXPECT_CALL(object, get_parent_tile).Times(0);
  EXPECT_CALL(object, get_type).WillOnce(testing::Return(ObjectType::kEllipse));
  EXPECT_CALL(object, get_id).WillOnce(testing::Return(42));
  EXPECT_CALL(object, get_position).WillOnce(testing::Return(Float2 {1.0f, 2.0f}));
  EXPECT_CALL(object, get_size).WillOnce(testing::Return(Float2 {3.0f, 4.0f}));
  EXPECT_CALL(object, get_tag).WillOnce(testing::Return("EllipseTag"));
  EXPECT_CALL(object, is_visible).WillOnce(testing::Return(true));
  EXPECT_CALL(object, get_meta);

  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(2));
  EXPECT_CALL(meta, get_property)
      .WillOnce(testing::Return(std::make_pair(prop1_name, prop1_value)))
      .WillOnce(testing::Return(std::make_pair(prop2_name, prop2_value)));

  const auto object_json = emit_tiled_tmj_object(object);

  EXPECT_EQ(object_json["id"], 42);
  EXPECT_EQ(object_json["x"], 1.0f);
  EXPECT_EQ(object_json["y"], 2.0f);
  EXPECT_EQ(object_json["width"], 3.0f);
  EXPECT_EQ(object_json["height"], 4.0f);
  EXPECT_EQ(object_json["type"], "EllipseTag");
  EXPECT_EQ(object_json["visible"], true);
  EXPECT_EQ(object_json["rotation"], 0);
  EXPECT_TRUE(object_json.contains("ellipse"));
  EXPECT_FALSE(object_json.contains("point"));
  EXPECT_TRUE(object_json.contains("properties"));

  ASSERT_EQ(object_json["properties"].size(), 2);
  for (const auto& [_, property_json] : object_json["properties"].items()) {
    EXPECT_THAT(property_json.at("name"), AnyOf(Eq(prop1_name), Eq(prop2_name)));
    EXPECT_THAT(property_json.at("type"), AnyOf(Eq("int"), Eq("string")));
    EXPECT_THAT(property_json.at("value"),
                AnyOf(Eq(prop1_value.as_int()), Eq(prop2_value.as_string())));
  }
}

// tactile::emit_tiled_tmj_object
TEST(TmjFormatObjectEmitter, EmitPointObject)
{
  ObjectViewMock object {};

  EXPECT_CALL(object, get_parent_layer).Times(0);
  EXPECT_CALL(object, get_parent_tile).Times(0);
  EXPECT_CALL(object, get_type).WillOnce(testing::Return(ObjectType::kPoint));
  EXPECT_CALL(object, get_id).WillOnce(testing::Return(42));
  EXPECT_CALL(object, get_position).WillOnce(testing::Return(Float2 {10.0f, 20.0f}));
  EXPECT_CALL(object, get_size).WillOnce(testing::Return(Float2 {30.0f, 40.0f}));
  EXPECT_CALL(object, get_tag).WillOnce(testing::Return("PointTag"));
  EXPECT_CALL(object, is_visible).WillOnce(testing::Return(true));
  EXPECT_CALL(object, get_meta);

  const auto object_json = emit_tiled_tmj_object(object);

  EXPECT_EQ(object_json["id"], 42);
  EXPECT_EQ(object_json["x"], 10.0f);
  EXPECT_EQ(object_json["y"], 20.0f);
  EXPECT_EQ(object_json["width"], 30.0f);
  EXPECT_EQ(object_json["height"], 40.0f);
  EXPECT_EQ(object_json["type"], "PointTag");
  EXPECT_EQ(object_json["visible"], true);
  EXPECT_EQ(object_json["rotation"], 0);
  EXPECT_FALSE(object_json.contains("ellipse"));
  EXPECT_TRUE(object_json.contains("point"));
  EXPECT_FALSE(object_json.contains("properties"));
}

}  // namespace tactile::test
