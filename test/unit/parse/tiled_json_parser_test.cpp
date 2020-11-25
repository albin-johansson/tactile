#include <catch.hpp>

#include "czstring.hpp"
#include "json_engine.hpp"
#include "map_parser.hpp"

using namespace tactile;

namespace {

void test_error(czstring path, const tmx::parse_error error)
{
  const QFileInfo file{path};

  tmx::map_parser<tmx::json_engine> parser{file};
  if (error != tmx::parse_error::none) {
    CHECK(!parser);
  } else {
    CHECK(parser);
  }
  CHECK(parser.error_code() == error);
}

}  // namespace

TEST_CASE("JSON parser: Valid embedded map", "[tiled_json_parser]")
{
  test_error("json/embedded.json", tmx::parse_error::none);
}

TEST_CASE("JSON parser: Valid external map", "[tiled_json_parser]")
{
  test_error("json/external.json", tmx::parse_error::none);
}

TEST_CASE("JSON parser: Map not found", "[tiled_json_parser]")
{
  test_error("json/invalid/does_not_exist.json",
             tmx::parse_error::map_file_not_found);
}

TEST_CASE("JSON parser: Could not parse JSON", "[tiled_json_parser]")
{
  test_error("json/invalid/could_not_parse.json",
             tmx::parse_error::could_not_parse_file);
}

TEST_CASE("JSON parser: Layer missing width property", "[tiled_json_parser]")
{
  test_error("json/invalid/layer_missing_width.json",
             tmx::parse_error::layer_missing_width);
}

TEST_CASE("JSON parser: Layer missing height property", "[tiled_json_parser]")
{
  test_error("json/invalid/layer_missing_height.json",
             tmx::parse_error::layer_missing_height);
}

TEST_CASE("JSON parser: Layer missing next layer ID", "[tiled_json_parser]")
{
  test_error("json/invalid/missing_next_layer_id.json",
             tmx::parse_error::map_missing_next_layer_id);
}

TEST_CASE("JSON parser: Missing tilesets", "[tiled_json_parser]")
{
  test_error("json/invalid/missing_tilesets.json",
             tmx::parse_error::map_missing_tilesets);
}

TEST_CASE("JSON parser: Missing layers", "[tiled_json_parser]")
{
  test_error("json/invalid/missing_layers.json",
             tmx::parse_error::map_missing_layers);
}

TEST_CASE("JSON parser: Layer missing type", "[tiled_json_parser]")
{
  test_error("json/invalid/layer_missing_type.json",
             tmx::parse_error::layer_missing_type);
}

TEST_CASE("JSON parser: Layer missing ID", "[tiled_json_parser]")
{
  test_error("json/invalid/layer_missing_id.json",
             tmx::parse_error::layer_missing_id);
}

TEST_CASE("JSON parser: Could not parse tile", "[tiled_json_parser]")
{
  test_error("json/invalid/could_not_parse_tile.json",
             tmx::parse_error::layer_could_not_parse_tile);
}

TEST_CASE("JSON parser: Tileset missing first GID", "[tiled_json_parser]")
{
  test_error("json/invalid/tileset_missing_first_gid.json",
             tmx::parse_error::tileset_missing_first_gid);
}

TEST_CASE("JSON parser: Tileset missing tile width", "[tiled_json_parser]")
{
  test_error("json/invalid/tileset_missing_tile_width.json",
             tmx::parse_error::tileset_missing_tile_width);
}

TEST_CASE("JSON parser: Tileset missing tile height", "[tiled_json_parser]")
{
  test_error("json/invalid/tileset_missing_tile_height.json",
             tmx::parse_error::tileset_missing_tile_height);
}

TEST_CASE("JSON parser: Tileset missing image path", "[tiled_json_parser]")
{
  test_error("json/invalid/tileset_missing_image_path.json",
             tmx::parse_error::tileset_missing_image_path);
}

TEST_CASE("JSON parser: Tileset missing name", "[tiled_json_parser]")
{
  test_error("json/invalid/tileset_missing_name.json",
             tmx::parse_error::tileset_missing_name);
}

TEST_CASE("JSON parser: Could not read external tileset", "[tiled_json_parser]")
{
  test_error("json/invalid/could_not_read_external_tileset.json",
             tmx::parse_error::could_not_read_external_tileset);
}
