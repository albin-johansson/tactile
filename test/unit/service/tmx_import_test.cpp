#include <catch.hpp>

#include "open_map.hpp"

using namespace tactile;

TEST_CASE("Import TMX with embedded tilesets", "[import_tmx]")
{
  const auto path = QStringLiteral(u"tmx/embedded.tmx");

  QObject object;

  tmx::parse_error error;
  auto* document = service::open_map(path, &error);
  REQUIRE(document != nullptr);

  document->setParent(&object);  // to make sure that the document is deleted

  SECTION("Layers")
  {
    REQUIRE(document->layer_count() == 2);
    REQUIRE(document->row_count() == 5_row);
    REQUIRE(document->col_count() == 7_col);

    SECTION("First layer")
    {
      auto* layer = document->get_tile_layer(1_layer);
      REQUIRE(layer);
      REQUIRE(layer->row_count() == 5_row);
      REQUIRE(layer->col_count() == 7_col);
      //  CHECK(layer.name() == QStringLiteral(u"Tile Layer 1"));
      CHECK(layer->visible());

      CHECK(layer->tile_at({0_row, 0_col}).value() == 360_t);
      CHECK(layer->tile_at({3_row, 0_col}).value() == 360_t);
      CHECK(layer->tile_at({0_row, 6_col}).value() == 354_t);
      CHECK(layer->tile_at({3_row, 6_col}).value() == 354_t);
    }

    SECTION("Second layer")
    {
      auto* layer = document->get_tile_layer(2_layer);
      REQUIRE(layer);
      REQUIRE(layer->row_count() == 5_row);
      REQUIRE(layer->col_count() == 7_col);
      //  CHECK(layer.name() == QStringLiteral(u"Tile Layer 2"));
      CHECK(layer->visible());

      CHECK(layer->tile_at({0_row, 0_col}).value() == empty);
      CHECK(layer->tile_at({3_row, 0_col}).value() == empty);
      CHECK(layer->tile_at({0_row, 6_col}).value() == 297_t);
      CHECK(layer->tile_at({3_row, 4_col}).value() == 328_t);
      CHECK(layer->tile_at({3_row, 6_col}).value() == empty);
    }
  }

  SECTION("Tilesets")
  {
    const auto* tilesets = document->tilesets();
    REQUIRE(tilesets != nullptr);
    REQUIRE(tilesets->count() == 1);

    REQUIRE(tilesets->current_tileset_id() == 1_ts);
    const auto& tileset = tilesets->at(1_ts);

    CHECK(tileset.first_id() == 1_t);
    CHECK(tileset.name() == QStringLiteral(u"terrain"));
    CHECK(tileset.get_tile_width() == 32_tw);
    CHECK(tileset.get_tile_height() == 32_th);
    CHECK(tileset.tile_count() == 1024);
    CHECK(tileset.width() == 1024);
    CHECK(tileset.height() == 1024);
    CHECK(tileset.col_count() == 32_col);

    const QFileInfo actual{tileset.file()};
    const QFileInfo expected{QStringLiteral(u"terrain.png")};
    CHECK(actual.absoluteFilePath() == expected.absoluteFilePath());
  }
}

TEST_CASE("Import TMX with external tilesets", "[import_tmx]")
{
  const auto path = QStringLiteral(u"tmx/external.tmx");

  QObject object;

  tmx::parse_error error;
  auto* document = service::open_map(path, &error);
  REQUIRE(document != nullptr);

  document->setParent(&object);  // to make sure that the document is deleted

  SECTION("Layers")
  {
    REQUIRE(document->layer_count() == 2);
    REQUIRE(document->row_count() == 6_row);
    REQUIRE(document->col_count() == 8_col);

    SECTION("First layer")
    {
      auto* layer = document->get_tile_layer(1_layer);
      REQUIRE(layer);
      REQUIRE(layer->row_count() == 6_row);
      REQUIRE(layer->col_count() == 8_col);
      CHECK(layer->visible());

      CHECK(layer->tile_at({0_row, 0_col}).value() == 107_t);
      CHECK(layer->tile_at({5_row, 0_col}).value() == 107_t);
      CHECK(layer->tile_at({0_row, 7_col}).value() == 372_t);
      CHECK(layer->tile_at({5_row, 7_col}).value() == 372_t);
    }

    SECTION("Second layer")
    {
      auto* layer = document->get_tile_layer(2_layer);
      REQUIRE(layer);
      REQUIRE(layer->row_count() == 6_row);
      REQUIRE(layer->col_count() == 8_col);
      CHECK(layer->visible());

      CHECK(layer->tile_at({0_row, 0_col}).value() == 1293_t);
      CHECK(layer->tile_at({0_row, 7_col}).value() == empty);
      CHECK(layer->tile_at({5_row, 0_col}).value() == empty);
      CHECK(layer->tile_at({2_row, 2_col}).value() == 1359_t);
      CHECK(layer->tile_at({5_row, 7_col}).value() == empty);
    }
  }

  SECTION("Tilesets")
  {
    const auto* tilesets = document->tilesets();
    REQUIRE(tilesets != nullptr);
    REQUIRE(tilesets->count() == 2);

    REQUIRE(tilesets->has_active_tileset());
    REQUIRE(tilesets->current_tileset_id() == 2_ts);

    SECTION("First tileset")
    {
      const auto& tileset = tilesets->at(1_ts);

      CHECK(tileset.first_id() == 1_t);
      CHECK(tileset.name() == QStringLiteral(u"terrain"));
      CHECK(tileset.get_tile_width() == 32_tw);
      CHECK(tileset.get_tile_height() == 32_th);
      CHECK(tileset.tile_count() == 1024);
      CHECK(tileset.width() == 1024);
      CHECK(tileset.height() == 1024);
      CHECK(tileset.col_count() == 32_col);

      const QFileInfo actual{tileset.file()};
      const QFileInfo expected{QStringLiteral(u"terrain.png")};
      CHECK(actual.absoluteFilePath() == expected.absoluteFilePath());
    }

    SECTION("Second tileset")
    {
      const auto& tileset = tilesets->at(2_ts);

      CHECK(tileset.first_id() == 1025_t);
      CHECK(tileset.name() == QStringLiteral(u"outside"));
      CHECK(tileset.get_tile_width() == 32_tw);
      CHECK(tileset.get_tile_height() == 32_th);
      CHECK(tileset.tile_count() == 1024);
      CHECK(tileset.width() == 1024);
      CHECK(tileset.height() == 1024);
      CHECK(tileset.col_count() == 32_col);

      const QFileInfo actual{tileset.file()};
      const QFileInfo expected{QStringLiteral(u"outside.png")};
      CHECK(actual.absoluteFilePath() == expected.absoluteFilePath());
    }
  }
}