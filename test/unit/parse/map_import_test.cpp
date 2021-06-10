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
#include "open_map_document.hpp"
#include "tactile_qstring.hpp"
#include "xml_engine.hpp"

using namespace tactile;

namespace {

inline constexpr auto rows = 10_row;
inline constexpr auto cols = 15_col;
inline constexpr auto tileWidth = 32_tw;
inline constexpr auto tileHeight = 32_th;

void validate_layers(const core::MapDocument& document)
{
  ASSERT_EQ(3, document.LayerCount());

  ASSERT_TRUE(document.HasLayer(1_layer));

  const auto ground = TACTILE_QSTRING(u"ground");

  {  // First layer
    const auto* layer = document.GetLayer(1_layer);
    ASSERT_TRUE(layer);
    ASSERT_EQ(core::LayerType::tile_layer, layer->Type());

    EXPECT_EQ(TACTILE_QSTRING(u"G0"), layer->Name());
    EXPECT_EQ(1.0, layer->Opacity());
    EXPECT_TRUE(layer->IsVisible());

    const auto* tileLayer = document.GetTileLayer(1_layer);
    ASSERT_TRUE(tileLayer);

    EXPECT_EQ(rows, tileLayer->RowCount());
    EXPECT_EQ(cols, tileLayer->ColumnCount());

    ASSERT_TRUE(layer->HasProperty(ground));
    EXPECT_TRUE(layer->GetProperty(ground).AsBoolean());
  }

  {  // Second layer
    const auto* layer = document.GetLayer(2_layer);
    ASSERT_TRUE(layer);
    ASSERT_EQ(core::LayerType::tile_layer, layer->Type());

    EXPECT_EQ(TACTILE_QSTRING(u"T0"), layer->Name());
    EXPECT_EQ(0.75, layer->Opacity());
    EXPECT_TRUE(layer->IsVisible());

    const auto* tileLayer = document.GetTileLayer(2_layer);
    ASSERT_TRUE(tileLayer);

    EXPECT_EQ(rows, tileLayer->RowCount());
    EXPECT_EQ(cols, tileLayer->ColumnCount());

    ASSERT_TRUE(layer->HasProperty(ground));
    EXPECT_FALSE(layer->GetProperty(ground).AsBoolean());
  }

  {  // Third layer
    const auto* layer = document.GetLayer(3_layer);
    ASSERT_TRUE(layer);
    ASSERT_EQ(core::LayerType::object_layer, layer->Type());

    EXPECT_EQ(TACTILE_QSTRING(u"O1"), layer->Name());
    EXPECT_EQ(1.0, layer->Opacity());
    EXPECT_TRUE(layer->IsVisible());

    const auto* objectLayer = document.GetObjectLayer(3_layer);
    ASSERT_TRUE(objectLayer);

    ASSERT_EQ(1, objectLayer->ObjectCount());
    ASSERT_TRUE(objectLayer->HasProperty(ground));
    EXPECT_FALSE(objectLayer->GetProperty(ground).AsBoolean());

    ASSERT_TRUE(objectLayer->HasObject(1_obj));
    const auto& object = objectLayer->GetObject(1_obj);

    EXPECT_EQ(128, object.X());
    EXPECT_EQ(96, object.Y());
    EXPECT_EQ(0, object.Width());
    EXPECT_EQ(0, object.Height());
    EXPECT_EQ(core::object_type::point, object.Type());
    EXPECT_TRUE(object.IsVisible());
    EXPECT_TRUE(object.IsPoint());
    EXPECT_FALSE(object.CustomType());
    EXPECT_EQ(TACTILE_QSTRING(u"Point"), object.Name());

    ASSERT_EQ(1, object.PropertyCount());
    ASSERT_TRUE(object.HasProperty(TACTILE_QSTRING(u"foo")));
    EXPECT_EQ(TACTILE_QSTRING(u"bar"),
              object.GetProperty(TACTILE_QSTRING(u"foo")).AsString());
  }
}

void validate_tilesets(const core::MapDocument& document)
{
  const auto* tilesets = document.GetTilesets();
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

void validate_properties(const core::MapDocument& document)
{
  ASSERT_EQ(7, document.PropertyCount());
  const auto string = TACTILE_QSTRING(u"str");
  const auto color = TACTILE_QSTRING(u"color");
  const auto integer = TACTILE_QSTRING(u"i");
  const auto floating = TACTILE_QSTRING(u"f");
  const auto boolean = TACTILE_QSTRING(u"isCool");
  const auto object = TACTILE_QSTRING(u"obj");
  const auto file = TACTILE_QSTRING(u"path");

  ASSERT_TRUE(document.HasProperty(string));
  EXPECT_EQ(TACTILE_QSTRING(u"foobar"),
            document.GetProperty(string).AsString());

  ASSERT_TRUE(document.HasProperty(color));
  EXPECT_EQ(QColor{TACTILE_QSTRING(u"#ff550000")},
            document.GetProperty(color).AsColor());

  ASSERT_TRUE(document.HasProperty(integer));
  EXPECT_EQ(123, document.GetProperty(integer).AsInteger());

  ASSERT_TRUE(document.HasProperty(floating));
  EXPECT_EQ(12.3, document.GetProperty(floating).AsFloating());

  ASSERT_TRUE(document.HasProperty(boolean));
  EXPECT_TRUE(document.GetProperty(boolean).AsBoolean());

  ASSERT_TRUE(document.HasProperty(object));
  EXPECT_EQ(core::object_ref{1}, document.GetProperty(object).AsObject());

  const QFileInfo path{TACTILE_QSTRING(u"../terrain.png")};
  ASSERT_TRUE(document.HasProperty(file));
  EXPECT_EQ(path.absoluteFilePath(),
            document.GetProperty(file).AsFile().absoluteFilePath());
}

}  // namespace

TACTILE_DEFINE_TEST_P(MapImportTest, QString)
{
  QObject parent;

  parse::parse_error error;
  auto* document = open_map_document(GetParam(), error);

  ASSERT_TRUE(document);
  document->setParent(&parent);  // Avoid memory leak

  EXPECT_EQ(rows, document->RowCount());
  EXPECT_EQ(cols, document->ColumnCount());

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
