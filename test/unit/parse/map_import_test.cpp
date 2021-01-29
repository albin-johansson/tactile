#include <gtest/gtest.h>

/**
 * These test have been designed around the fact that there are two maps,
 * "embedded" and "external", each available in both XML and JSON
 * formats, where the idea is that the contents XML and JSON versions of each
 * map are identical. This means that we can reuse the code that validates the
 * state of the parsed document.
 */

#include "gtest_macros.hpp"
#include "json_engine.hpp"
#include "map_parser.hpp"
#include "object_layer.hpp"
#include "open_map.hpp"
#include "tactile_qstring.hpp"
#include "xml_engine.hpp"

using namespace tactile;

static_assert(parse::is_object<parse::xml_element>);
static_assert(parse::is_object<parse::json_element>);

// clang-format off
static_assert(parse::is_engine<parse::xml_engine, QDomDocument, parse::xml_element>);
static_assert(parse::is_engine<parse::json_engine, QJsonDocument, parse::json_element>);
// clang-format on

namespace {

inline constexpr auto rows = 10_row;
inline constexpr auto cols = 15_col;
inline constexpr auto tileWidth = 32_tw;
inline constexpr auto tileHeight = 32_th;

void validate_layers(const core::map_document& document)
{
  ASSERT_EQ(3, document.layer_count());

  ASSERT_TRUE(document.has_layer(1_layer));

  const auto ground = TACTILE_QSTRING(u"ground");

  {  // First layer
    const auto* layer = document.get_layer(1_layer);
    ASSERT_TRUE(layer);
    ASSERT_EQ(core::layer_type::tile_layer, layer->type());

    EXPECT_EQ(TACTILE_QSTRING(u"G0"), layer->name());
    EXPECT_EQ(1.0, layer->opacity());
    EXPECT_TRUE(layer->visible());

    const auto* tileLayer = document.get_tile_layer(1_layer);
    ASSERT_TRUE(tileLayer);

    EXPECT_EQ(rows, tileLayer->row_count());
    EXPECT_EQ(cols, tileLayer->col_count());

    ASSERT_TRUE(layer->has_property(ground));
    EXPECT_TRUE(layer->get_property(ground).as_boolean());
  }

  {  // Second layer
    const auto* layer = document.get_layer(2_layer);
    ASSERT_TRUE(layer);
    ASSERT_EQ(core::layer_type::tile_layer, layer->type());

    EXPECT_EQ(TACTILE_QSTRING(u"T0"), layer->name());
    EXPECT_EQ(0.75, layer->opacity());
    EXPECT_TRUE(layer->visible());

    const auto* tileLayer = document.get_tile_layer(2_layer);
    ASSERT_TRUE(tileLayer);

    EXPECT_EQ(rows, tileLayer->row_count());
    EXPECT_EQ(cols, tileLayer->col_count());

    ASSERT_TRUE(layer->has_property(ground));
    EXPECT_FALSE(layer->get_property(ground).as_boolean());
  }

  {  // Third layer
    const auto* layer = document.get_layer(3_layer);
    ASSERT_TRUE(layer);
    ASSERT_EQ(core::layer_type::object_layer, layer->type());

    EXPECT_EQ(TACTILE_QSTRING(u"O1"), layer->name());
    EXPECT_EQ(1.0, layer->opacity());
    EXPECT_TRUE(layer->visible());

    const auto* objectLayer = document.get_object_layer(3_layer);
    ASSERT_TRUE(objectLayer);

    ASSERT_EQ(1, objectLayer->object_count());
    ASSERT_TRUE(objectLayer->has_property(ground));
    EXPECT_FALSE(objectLayer->get_property(ground).as_boolean());

    ASSERT_TRUE(objectLayer->has_object(1_obj));
    const auto& object = objectLayer->get_object(1_obj);

    EXPECT_EQ(128, object.x());
    EXPECT_EQ(96, object.y());
    EXPECT_EQ(0, object.width());
    EXPECT_EQ(0, object.height());
    EXPECT_EQ(core::object_type::point, object.type());
    EXPECT_TRUE(object.visible());
    EXPECT_TRUE(object.is_point());
    EXPECT_FALSE(object.custom_type());
    EXPECT_EQ(TACTILE_QSTRING(u"Point"), object.name());

    ASSERT_EQ(1, object.property_count());
    ASSERT_TRUE(object.has_property(TACTILE_QSTRING(u"foo")));
    EXPECT_EQ(TACTILE_QSTRING(u"bar"),
              object.get_property(TACTILE_QSTRING(u"foo")).as_string());
  }
}

void validate_tilesets(const core::map_document& document)
{
  const auto* tilesets = document.tilesets();
  ASSERT_TRUE(tilesets);
  ASSERT_EQ(2, tilesets->count());

  ASSERT_TRUE(tilesets->contains(1_ts));
  const auto& terrain = tilesets->at(1_ts);

  const QFileInfo terrainPath{TACTILE_QSTRING(u"terrain.png")};
  EXPECT_EQ(terrainPath.absoluteFilePath(), terrain.file().canonicalFilePath());

  ASSERT_EQ(TACTILE_QSTRING(u"terrain"), terrain.name());
  EXPECT_EQ(1_t, terrain.first_id());
  EXPECT_EQ(32_col, terrain.col_count());
  EXPECT_EQ(1'024, terrain.tile_count());
  EXPECT_EQ(1'024, terrain.image().width());
  EXPECT_EQ(1'024, terrain.image().height());
  EXPECT_EQ(32_tw, terrain.get_tile_width());
  EXPECT_EQ(32_th, terrain.get_tile_height());

  ASSERT_TRUE(tilesets->contains(2_ts));
  const auto& outside = tilesets->at(2_ts);

  const QFileInfo outsidePath{TACTILE_QSTRING(u"outside.png")};
  EXPECT_EQ(outsidePath.absoluteFilePath(), outside.file().canonicalFilePath());

  ASSERT_EQ(TACTILE_QSTRING(u"outside"), outside.name());
  EXPECT_EQ(1'025_t, outside.first_id());
  EXPECT_EQ(32_col, outside.col_count());
  EXPECT_EQ(1'024, outside.tile_count());
  EXPECT_EQ(1'024, outside.image().width());
  EXPECT_EQ(1'024, outside.image().height());
  EXPECT_EQ(32_tw, outside.get_tile_width());
  EXPECT_EQ(32_th, outside.get_tile_height());
}

void validate_properties(const core::map_document& document)
{
  ASSERT_EQ(7, document.property_count());
  const auto string = TACTILE_QSTRING(u"str");
  const auto color = TACTILE_QSTRING(u"color");
  const auto integer = TACTILE_QSTRING(u"i");
  const auto floating = TACTILE_QSTRING(u"f");
  const auto boolean = TACTILE_QSTRING(u"isCool");
  const auto object = TACTILE_QSTRING(u"obj");
  const auto file = TACTILE_QSTRING(u"path");

  ASSERT_TRUE(document.has_property(string));
  EXPECT_EQ(TACTILE_QSTRING(u"foobar"),
            document.get_property(string).as_string());

  ASSERT_TRUE(document.has_property(color));
  EXPECT_EQ(QColor{TACTILE_QSTRING(u"#ff550000")},
            document.get_property(color).as_color());

  ASSERT_TRUE(document.has_property(integer));
  EXPECT_EQ(123, document.get_property(integer).as_integer());

  ASSERT_TRUE(document.has_property(floating));
  EXPECT_EQ(12.3, document.get_property(floating).as_floating());

  ASSERT_TRUE(document.has_property(boolean));
  EXPECT_TRUE(document.get_property(boolean).as_boolean());

  ASSERT_TRUE(document.has_property(object));
  EXPECT_EQ(core::object_ref{1}, document.get_property(object).as_object());

  const QFileInfo path{TACTILE_QSTRING(u"../terrain.png")};
  ASSERT_TRUE(document.has_property(file));
  EXPECT_EQ(path.absoluteFilePath(),
            document.get_property(file).as_file().absoluteFilePath());
}

}  // namespace

TACTILE_DEFINE_TEST_P(MapImportTest, QString)
{
  QObject parent;

  parse::parse_error error;
  auto* document = service::open_map(GetParam(), &error);

  ASSERT_TRUE(document);
  document->setParent(&parent);  // Avoid memory leak

  EXPECT_EQ(rows, document->row_count());
  EXPECT_EQ(cols, document->col_count());

  validate_layers(*document);
  validate_tilesets(*document);
  validate_properties(*document);
}

inline const auto embeddedValues =
    testing::Values(TACTILE_QSTRING(u"maps/embedded.tmx"),
                    TACTILE_QSTRING(u"maps/embedded.json"));

TACTILE_REGISTER_TEST_P(MapImportTest, embeddedValues);

// inline const auto externalValues =
//    testing::Values(TACTILE_QSTRING(u"maps/external.tmx"),
//                    TACTILE_QSTRING(u"maps/external.json"));

// TACTILE_REGISTER_TEST_P(MapImportExternalTest, externalValues);
