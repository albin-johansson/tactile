#include "io/parsing/map_parser.hpp"

#include <gtest/gtest.h>

#include <utility>  // pair

#include "common/cstr.hpp"
#include "io/parsing/parse_error.hpp"

using namespace Tactile;
using namespace Tactile::IO;

using PE = ParseError;
using PathErrorPair = std::pair<CStr, PE>;

#define JSON_INVALID "resources/json/invalid/"
#define YAML_INVALID "resources/yaml/invalid/"
#define TMX_INVALID "resources/xml/invalid/"

// clang-format off
inline const auto maps = testing::Values(
    std::make_pair("resources/json/embedded.json", PE::None),
    std::make_pair("resources/json/external.json", PE::None),
    std::make_pair("foo.json", PE::MapDoesNotExist),
    std::make_pair(JSON_INVALID "unsupported_extension.txt", PE::MapUnsupportedExtension),
    std::make_pair(JSON_INVALID "unsupported_orientation.json", PE::MapUnsupportedOrientation),
    std::make_pair(JSON_INVALID "infinite.json", PE::MapUnsupportedInfinite),
    std::make_pair(JSON_INVALID "missing_next_layer_id.json", PE::MapMissingNextLayerId),
    std::make_pair(JSON_INVALID "missing_next_object_id.json", PE::MapMissingNextObjectId),
    std::make_pair(JSON_INVALID "missing_tilesets.json", PE::MapMissingTilesets),
    std::make_pair(JSON_INVALID "missing_layers.json", PE::MapMissingLayers),
    std::make_pair(JSON_INVALID "missing_width.json", PE::MapMissingWidth),
    std::make_pair(JSON_INVALID "missing_height.json", PE::MapMissingHeight),
    std::make_pair(JSON_INVALID "missing_tile_width.json", PE::MapMissingTileWidth),
    std::make_pair(JSON_INVALID "missing_tile_height.json", PE::MapMissingTileHeight),
    std::make_pair(JSON_INVALID "layer_missing_width.json", PE::LayerMissingWidth),
    std::make_pair(JSON_INVALID "layer_missing_height.json", PE::LayerMissingHeight),
    std::make_pair(JSON_INVALID "layer_missing_type.json", PE::LayerMissingType),
    std::make_pair(JSON_INVALID "layer_missing_id.json", PE::LayerMissingId),
    std::make_pair(JSON_INVALID "layer_unknown_type.json", PE::LayerUnknownType),
    std::make_pair(JSON_INVALID "could_not_parse_tiles.json", PE::CouldNotParseTiles),
    std::make_pair(JSON_INVALID "unsupported_tile_encoding.json", PE::UnsupportedTileEncoding),
    std::make_pair(JSON_INVALID "could_not_read_external_tileset.json", PE::CouldNotReadExternalTileset),
    std::make_pair(JSON_INVALID "tileset_missing_first_gid.json", PE::TilesetMissingFirstGid),
    std::make_pair(JSON_INVALID "tileset_missing_tile_width.json", PE::TilesetMissingTileWidth),
    std::make_pair(JSON_INVALID "tileset_missing_tile_height.json", PE::TilesetMissingTileHeight),
    std::make_pair(JSON_INVALID "tileset_missing_image_path.json", PE::TilesetMissingImagePath),
    std::make_pair(JSON_INVALID "tileset_missing_name.json", PE::TilesetMissingName),
    std::make_pair(JSON_INVALID "tileset_image_does_not_exist.json", PE::TilesetImageDoesNotExist),
    std::make_pair(JSON_INVALID "could_not_parse_property.json", PE::CouldNotParseProperty),
    std::make_pair(JSON_INVALID "property_missing_name.json", PE::PropertyMissingName),
    std::make_pair(JSON_INVALID "property_missing_type.json", PE::PropertyMissingType),
    std::make_pair(JSON_INVALID "property_unknown_type.json", PE::PropertyUnknownType),
    std::make_pair(JSON_INVALID "external_tileset_does_not_exist.json", PE::ExternalTilesetDoesNotExist),

    std::make_pair("resources/yaml/map.yaml", PE::None),

    std::make_pair("resources/xml/embedded.tmx", PE::None),
    std::make_pair("resources/xml/external.tmx", PE::None),
    std::make_pair("foo.tmx", PE::MapDoesNotExist),
    std::make_pair(TMX_INVALID "unsupported_extension.txt", PE::MapUnsupportedExtension),
    std::make_pair(TMX_INVALID "unsupported_orientation.tmx", PE::MapUnsupportedOrientation),
    std::make_pair(TMX_INVALID "infinite.tmx", PE::MapUnsupportedInfinite),
    std::make_pair(TMX_INVALID "missing_next_layer_id.tmx", PE::MapMissingNextLayerId),
    std::make_pair(TMX_INVALID "missing_next_object_id.tmx", PE::MapMissingNextObjectId),
    std::make_pair(TMX_INVALID "missing_width.tmx", PE::MapMissingWidth),
    std::make_pair(TMX_INVALID "missing_height.tmx", PE::MapMissingHeight),
    std::make_pair(TMX_INVALID "missing_tile_width.tmx", PE::MapMissingTileWidth),
    std::make_pair(TMX_INVALID "missing_tile_height.tmx", PE::MapMissingTileHeight),
    std::make_pair(TMX_INVALID "layer_missing_width.tmx", PE::LayerMissingWidth),
    std::make_pair(TMX_INVALID "layer_missing_height.tmx", PE::LayerMissingHeight),
    std::make_pair(TMX_INVALID "layer_missing_id.tmx", PE::LayerMissingId),
    std::make_pair(TMX_INVALID "could_not_parse_tiles.tmx", PE::CouldNotParseTiles),
    std::make_pair(TMX_INVALID "unsupported_tile_encoding.tmx", PE::UnsupportedTileEncoding),
    std::make_pair(TMX_INVALID "could_not_read_external_tileset.tmx", PE::CouldNotReadExternalTileset),
    std::make_pair(TMX_INVALID "tileset_missing_first_gid.tmx", PE::TilesetMissingFirstGid),
    std::make_pair(TMX_INVALID "tileset_missing_tile_width.tmx", PE::TilesetMissingTileWidth),
    std::make_pair(TMX_INVALID "tileset_missing_tile_height.tmx", PE::TilesetMissingTileHeight),
    std::make_pair(TMX_INVALID "tileset_missing_image_path.tmx", PE::TilesetMissingImagePath),
    std::make_pair(TMX_INVALID "tileset_missing_name.tmx", PE::TilesetMissingName),
    std::make_pair(TMX_INVALID "tileset_image_does_not_exist.tmx", PE::TilesetImageDoesNotExist),
    std::make_pair(TMX_INVALID "could_not_parse_property.tmx", PE::CouldNotParseProperty),
    std::make_pair(TMX_INVALID "property_missing_name.tmx", PE::PropertyMissingName),
    std::make_pair(TMX_INVALID "property_unknown_type.tmx", PE::PropertyUnknownType),
    std::make_pair(TMX_INVALID "external_tileset_does_not_exist.tmx", PE::ExternalTilesetDoesNotExist)
);
// clang-format on

class MapParserTest : public testing::TestWithParam<PathErrorPair>
{};

TEST_P(MapParserTest, MapParser)
{
  const auto [path, code] = GetParam();

  MapParser parser{path};
  ASSERT_EQ(code, parser.GetError());

  if (code != ParseError::None) {
    ASSERT_FALSE(parser);
  }
  else {
    ASSERT_TRUE(parser);
  }
}

INSTANTIATE_TEST_SUITE_P(MapParserTests, MapParserTest, maps);  // NOLINT
