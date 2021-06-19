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
#include "xml_engine.hpp"

using namespace tactile;

namespace {

inline constexpr auto rows = 10_row;
inline constexpr auto cols = 15_col;
inline constexpr auto tileWidth = 32_tw;
inline constexpr auto tileHeight = 32_th;

void ValidateLayers(const core::MapDocument& document)
{
  ASSERT_EQ(3, document.LayerCount());

  ASSERT_TRUE(document.HasLayer(1_layer));

  const auto ground = QStringLiteral(u"ground");

  {  // First layer
    const auto* layer = document.GetLayer(1_layer);
    ASSERT_TRUE(layer);
    ASSERT_EQ(core::LayerType::TileLayer, layer->GetType());

    EXPECT_EQ(QStringLiteral(u"G0"), layer->GetName());
    EXPECT_EQ(1.0, layer->GetOpacity());
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
    ASSERT_EQ(core::LayerType::TileLayer, layer->GetType());

    EXPECT_EQ(QStringLiteral(u"T0"), layer->GetName());
    EXPECT_EQ(0.75, layer->GetOpacity());
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
    ASSERT_EQ(core::LayerType::ObjectLayer, layer->GetType());

    EXPECT_EQ(QStringLiteral(u"O1"), layer->GetName());
    EXPECT_EQ(1.0, layer->GetOpacity());
    EXPECT_TRUE(layer->IsVisible());

    const auto* objectLayer = document.GetObjectLayer(3_layer);
    ASSERT_TRUE(objectLayer);

    ASSERT_EQ(1, objectLayer->ObjectCount());
    ASSERT_TRUE(objectLayer->HasProperty(ground));
    EXPECT_FALSE(objectLayer->GetProperty(ground).AsBoolean());

    ASSERT_TRUE(objectLayer->HasObject(1_obj));
    const auto& object = objectLayer->GetObject(1_obj);

    EXPECT_EQ(128, object.GetX());
    EXPECT_EQ(96, object.GetY());
    EXPECT_EQ(0, object.GetWidth());
    EXPECT_EQ(0, object.GetHeight());
    EXPECT_EQ(core::ObjectType::Point, object.GetType());
    EXPECT_TRUE(object.IsVisible());
    EXPECT_TRUE(object.IsPoint());
    EXPECT_FALSE(object.GetCustomType());
    EXPECT_EQ(QStringLiteral(u"Point"), object.GetName());

    ASSERT_EQ(1, object.PropertyCount());
    ASSERT_TRUE(object.HasProperty(QStringLiteral(u"foo")));
    EXPECT_EQ(QStringLiteral(u"bar"),
              object.GetProperty(QStringLiteral(u"foo")).AsString());
  }
}

void ValidateTilesets(const core::MapDocument& document)
{
  const auto* tilesets = document.GetTilesets();
  ASSERT_TRUE(tilesets);
  ASSERT_EQ(2, tilesets->Count());

  ASSERT_TRUE(tilesets->Contains(1_ts));
  const auto& terrain = tilesets->At(1_ts);

  const QFileInfo terrainPath{QStringLiteral(u"terrain.png")};
  EXPECT_EQ(terrainPath.absoluteFilePath(), terrain.File().canonicalFilePath());

  ASSERT_EQ(QStringLiteral(u"terrain"), terrain.Name());
  EXPECT_EQ(1_t, terrain.FirstId());
  EXPECT_EQ(32_col, terrain.ColumnCount());
  EXPECT_EQ(1'024, terrain.TileCount());
  EXPECT_EQ(1'024, terrain.Image().width());
  EXPECT_EQ(1'024, terrain.Image().height());
  EXPECT_EQ(32_tw, terrain.GetTileWidth());
  EXPECT_EQ(32_th, terrain.GetTileHeight());

  ASSERT_TRUE(tilesets->Contains(2_ts));
  const auto& outside = tilesets->At(2_ts);

  const QFileInfo outsidePath{QStringLiteral(u"outside.png")};
  EXPECT_EQ(outsidePath.absoluteFilePath(), outside.File().canonicalFilePath());

  ASSERT_EQ(QStringLiteral(u"outside"), outside.Name());
  EXPECT_EQ(1'025_t, outside.FirstId());
  EXPECT_EQ(32_col, outside.ColumnCount());
  EXPECT_EQ(1'024, outside.TileCount());
  EXPECT_EQ(1'024, outside.Image().width());
  EXPECT_EQ(1'024, outside.Image().height());
  EXPECT_EQ(32_tw, outside.GetTileWidth());
  EXPECT_EQ(32_th, outside.GetTileHeight());
}

void ValidateProperties(const core::MapDocument& document)
{
  ASSERT_EQ(7, document.PropertyCount());
  const auto string = QStringLiteral(u"str");
  const auto color = QStringLiteral(u"color");
  const auto integer = QStringLiteral(u"i");
  const auto floating = QStringLiteral(u"f");
  const auto boolean = QStringLiteral(u"isCool");
  const auto object = QStringLiteral(u"obj");
  const auto file = QStringLiteral(u"path");

  ASSERT_TRUE(document.HasProperty(string));
  EXPECT_EQ(QStringLiteral(u"foobar"), document.GetProperty(string).AsString());

  ASSERT_TRUE(document.HasProperty(color));
  EXPECT_EQ(QColor{QStringLiteral(u"#ff550000")},
            document.GetProperty(color).AsColor());

  ASSERT_TRUE(document.HasProperty(integer));
  EXPECT_EQ(123, document.GetProperty(integer).AsInteger());

  ASSERT_TRUE(document.HasProperty(floating));
  EXPECT_EQ(12.3, document.GetProperty(floating).AsFloating());

  ASSERT_TRUE(document.HasProperty(boolean));
  EXPECT_TRUE(document.GetProperty(boolean).AsBoolean());

  ASSERT_TRUE(document.HasProperty(object));
  EXPECT_EQ(core::object_ref{1}, document.GetProperty(object).AsObject());

  const QFileInfo path{QStringLiteral(u"../terrain.png")};
  ASSERT_TRUE(document.HasProperty(file));
  EXPECT_EQ(path.absoluteFilePath(),
            document.GetProperty(file).AsFile().absoluteFilePath());
}

}  // namespace

TACTILE_DEFINE_TEST_P(MapImportTest, QString)
{
  QObject parent;

  ParseError error;
  auto* document = OpenMapDocument(GetParam(), error);

  ASSERT_TRUE(document);
  document->setParent(&parent);  // Avoid memory leak

  EXPECT_EQ(rows, document->RowCount());
  EXPECT_EQ(cols, document->ColumnCount());

  ValidateLayers(*document);
  ValidateTilesets(*document);
  ValidateProperties(*document);
}

inline const auto embeddedValues =
    testing::Values(QStringLiteral(u"maps/embedded.tmx"),
                    QStringLiteral(u"maps/embedded.json"));

TACTILE_REGISTER_TEST_P(MapImportTest, embeddedValues);

// inline const auto externalValues =
//    testing::Values(QStringLiteral(u"maps/external.tmx"),
//                    QStringLiteral(u"maps/external.json"));

// TACTILE_REGISTER_TEST_P(MapImportExternalTest, externalValues);
