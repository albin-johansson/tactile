#include <gtest/gtest.h>

/**
 * These test have been designed around the fact that there are two maps,
 * "embedded" and "external", each available in both XML and JSON
 * formats, where the idea is that the contents XML and JSON versions of each
 * map are identical. This means that we can reuse the code that validates the
 * state of the parsed document.
 */

#include <QDebug>

#include "gtest_macros.hpp"
#include "open_map.hpp"
#include "tactile_qstring.hpp"

using namespace tactile;

TACTILE_DEFINE_TEST_P(MapImportEmbeddedTest, QString)
{
  QObject parent;
  tmx::parse_error error;

  auto* document = service::open_map(GetParam(), &error);
  ASSERT_TRUE(document);

  document->setParent(&parent);  // Avoid memory leak

  constexpr auto rows = 10_row;
  constexpr auto cols = 15_col;
  constexpr auto tileWidth = 32_tw;
  constexpr auto tileHeight = 32_th;

  EXPECT_EQ(rows, document->row_count());
  EXPECT_EQ(cols, document->col_count());

  {  // Layers
    ASSERT_EQ(3, document->layer_count());

    ASSERT_TRUE(document->has_layer(1_layer));

    {  // First layer
      const auto* layer = document->get_layer(1_layer);
      ASSERT_TRUE(layer);
      ASSERT_EQ(core::layer_type::tile_layer, layer->type());

      EXPECT_EQ(TACTILE_QSTRING(u"G0"), layer->name());
      EXPECT_EQ(1.0, layer->opacity());
      EXPECT_TRUE(layer->visible());

      const auto* tileLayer = document->get_tile_layer(1_layer);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(rows, tileLayer->row_count());
      EXPECT_EQ(cols, tileLayer->col_count());
    }

    {  // Second layer
      const auto* layer = document->get_layer(2_layer);
      ASSERT_TRUE(layer);
      ASSERT_EQ(core::layer_type::tile_layer, layer->type());

      EXPECT_EQ(TACTILE_QSTRING(u"T0"), layer->name());
      EXPECT_EQ(0.75, layer->opacity());
      EXPECT_TRUE(layer->visible());

      const auto* tileLayer = document->get_tile_layer(2_layer);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(rows, tileLayer->row_count());
      EXPECT_EQ(cols, tileLayer->col_count());
    }

    {  // Third layer
      const auto* layer = document->get_layer(3_layer);
      ASSERT_TRUE(layer);
      ASSERT_EQ(core::layer_type::object_layer, layer->type());

      EXPECT_EQ(TACTILE_QSTRING(u"O1"), layer->name());
      EXPECT_EQ(1.0, layer->opacity());
      EXPECT_TRUE(layer->visible());

      // TODO document->get_object_layer()
    }
  }

  {  // Tilesets
    const auto* tilesets = document->tilesets();
    ASSERT_TRUE(tilesets);
    ASSERT_EQ(2, tilesets->count());

    ASSERT_TRUE(tilesets->contains(1_ts));

    const auto& terrain = tilesets->at(1_ts);
    const QFileInfo terrainPath{TACTILE_QSTRING(u"terrain.png")};

    ASSERT_EQ(TACTILE_QSTRING(u"terrain"), terrain.name());
    EXPECT_EQ(32_col, terrain.col_count());
    EXPECT_EQ(1_t, terrain.first_id());
    EXPECT_EQ(terrainPath.absoluteFilePath(),
              terrain.file().canonicalFilePath());
    EXPECT_EQ(1'024, terrain.image().width());
    EXPECT_EQ(1'024, terrain.image().height());
    EXPECT_EQ(1'024, terrain.tile_count());
    EXPECT_EQ(32_tw, terrain.get_tile_width());
    EXPECT_EQ(32_th, terrain.get_tile_height());

    ASSERT_TRUE(tilesets->contains(2_ts));

    const auto& outside = tilesets->at(2_ts);
    const QFileInfo outsidePath{TACTILE_QSTRING(u"outside.png")};

    ASSERT_EQ(TACTILE_QSTRING(u"outside"), outside.name());
    EXPECT_EQ(32_col, outside.col_count());
    EXPECT_EQ(1'025_t, outside.first_id());
    EXPECT_EQ(outsidePath.absoluteFilePath(),
              outside.file().canonicalFilePath());
    EXPECT_EQ(1'024, outside.image().width());
    EXPECT_EQ(1'024, outside.image().height());
    EXPECT_EQ(1'024, outside.tile_count());
    EXPECT_EQ(32_tw, outside.get_tile_width());
    EXPECT_EQ(32_th, outside.get_tile_height());
  }

  {  // Properties
    ASSERT_EQ(7, document->property_count());

    const auto string = TACTILE_QSTRING(u"str");
    const auto color = TACTILE_QSTRING(u"color");
    const auto integer = TACTILE_QSTRING(u"i");
    const auto floating = TACTILE_QSTRING(u"f");
    const auto boolean = TACTILE_QSTRING(u"isCool");
    const auto object = TACTILE_QSTRING(u"obj");
    const auto file = TACTILE_QSTRING(u"path");

    // TODO use document->has_property() when added

    EXPECT_NO_THROW(document->get_property(string));  // NOLINT
    EXPECT_EQ(TACTILE_QSTRING(u"foobar"),
              document->get_property(string).as_string());

    EXPECT_NO_THROW(document->get_property(color));  // NOLINT
    EXPECT_EQ(QColor{TACTILE_QSTRING(u"#ff550000")},
              document->get_property(color).as_color());

    EXPECT_NO_THROW(document->get_property(integer));  // NOLINT
    EXPECT_EQ(123, document->get_property(integer).as_integer());

    EXPECT_NO_THROW(document->get_property(floating));  // NOLINT
    EXPECT_EQ(12.3, document->get_property(floating).as_floating());

    EXPECT_NO_THROW(document->get_property(boolean));  // NOLINT
    EXPECT_TRUE(document->get_property(boolean).as_boolean());

    EXPECT_NO_THROW(document->get_property(object));  // NOLINT
    EXPECT_EQ(core::object_ref{1}, document->get_property(object).as_object());

    EXPECT_NO_THROW(document->get_property(file));  // NOLINT
    const QFileInfo path{TACTILE_QSTRING(u"../terrain.png")};
    EXPECT_EQ(path.absoluteFilePath(),
              document->get_property(file).as_file().absoluteFilePath());
  }
}

TACTILE_DEFINE_TEST_P(MapImportExternalTest, QString)
{}

inline const auto embeddedValues =
    testing::Values(TACTILE_QSTRING(u"maps/embedded.tmx"),
                    TACTILE_QSTRING(u"maps/embedded.json"));

inline const auto externalValues =
    testing::Values(TACTILE_QSTRING(u"maps/external.tmx"),
                    TACTILE_QSTRING(u"maps/external.json"));

TACTILE_REGISTER_TEST_P(MapImportEmbeddedTest, embeddedValues);
TACTILE_REGISTER_TEST_P(MapImportExternalTest, externalValues);
