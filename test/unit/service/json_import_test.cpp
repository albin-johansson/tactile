#include <gtest/gtest.h>

#include "open_map.hpp"
#include "tactile_qstring.hpp"

using namespace tactile;

TEST(ImportJsonMap, Embedded)
{
  const auto path = TACTILE_QSTRING(u"json/embedded.json");

  QObject object;
  tmx::parse_error error;

  auto* document = service::open_map(path, &error);
  ASSERT_TRUE(document);

  document->setParent(&object);  // Avoid memory leak

  {  // Layers
    ASSERT_EQ(2, document->layer_count());
    ASSERT_EQ(5_row, document->row_count());
    ASSERT_EQ(7_col, document->col_count());

    {  // First layer
      ASSERT_TRUE(document->has_layer(1_layer));

      auto* layer = document->get_layer(1_layer);
      ASSERT_EQ(core::layer_type::tile_layer, layer->type());
      EXPECT_TRUE(layer->visible());
      EXPECT_EQ(1.0, layer->opacity());

      auto* tileLayer = document->get_tile_layer(1_layer);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(5_row, tileLayer->row_count());
      EXPECT_EQ(7_col, tileLayer->col_count());
      EXPECT_EQ(360_t, tileLayer->tile_at({0_row, 0_col}));
      EXPECT_EQ(360_t, tileLayer->tile_at({3_row, 0_col}));
      EXPECT_EQ(354_t, tileLayer->tile_at({0_row, 6_col}));
      EXPECT_EQ(354_t, tileLayer->tile_at({3_row, 6_col}));
    }

    {  // Second layer
      ASSERT_TRUE(document->has_layer(2_layer));

      auto* layer = document->get_layer(2_layer);
      ASSERT_EQ(core::layer_type::tile_layer, layer->type());
      EXPECT_TRUE(layer->visible());
      EXPECT_EQ(0.8, layer->opacity());

      auto* tileLayer = document->get_tile_layer(2_layer);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(empty, tileLayer->tile_at({0_row, 0_col}));
      EXPECT_EQ(empty, tileLayer->tile_at({3_row, 0_col}));
      EXPECT_EQ(297_t, tileLayer->tile_at({0_row, 6_col}));
      EXPECT_EQ(328_t, tileLayer->tile_at({3_row, 4_col}));
      EXPECT_EQ(empty, tileLayer->tile_at({3_row, 6_col}));
    }
  }

  {  // Tilesets
    const auto* tilesets = document->tilesets();
    ASSERT_TRUE(tilesets);
    ASSERT_EQ(1, tilesets->count());

    ASSERT_TRUE(tilesets->contains(1_ts));
    EXPECT_EQ(1_ts, tilesets->current_tileset_id());

    const auto& tileset = tilesets->at(1_ts);
    EXPECT_EQ(1_t, tileset.first_id());
    EXPECT_EQ(TACTILE_QSTRING(u"terrain"), tileset.name());
    EXPECT_EQ(32_tw, tileset.get_tile_width());
    EXPECT_EQ(32_th, tileset.get_tile_height());
    EXPECT_EQ(1'024, tileset.tile_count());
    EXPECT_EQ(1'024, tileset.width());
    EXPECT_EQ(1'024, tileset.height());
    EXPECT_EQ(32_col, tileset.col_count());

    const QFileInfo file{TACTILE_QSTRING(u"terrain.png")};
    EXPECT_EQ(file.absoluteFilePath(), tileset.file().absoluteFilePath());
  }
}

// TEST_CASE("Import JSON with external tilesets", "[import_json]")
//{
//  const auto path = QStringLiteral(u"json/external.json");
//
//  QObject object;
//
//  tmx::parse_error error;
//  auto* document = service::open_map(path, &error);
//  REQUIRE(document != nullptr);
//
//  document->setParent(&object);  // to make sure that the document is deleted
//
//  SECTION("Layers")
//  {
//    REQUIRE(document->layer_count() == 2);
//    REQUIRE(document->row_count() == 6_row);
//    REQUIRE(document->col_count() == 8_col);
//
//    SECTION("First layer")
//    {
//      auto* layer = document->get_tile_layer(1_layer);
//      REQUIRE(layer->row_count() == 6_row);
//      REQUIRE(layer->col_count() == 8_col);
//      CHECK(layer->visible());
//      CHECK(layer->opacity() == 0.2);
//
//      CHECK(layer->tile_at({0_row, 0_col}).value() == 107_t);
//      CHECK(layer->tile_at({5_row, 0_col}).value() == 107_t);
//      CHECK(layer->tile_at({0_row, 7_col}).value() == 372_t);
//      CHECK(layer->tile_at({5_row, 7_col}).value() == 372_t);
//    }
//
//    SECTION("Second layer")
//    {
//      auto* layer = document->get_tile_layer(2_layer);
//      REQUIRE(layer->row_count() == 6_row);
//      REQUIRE(layer->col_count() == 8_col);
//      CHECK(layer->visible());
//      CHECK(layer->opacity() == 1.0);
//
//      CHECK(layer->tile_at({0_row, 0_col}).value() == 1293_t);
//      CHECK(layer->tile_at({0_row, 7_col}).value() == empty);
//      CHECK(layer->tile_at({5_row, 0_col}).value() == empty);
//      CHECK(layer->tile_at({2_row, 2_col}).value() == 1359_t);
//      CHECK(layer->tile_at({5_row, 7_col}).value() == empty);
//    }
//  }
//
//  SECTION("Tilesets")
//  {
//    const auto* tilesets = document->tilesets();
//    REQUIRE(tilesets != nullptr);
//    REQUIRE(tilesets->count() == 2);
//
//    REQUIRE(tilesets->has_active_tileset());
//    REQUIRE(tilesets->current_tileset_id() == 2_ts);
//
//    SECTION("First tileset")
//    {
//      const auto& tileset = tilesets->at(1_ts);
//
//      CHECK(tileset.first_id() == 1_t);
//      CHECK(tileset.name() == QStringLiteral(u"terrain"));
//      CHECK(tileset.get_tile_width() == 32_tw);
//      CHECK(tileset.get_tile_height() == 32_th);
//      CHECK(tileset.tile_count() == 1024);
//      CHECK(tileset.width() == 1024);
//      CHECK(tileset.height() == 1024);
//      CHECK(tileset.col_count() == 32_col);
//
//      const auto& actual = tileset.file();
//      const QFileInfo expected{QStringLiteral(u"terrain.png")};
//      CHECK(actual.absoluteFilePath() == expected.absoluteFilePath());
//    }
//
//    SECTION("Second tileset")
//    {
//      const auto& tileset = tilesets->at(2_ts);
//
//      CHECK(tileset.first_id() == 1025_t);
//      CHECK(tileset.name() == QStringLiteral(u"outside"));
//      CHECK(tileset.get_tile_width() == 32_tw);
//      CHECK(tileset.get_tile_height() == 32_th);
//      CHECK(tileset.tile_count() == 1024);
//      CHECK(tileset.width() == 1024);
//      CHECK(tileset.height() == 1024);
//      CHECK(tileset.col_count() == 32_col);
//
//      const auto& actual = tileset.file() ;
//      const QFileInfo expected{QStringLiteral(u"outside.png")};
//      CHECK(actual.absoluteFilePath() == expected.absoluteFilePath());
//    }
//  }
//}
