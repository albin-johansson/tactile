#include "map_parser.hpp"

#include <catch.hpp>

using namespace tactile;

using xml_parser = tmx::map_parser<tmx::xml_engine>;

static_assert(tmx::is_object<tmx::tmx_element>);
static_assert(tmx::is_object<tmx::json_element>);

static_assert(tmx::is_parser<tmx::xml_engine, QDomDocument, tmx::tmx_element>);

TEST_CASE("foo", "[map_parser]")
{
  SECTION("Valid map")
  {
    xml_parser parser{QFileInfo{"tmx/embedded.tmx"}};
    CHECK(parser);
    CHECK(parser.error_code() == parse_error::none);
  }

  SECTION("Invalid map")
  {
    xml_parser parser{QFileInfo{"tmx/does_not_exist.tmx"}};
    CHECK(!parser);
    CHECK(parser.error_code() == parse_error::map_file_not_found);
  }
}