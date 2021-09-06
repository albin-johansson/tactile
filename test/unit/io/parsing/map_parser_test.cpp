#include "io/parsing/map_parser.hpp"

#include <gtest/gtest.h>

#include <utility>  // pair

#include "aliases/cstr.hpp"
#include "io/parsing/parse_error.hpp"

using namespace Tactile;
using namespace Tactile::IO;

using PathErrorPair = std::pair<czstring, ParseError>;

// clang-format off
inline const auto maps = testing::Values(
    std::make_pair("resources/json/embedded.json", ParseError::None),
    std::make_pair("resources/json/external.json", ParseError::None),
    std::make_pair("resources/json/invalid/unsupported_extension.txt", ParseError::MapUnsupportedExtension),
    std::make_pair("resources/json/invalid/unsupported_orientation.json", ParseError::MapUnsupportedOrientation),
    std::make_pair("resources/json/invalid/infinite.json", ParseError::MapUnsupportedInfinite),
    std::make_pair("foo.json", ParseError::MapDoesNotExist),
    std::make_pair("resources/json/invalid/missing_next_layer_id.json", ParseError::MapMissingNextLayerId),
    std::make_pair("resources/json/invalid/missing_next_object_id.json", ParseError::MapMissingNextObjectId),
    std::make_pair("resources/json/invalid/missing_tilesets.json", ParseError::MapMissingTilesets),
    std::make_pair("resources/json/invalid/missing_layers.json", ParseError::MapMissingLayers),
    std::make_pair("resources/json/invalid/layer_missing_width.json", ParseError::LayerMissingWidth),
    std::make_pair("resources/json/invalid/layer_missing_height.json", ParseError::LayerMissingHeight),
    std::make_pair("resources/json/invalid/layer_missing_type.json", ParseError::LayerMissingType),
    std::make_pair("resources/json/invalid/layer_missing_id.json", ParseError::LayerMissingId),
    std::make_pair("resources/json/invalid/layer_unknown_type.json", ParseError::LayerUnknownType),
    std::make_pair("resources/json/invalid/could_not_parse_tiles.json", ParseError::CouldNotParseTiles),
    std::make_pair("resources/json/invalid/unsupported_tile_encoding.json", ParseError::UnsupportedTileEncoding),
    std::make_pair("resources/json/invalid/could_not_read_external_tileset.json", ParseError::CouldNotReadExternalTileset),
    std::make_pair("resources/json/invalid/tileset_missing_first_gid.json", ParseError::TilesetMissingFirstGid),
    std::make_pair("resources/json/invalid/tileset_missing_tile_width.json", ParseError::TilesetMissingTileWidth),
    std::make_pair("resources/json/invalid/tileset_missing_tile_height.json", ParseError::TilesetMissingTileHeight),
    std::make_pair("resources/json/invalid/tileset_missing_image_path.json", ParseError::TilesetMissingImagePath),
    std::make_pair("resources/json/invalid/tileset_missing_name.json", ParseError::TilesetMissingName),
    std::make_pair("resources/json/invalid/tileset_image_does_not_exist.json", ParseError::TilesetImageDoesNotExist),
    std::make_pair("resources/json/invalid/could_not_parse_property.json", ParseError::CouldNotParseProperty),
    std::make_pair("resources/json/invalid/property_missing_name.json", ParseError::PropertyMissingName),
    std::make_pair("resources/json/invalid/property_unknown_type.json", ParseError::PropertyUnknownType),

    std::make_pair("resources/xml/embedded.tmx", ParseError::None),
    std::make_pair("resources/xml/external.tmx", ParseError::None),
    std::make_pair("resources/xml/invalid/unsupported_extension.txt", ParseError::MapUnsupportedExtension),
    std::make_pair("resources/xml/invalid/unsupported_orientation.tmx", ParseError::MapUnsupportedOrientation),
    std::make_pair("resources/xml/invalid/infinite.tmx", ParseError::MapUnsupportedInfinite),
    std::make_pair("foo.tmx", ParseError::MapDoesNotExist),
    std::make_pair("resources/xml/invalid/missing_next_layer_id.tmx", ParseError::MapMissingNextLayerId),
    std::make_pair("resources/xml/invalid/missing_next_object_id.tmx", ParseError::MapMissingNextObjectId),
    std::make_pair("resources/xml/invalid/layer_missing_width.tmx", ParseError::LayerMissingWidth),
    std::make_pair("resources/xml/invalid/layer_missing_height.tmx", ParseError::LayerMissingHeight),
    std::make_pair("resources/xml/invalid/layer_missing_id.tmx", ParseError::LayerMissingId),
    std::make_pair("resources/xml/invalid/could_not_parse_tiles.tmx", ParseError::CouldNotParseTiles),
    std::make_pair("resources/xml/invalid/unsupported_tile_encoding.tmx", ParseError::UnsupportedTileEncoding),
    std::make_pair("resources/xml/invalid/could_not_read_external_tileset.tmx", ParseError::CouldNotReadExternalTileset),
    std::make_pair("resources/xml/invalid/tileset_missing_first_gid.tmx", ParseError::TilesetMissingFirstGid),
    std::make_pair("resources/xml/invalid/tileset_missing_tile_width.tmx", ParseError::TilesetMissingTileWidth),
    std::make_pair("resources/xml/invalid/tileset_missing_tile_height.tmx", ParseError::TilesetMissingTileHeight),
    std::make_pair("resources/xml/invalid/tileset_missing_image_path.tmx", ParseError::TilesetMissingImagePath),
    std::make_pair("resources/xml/invalid/tileset_missing_name.tmx", ParseError::TilesetMissingName),
    std::make_pair("resources/xml/invalid/tileset_image_does_not_exist.tmx", ParseError::TilesetImageDoesNotExist),
    std::make_pair("resources/xml/invalid/could_not_parse_property.tmx", ParseError::CouldNotParseProperty),
    std::make_pair("resources/xml/invalid/property_missing_name.tmx", ParseError::PropertyMissingName),
    std::make_pair("resources/xml/invalid/property_unknown_type.tmx", ParseError::PropertyUnknownType)
);
// clang-format on

class MapParserTest : public testing::TestWithParam<PathErrorPair>
{};

TEST_P(MapParserTest, MapParser)
{
  const auto [path, code] = GetParam();

  MapParser parser{path};
  ASSERT_EQ(code, parser.GetError());

  if (code != ParseError::None)
  {
    ASSERT_FALSE(parser);
  }
  else
  {
    ASSERT_TRUE(parser);
  }
}

INSTANTIATE_TEST_SUITE_P(MapParserTests, MapParserTest, maps);  // NOLINT
