#include <gtest/gtest.h>

#include <QStringView>  // QStringView
#include <utility>      // pair, make_pair

#include "gtest_macros.hpp"
#include "json_engine.hpp"
#include "map_parser.hpp"

using namespace tactile;

using path_error_pair = std::pair<QStringView, ParseError>;

TACTILE_DEFINE_TEST_P(JsonMapParser, path_error_pair)
{
  const auto [path, code] = GetParam();

  MapParser<JsonEngine> parser{path.toString()};

  if (code != ParseError::None)
  {
    EXPECT_FALSE(parser);
  }
  else
  {
    EXPECT_TRUE(parser);
  }

  EXPECT_EQ(code, parser.GetError());
}

[[nodiscard]] constexpr auto AsPair(const QStringView path,
                                    const ParseError code) -> path_error_pair
{
  return std::make_pair(path, code);
}

// clang-format off
inline const auto values = testing::Values(
  AsPair(u"json/embedded.json", ParseError::None),
  AsPair(u"json/external.json", ParseError::None),
  AsPair(u"json/invalid/does_not_exist.json", ParseError::MapFileNotFound),
  AsPair(u"json/invalid/could_not_parse.json", ParseError::CouldNotParseFile),
  AsPair(u"json/invalid/could_not_parse_tile.json", ParseError::LayerCouldNotParseTile),
  AsPair(u"json/invalid/missing_layers.json", ParseError::MapMissingLayers),
  AsPair(u"json/invalid/missing_next_layer_id.json", ParseError::MapMissingNextLayerId),
  AsPair(u"json/invalid/layer_missing_width.json", ParseError::LayerMissingWidth),
  AsPair(u"json/invalid/layer_missing_height.json", ParseError::LayerMissingHeight),
  AsPair(u"json/invalid/layer_missing_type.json", ParseError::LayerMissingType),
  AsPair(u"json/invalid/layer_missing_id.json", ParseError::LayerMissingId),
  AsPair(u"json/invalid/missing_tilesets.json", ParseError::MapMissingTilesets),
  AsPair(u"json/invalid/tileset_missing_first_gid.json", ParseError::TilesetMissingFirstGid),
  AsPair(u"json/invalid/tileset_missing_tile_width.json", ParseError::TilesetMissingTileWidth),
  AsPair(u"json/invalid/tileset_missing_tile_height.json", ParseError::TilesetMissingTileHeight),
  AsPair(u"json/invalid/tileset_missing_image_path.json", ParseError::TilesetMissingImagePath),
  AsPair(u"json/invalid/tileset_missing_name.json", ParseError::TilesetMissingName),
  AsPair(u"json/invalid/could_not_read_external_tileset.json", ParseError::CouldNotReadExternalTileset));
// clang-format on

TACTILE_REGISTER_TEST_P(JsonMapParser, values);