#include "map_parser.hpp"

#include <catch.hpp>

#include "json_element.hpp"
#include "json_engine.hpp"
#include "xml_element.hpp"
#include "xml_engine.hpp"

using namespace tactile;

using xml_parser = tmx::map_parser<tmx::xml_engine>;
using json_parser = tmx::map_parser<tmx::json_engine>;

static_assert(tmx::is_object<tmx::xml_element>);
static_assert(tmx::is_object<tmx::json_element>);

static_assert(tmx::is_parser<tmx::xml_engine, QDomDocument, tmx::xml_element>);
static_assert(
    tmx::is_parser<tmx::json_engine, QJsonDocument, tmx::json_element>);

TEST_CASE("tmx_parser", "[map_parser]")
{
  SECTION("Valid map")
  {
    xml_parser parser{QFileInfo{"tmx/embedded.tmx"}};
    CHECK(parser);
    CHECK(parser.error_code() == tmx::parse_error::none);
  }

  SECTION("Invalid map")
  {
    xml_parser parser{QFileInfo{"tmx/does_not_exist.tmx"}};
    CHECK(!parser);
    CHECK(parser.error_code() == tmx::parse_error::map_file_not_found);
  }
}

TEST_CASE("json_parser", "[map_parser]")
{
  SECTION("Valid map")
  {
    json_parser parser{QFileInfo{"json/external.json"}};
    CHECK(parser);
    CHECK(parser.error_code() == tmx::parse_error::none);
  }

  SECTION("Invalid map")
  {
    json_parser parser{QFileInfo{"json/invalid/layer_missing_height.json"}};
    CHECK(!parser);
    CHECK(parser.error_code() == tmx::parse_error::layer_missing_height);
  }
}