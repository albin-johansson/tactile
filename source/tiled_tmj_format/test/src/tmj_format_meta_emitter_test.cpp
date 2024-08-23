// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_meta_emitter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/meta/attribute.hpp"
#include "tactile/test_util/document_view_mocks.hpp"

namespace tactile::test {

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitContextWithoutProperties)
{
  MetaViewMock meta {};

  EXPECT_CALL(meta, get_name).WillOnce(testing::Return("META"));
  EXPECT_CALL(meta, get_property).Times(0);
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(0));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "META");
  EXPECT_FALSE(meta_json.contains("properties"));
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitStringProperty)
{
  MetaViewMock meta {};

  const String property_name {"s"};
  const Attribute property_value {"foo"};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "string");
  EXPECT_EQ(property_json["value"], property_value.as_string());
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitIntProperty)
{
  MetaViewMock meta {};

  const String property_name {"i"};
  const Attribute property_value {42};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "int");
  EXPECT_EQ(property_json["value"], property_value.as_int());
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitInt2Property)
{
  MetaViewMock meta {};

  const String property_name {"i2"};
  const Attribute property_value {Int2 {1, 2}};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "string");
  EXPECT_EQ(property_json["value"], "1;2");
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitInt3Property)
{
  MetaViewMock meta {};

  const String property_name {"i3"};
  const Attribute property_value {Int3 {1, 2, 3}};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "string");
  EXPECT_EQ(property_json["value"], "1;2;3");
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitInt4Property)
{
  MetaViewMock meta {};

  const String property_name {"i4"};
  const Attribute property_value {Int4 {1, 2, 3, 4}};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "string");
  EXPECT_EQ(property_json["value"], "1;2;3;4");
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitFloatProperty)
{
  MetaViewMock meta {};

  const String property_name {"f"};
  const Attribute property_value {1.5f};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "float");
  EXPECT_EQ(property_json["value"], property_value.as_float());
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitFloat2Property)
{
  MetaViewMock meta {};

  const String property_name {"f2"};
  const Attribute property_value {Float2 {1, 2}};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "string");
  EXPECT_EQ(property_json["value"], "1;2");
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitFloat3Property)
{
  MetaViewMock meta {};

  const String property_name {"f3"};
  const Attribute property_value {Float3 {1, 2, 3}};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "string");
  EXPECT_EQ(property_json["value"], "1;2;3");
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitFloat4Property)
{
  MetaViewMock meta {};

  const String property_name {"f4"};
  const Attribute property_value {Float4 {1, 2, 3, 4}};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "string");
  EXPECT_EQ(property_json["value"], "1;2;3;4");
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitBoolProperty)
{
  MetaViewMock meta {};

  const String property_name {"b"};
  const Attribute property_value {true};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "bool");
  EXPECT_EQ(property_json["value"], property_value.as_bool());
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitColorProperty)
{
  MetaViewMock meta {};

  const String property_name {"c"};
  const Attribute property_value {UColor {0x1A, 0x3B, 0x5C, 0x7D}};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "color");
  EXPECT_EQ(property_json["value"], "#7D1A3B5C");
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitPathProperty)
{
  MetaViewMock meta {};

  const String property_name {"p"};
  const Attribute property_value {Path {"a/b/c"}};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "file");
  EXPECT_EQ(property_json["value"], property_value.as_path().string());
}

// tactile::emit_tiled_tmj_metadata
TEST(TmjFormatMetaEmitter, EmitObjectProperty)
{
  MetaViewMock meta {};

  const String property_name {"o"};
  const Attribute property_value {ObjectRef {99}};
  const std::pair<const String&, const Attribute&> property {property_name, property_value};

  EXPECT_CALL(meta, get_name);
  EXPECT_CALL(meta, get_property).WillOnce(testing::Return(property));
  EXPECT_CALL(meta, property_count).WillOnce(testing::Return(1));

  nlohmann::json meta_json {};
  emit_tiled_tmj_metadata(meta, meta_json);

  EXPECT_EQ(meta_json["name"], "");
  ASSERT_EQ(meta_json["properties"].size(), 1);

  const auto& property_json = meta_json["properties"].front();
  EXPECT_EQ(property_json["name"], property_name);
  EXPECT_EQ(property_json["type"], "object");
  EXPECT_EQ(property_json["value"], property_value.as_object().value);
}

}  // namespace tactile::test
