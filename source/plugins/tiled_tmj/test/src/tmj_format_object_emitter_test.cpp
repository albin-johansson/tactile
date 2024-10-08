// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_object_emitter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/meta/attribute.hpp"
#include "tactile/test_util/document_view_mocks.hpp"

namespace tactile::test {

// tactile::emit_tiled_tmj_object
TEST(TmjFormatObjectEmitter, EmitRectangleObject)
{
  const ir::Object object {
    .meta = ir::Metadata {},
    .id = ObjectID {42},
    .type = ObjectType::kRect,
    .position = Float2 {1.0f, 2.0f},
    .size = Float2 {3.0f, 4.0f},
    .tag = "RectangleTag",
    .visible = false,
  };

  const ObjectViewMock object_view {object};
  EXPECT_CALL(object_view, get_parent_layer).Times(0);
  EXPECT_CALL(object_view, get_parent_tile).Times(0);
  EXPECT_CALL(object_view, get_type);
  EXPECT_CALL(object_view, get_id);
  EXPECT_CALL(object_view, get_position);
  EXPECT_CALL(object_view, get_size);
  EXPECT_CALL(object_view, get_tag);
  EXPECT_CALL(object_view, is_visible);
  EXPECT_CALL(object_view, get_meta);

  const auto object_json = emit_tiled_tmj_object(object_view);

  EXPECT_EQ(object_json["id"], object.id);
  EXPECT_EQ(object_json["x"], object.position.x());
  EXPECT_EQ(object_json["y"], object.position.y());
  EXPECT_EQ(object_json["width"], object.size.x());
  EXPECT_EQ(object_json["height"], object.size.y());
  EXPECT_EQ(object_json["type"], object.tag);
  EXPECT_EQ(object_json["visible"], object.visible);
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

  const ir::Object object {
    .meta = ir::Metadata {},
    .id = ObjectID {42},
    .type = ObjectType::kEllipse,
    .position = Float2 {1.0f, 2.0f},
    .size = Float2 {3.0f, 4.0f},
    .tag = "EllipseTag",
    .visible = true,
  };

  const std::string prop1_name {"P1"};
  const std::string prop2_name {"P2"};
  const Attribute prop1_value {42};
  const Attribute prop2_value {"demo"};

  ObjectViewMock object_view {object};
  EXPECT_CALL(object_view, get_parent_layer).Times(0);
  EXPECT_CALL(object_view, get_parent_tile).Times(0);
  EXPECT_CALL(object_view, get_type);
  EXPECT_CALL(object_view, get_id);
  EXPECT_CALL(object_view, get_position);
  EXPECT_CALL(object_view, get_size);
  EXPECT_CALL(object_view, get_tag);
  EXPECT_CALL(object_view, is_visible);
  EXPECT_CALL(object_view, get_meta);

  auto& meta_view = object_view.get_meta_view_mock();
  EXPECT_CALL(meta_view, property_count).WillOnce(testing::Return(2));
  EXPECT_CALL(meta_view, get_property)
      .WillOnce(testing::Return(std::make_pair(prop1_name, prop1_value)))
      .WillOnce(testing::Return(std::make_pair(prop2_name, prop2_value)));

  const auto object_json = emit_tiled_tmj_object(object_view);

  EXPECT_EQ(object_json["id"], object.id);
  EXPECT_EQ(object_json["x"], object.position.x());
  EXPECT_EQ(object_json["y"], object.position.y());
  EXPECT_EQ(object_json["width"], object.size.x());
  EXPECT_EQ(object_json["height"], object.size.y());
  EXPECT_EQ(object_json["type"], object.tag);
  EXPECT_EQ(object_json["visible"], object.visible);
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
  const ir::Object object {
    .meta = ir::Metadata {},
    .id = ObjectID {42},
    .type = ObjectType::kPoint,
    .position = Float2 {10.0f, 20.0f},
    .size = Float2 {30.0f, 40.0f},
    .tag = "PointTag",
    .visible = true,
  };

  const ObjectViewMock object_view {object};
  EXPECT_CALL(object_view, get_parent_layer).Times(0);
  EXPECT_CALL(object_view, get_parent_tile).Times(0);
  EXPECT_CALL(object_view, get_type);
  EXPECT_CALL(object_view, get_id);
  EXPECT_CALL(object_view, get_position);
  EXPECT_CALL(object_view, get_size);
  EXPECT_CALL(object_view, get_tag);
  EXPECT_CALL(object_view, is_visible);
  EXPECT_CALL(object_view, get_meta);

  const auto object_json = emit_tiled_tmj_object(object_view);

  EXPECT_EQ(object_json["id"], object.id);
  EXPECT_EQ(object_json["x"], object.position.x());
  EXPECT_EQ(object_json["y"], object.position.y());
  EXPECT_EQ(object_json["width"], object.size.x());
  EXPECT_EQ(object_json["height"], object.size.y());
  EXPECT_EQ(object_json["type"], object.tag);
  EXPECT_EQ(object_json["visible"], object.visible);
  EXPECT_EQ(object_json["rotation"], 0);

  EXPECT_FALSE(object_json.contains("ellipse"));
  EXPECT_TRUE(object_json.contains("point"));
  EXPECT_FALSE(object_json.contains("properties"));
}

}  // namespace tactile::test
