#include "map_parser.hpp"

#include <gtest/gtest.h>

#include "json_element.hpp"
#include "json_engine.hpp"
#include "xml_element.hpp"
#include "xml_engine.hpp"

using namespace tactile;

using xml_parser = tmx::map_parser<tmx::xml_engine>;
using json_parser = tmx::map_parser<tmx::json_engine>;

static_assert(tmx::is_object<tmx::xml_element>);
static_assert(tmx::is_object<tmx::json_element>);

// clang-format off
static_assert(tmx::is_parser<tmx::xml_engine, QDomDocument, tmx::xml_element>);
static_assert(tmx::is_parser<tmx::json_engine, QJsonDocument, tmx::json_element>);
// clang-format on

TEST(TMXParser, ValidMap)
{
  const xml_parser parser{TACTILE_QSTRING(u"tmx/embedded.tmx")};
  EXPECT_TRUE(parser);
  EXPECT_EQ(tmx::parse_error::none, parser.error_code());
}

TEST(TMXParser, InvalidMap)
{
  const xml_parser parser{TACTILE_QSTRING(u"tmx/does_not_exist.tmx")};
  EXPECT_FALSE(parser);
  EXPECT_EQ(tmx::parse_error::map_file_not_found, parser.error_code());
}

TEST(JSONParser, ValidMap)
{
  const json_parser parser{TACTILE_QSTRING(u"json/external.json")};
  EXPECT_TRUE(parser);
  EXPECT_EQ(tmx::parse_error::none, parser.error_code());
}

TEST(JSONParser, InvalidMap)
{
  const auto path = TACTILE_QSTRING(u"json/invalid/layer_missing_height.json");
  const json_parser parser{path};
  EXPECT_FALSE(parser);
  EXPECT_EQ(tmx::parse_error::layer_missing_height, parser.error_code());
}
