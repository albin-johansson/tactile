#include "tiled_json_parser.hpp"

#include <catch.hpp>

using namespace tactile;

TEST_CASE("JSON parser: Valid embedded map", "[tiled_json_parser]")
{
  const QFileInfo path{"json/embedded.json"};
  tiled_json_parser parser{path};
  CHECK(parser);
  CHECK(parser.error_code() == tiled_json_parser::parse_error::none);
}

TEST_CASE("JSON parser: Valid external map", "[tiled_json_parser]")
{
  const QFileInfo path{"json/external.json"};
  tiled_json_parser parser{path};
  CHECK(parser);
  CHECK(parser.error_code() == tiled_json_parser::parse_error::none);
}

TEST_CASE("JSON parser: Map not found", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/does_not_exist.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
  CHECK(parser.error_code() ==
        tiled_json_parser::parse_error::map_file_not_found);
}

TEST_CASE("JSON parser: Could not parse JSON", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/could_not_parse.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
  CHECK(parser.error_code() ==
        tiled_json_parser::parse_error::could_not_parse_json);
}

TEST_CASE("JSON parser: Layer missing width property", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/layer_missing_width.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
  CHECK(parser.error_code() ==
        tiled_json_parser::parse_error::layer_missing_width);
}

TEST_CASE("JSON parser: Layer missing height property", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/layer_missing_height.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
  CHECK(parser.error_code() ==
        tiled_json_parser::parse_error::layer_missing_height);
}

TEST_CASE("JSON parser: Layer missing next layer ID", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/missing_next_layer_id.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
  CHECK(parser.error_code() ==
        tiled_json_parser::parse_error::map_missing_next_layer_id);
}

TEST_CASE("JSON parser: Missing tilesets", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/missing_tilesets.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
  CHECK(parser.error_code() ==
        tiled_json_parser::parse_error::map_missing_tilesets);
}

TEST_CASE("JSON parser: Missing layers", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/missing_layers.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
  CHECK(parser.error_code() ==
        tiled_json_parser::parse_error::map_missing_layers);
}

TEST_CASE("JSON parser: Layer missing type", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/layer_missing_type.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
  CHECK(parser.error_code() ==
        tiled_json_parser::parse_error::layer_missing_type);
}

TEST_CASE("JSON parser: Layer missing ID", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/layer_missing_id.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
  CHECK(parser.error_code() ==
        tiled_json_parser::parse_error::layer_missing_id);
}

TEST_CASE("JSON parser: Could not parse tile", "[tiled_json_parser]")
{
  const QFileInfo path{"json/invalid/could_not_parse_tile.json"};
  tiled_json_parser parser{path};
  CHECK(!parser);
//  CHECK(parser.error_code() ==
//      tiled_json_parser::parse_error::layer_could_not_parse_tile);
}

TEST_CASE("JSON parser: Tileset missing first GID", "[tiled_json_parser]")
{}

TEST_CASE("JSON parser: Tileset missing tile width", "[tiled_json_parser]")
{}

TEST_CASE("JSON parser: Tileset missing tile height", "[tiled_json_parser]")
{}

TEST_CASE("JSON parser: Tileset missing image path", "[tiled_json_parser]")
{}

TEST_CASE("JSON parser: Tileset missing name", "[tiled_json_parser]")
{}

TEST_CASE("JSON parser: Could not create tileset", "[tiled_json_parser]")
{}

TEST_CASE("JSON parser: Could not read external tileset", "[tiled_json_parser]")
{}
