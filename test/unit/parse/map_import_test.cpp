#include <gtest/gtest.h>

/**
 * These test have been designed around the fact that there are two maps,
 * "embedded" and "external", each available in both XML and JSON
 * formats, where the idea is that the contents XML and JSON versions of each
 * map are identical. This means that we can reuse the code that validates the
 * state of the parsed document.
 */

#include "gtest_macros.hpp"
#include "open_map.hpp"
#include "tactile_qstring.hpp"

using namespace tactile;

TACTILE_DEFINE_TEST_P(MapImportEmbeddedTest, QString)
{
  const auto path = GetParam();

  QObject parent;
  tmx::parse_error error;

  auto* document = service::open_map(path, &error);
  ASSERT_TRUE(document);

  document->setParent(&parent);  // Avoid memory leak

  {
    // Layers: GO: tile_layer, T0: tile_layer
    constexpr auto rows = 10_row;
    constexpr auto cols = 15_col;
    constexpr auto tileWidth = 32_tw;
    constexpr auto tileHeight = 32_th;

    ASSERT_EQ(3, document->layer_count());
  }

  {  // Tilesets: terrain, outside
    const auto* tilesets = document->tilesets();
    ASSERT_TRUE(tilesets);
    ASSERT_EQ(2, tilesets->count());
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
