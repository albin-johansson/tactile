#include <gtest/gtest.h>

#include "open_map_document.hpp"
#include "tile_layer.hpp"

using namespace tactile;

TEST(ImportJsonMap, Embedded)
{
  const auto path = QStringLiteral(u"json/embedded.json");

  QObject object;
  ParseError error;

  auto* document = OpenMapDocument(path, error);
  ASSERT_TRUE(document);

  document->setParent(&object);  // Avoid memory leak

  {  // Layers
    constexpr auto rows = 5_row;
    constexpr auto cols = 7_col;

    ASSERT_EQ(2, document->LayerCount());
    ASSERT_EQ(rows, document->RowCount());
    ASSERT_EQ(cols, document->ColumnCount());

    {  // First layer
      constexpr auto id = 1_layer;
      ASSERT_TRUE(document->HasLayer(id));

      const auto* layer = document->GetLayer(id);
      ASSERT_EQ(core::LayerType::TileLayer, layer->Type());
      EXPECT_TRUE(layer->IsVisible());
      EXPECT_EQ(1.0, layer->Opacity());

      const auto* tileLayer = document->GetTileLayer(id);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(rows, tileLayer->RowCount());
      EXPECT_EQ(cols, tileLayer->ColumnCount());
      EXPECT_EQ(360_t, tileLayer->TileAt({0_row, 0_col}));
      EXPECT_EQ(360_t, tileLayer->TileAt({3_row, 0_col}));
      EXPECT_EQ(354_t, tileLayer->TileAt({0_row, 6_col}));
      EXPECT_EQ(354_t, tileLayer->TileAt({3_row, 6_col}));
    }

    {  // Second layer
      constexpr auto id = 2_layer;
      ASSERT_TRUE(document->HasLayer(id));

      const auto* layer = document->GetLayer(id);
      ASSERT_EQ(core::LayerType::TileLayer, layer->Type());
      EXPECT_TRUE(layer->IsVisible());
      EXPECT_EQ(0.8, layer->Opacity());

      const auto* tileLayer = document->GetTileLayer(id);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(empty, tileLayer->TileAt({0_row, 0_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({3_row, 0_col}));
      EXPECT_EQ(297_t, tileLayer->TileAt({0_row, 6_col}));
      EXPECT_EQ(328_t, tileLayer->TileAt({3_row, 4_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({3_row, 6_col}));
    }
  }

  {  // Tilesets
    const auto* tilesets = document->GetTilesets();
    ASSERT_TRUE(tilesets);
    ASSERT_EQ(1, tilesets->Count());

    constexpr auto id = 1_ts;
    ASSERT_TRUE(tilesets->Contains(id));
    EXPECT_EQ(id, tilesets->CurrentTilesetId());

    const auto& tileset = tilesets->At(id);
    EXPECT_EQ(1_t, tileset.FirstId());
    EXPECT_EQ(QStringLiteral(u"terrain"), tileset.Name());
    EXPECT_EQ(32_tw, tileset.GetTileWidth());
    EXPECT_EQ(32_th, tileset.GetTileHeight());
    EXPECT_EQ(1'024, tileset.TileCount());
    EXPECT_EQ(1'024, tileset.Width());
    EXPECT_EQ(1'024, tileset.Height());
    EXPECT_EQ(32_col, tileset.ColumnCount());

    const QFileInfo file{QStringLiteral(u"terrain.png")};
    EXPECT_EQ(file.absoluteFilePath(), tileset.File().absoluteFilePath());
  }
}

// TODO move this to map_import_test.cpp
TEST(ImportJsonMap, External)
{
  const auto path = QStringLiteral(u"json/external.json");

  QObject object;
  ParseError error;

  auto* document = OpenMapDocument(path, error);
  ASSERT_TRUE(document);

  document->setParent(&object);  // Avoid memory leak

  constexpr auto rows = 6_row;
  constexpr auto cols = 8_col;

  {  // Layers
    ASSERT_EQ(2, document->LayerCount());
    ASSERT_EQ(rows, document->RowCount());
    ASSERT_EQ(cols, document->ColumnCount());

    {  // First layer
      constexpr auto id = 1_layer;
      ASSERT_TRUE(document->HasLayer(id));

      const auto* layer = document->GetLayer(id);
      ASSERT_EQ(core::LayerType::TileLayer, layer->Type());
      EXPECT_TRUE(layer->IsVisible());
      EXPECT_EQ(0.2, layer->Opacity());

      const auto* tileLayer = document->GetTileLayer(id);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(rows, tileLayer->RowCount());
      EXPECT_EQ(cols, tileLayer->ColumnCount());
      EXPECT_EQ(107_t, tileLayer->TileAt({0_row, 0_col}));
      EXPECT_EQ(107_t, tileLayer->TileAt({5_row, 0_col}));
      EXPECT_EQ(372_t, tileLayer->TileAt({0_row, 7_col}));
      EXPECT_EQ(372_t, tileLayer->TileAt({5_row, 7_col}));
    }

    {  // Second layer
      constexpr auto id = 2_layer;
      ASSERT_TRUE(document->HasLayer(id));

      const auto* layer = document->GetLayer(id);
      ASSERT_EQ(core::LayerType::TileLayer, layer->Type());
      EXPECT_TRUE(layer->IsVisible());
      EXPECT_EQ(1.0, layer->Opacity());

      const auto* tileLayer = document->GetTileLayer(id);
      ASSERT_TRUE(tileLayer);
      EXPECT_EQ(1'293_t, tileLayer->TileAt({0_row, 0_col}));
      EXPECT_EQ(1'359_t, tileLayer->TileAt({2_row, 2_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({0_row, 7_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({5_row, 0_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({5_row, 7_col}));
    }
  }

  {  // Tilesets
    const auto* tilesets = document->GetTilesets();
    ASSERT_TRUE(tilesets);
    ASSERT_EQ(2, tilesets->Count());
    ASSERT_TRUE(tilesets->HasActiveTileset());
    ASSERT_EQ(2_ts, tilesets->CurrentTilesetId());

    {  // First tileset
      constexpr auto id = 1_ts;
      ASSERT_TRUE(tilesets->Contains(id));

      const auto& tileset = tilesets->At(id);
      EXPECT_EQ(1_t, tileset.FirstId());
      EXPECT_EQ(QStringLiteral(u"terrain"), tileset.Name());
      EXPECT_EQ(32_tw, tileset.GetTileWidth());
      EXPECT_EQ(32_th, tileset.GetTileHeight());
      EXPECT_EQ(1'024, tileset.TileCount());
      EXPECT_EQ(1'024, tileset.Width());
      EXPECT_EQ(1'024, tileset.Height());
      EXPECT_EQ(32_col, tileset.ColumnCount());

      const QFileInfo file{QStringLiteral(u"terrain.png")};
      EXPECT_EQ(file.absoluteFilePath(), tileset.File().absoluteFilePath());
    }

    {  // Second tileset
      constexpr auto id = 2_ts;
      ASSERT_TRUE(tilesets->Contains(id));

      const auto& tileset = tilesets->At(id);
      EXPECT_EQ(1025_t, tileset.FirstId());
      EXPECT_EQ(QStringLiteral(u"outside"), tileset.Name());
      EXPECT_EQ(32_tw, tileset.GetTileWidth());
      EXPECT_EQ(32_th, tileset.GetTileHeight());
      EXPECT_EQ(1'024, tileset.TileCount());
      EXPECT_EQ(1'024, tileset.Width());
      EXPECT_EQ(1'024, tileset.Height());
      EXPECT_EQ(32_col, tileset.ColumnCount());

      const QFileInfo file{QStringLiteral(u"outside.png")};
      EXPECT_EQ(file.absoluteFilePath(), tileset.File().absoluteFilePath());
    }
  }
}
