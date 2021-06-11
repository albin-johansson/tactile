#include <gtest/gtest.h>

#include <QStringView>  // QStringView
#include <utility>      // pair, make_pair

#include "gtest_macros.hpp"
#include "json_engine.hpp"
#include "map_parser.hpp"

using namespace tactile;

using path_error_pair = std::pair<QStringView, parse::ParseError>;
using error = parse::ParseError;

TACTILE_DEFINE_TEST_P(JsonMapParser, path_error_pair)
{
  const auto [path, code] = GetParam();

  parse::MapParser<parse::json_engine> parser{path.toString()};

  if (code != parse::ParseError::None) {
    EXPECT_FALSE(parser);
  } else {
    EXPECT_TRUE(parser);
  }

  EXPECT_EQ(code, parser.GetError());
}

[[nodiscard]] constexpr auto as_pair(const QStringView path, const error code)
    -> path_error_pair
{
  return std::make_pair(path, code);
}

// clang-format off
inline const auto values = testing::Values(
  as_pair(u"json/embedded.json", error::None),
  as_pair(u"json/external.json", error::None),

  as_pair(u"json/invalid/does_not_exist.json", error::MapFileNotFound),
  as_pair(u"json/invalid/could_not_parse.json", error::CouldNotParseFile),
  as_pair(u"json/invalid/could_not_parse_tile.json", error::LayerCouldNotParseTile),
  as_pair(u"json/invalid/missing_layers.json", error::MapMissingLayers),
  as_pair(u"json/invalid/missing_next_layer_id.json", error::MapMissingNextLayerId),
  as_pair(u"json/invalid/layer_missing_width.json", error::LayerMissingWidth),
  as_pair(u"json/invalid/layer_missing_height.json", error::LayerMissingHeight),
  as_pair(u"json/invalid/layer_missing_type.json", error::LayerMissingType),
  as_pair(u"json/invalid/layer_missing_id.json", error::LayerMissingId),
  as_pair(u"json/invalid/missing_tilesets.json", error::MapMissingTilesets),
  as_pair(u"json/invalid/tileset_missing_first_gid.json", error::TilesetMissingFirstGid),
  as_pair(u"json/invalid/tileset_missing_tile_width.json", error::TilesetMissingTileWidth),
  as_pair(u"json/invalid/tileset_missing_tile_height.json", error::TilesetMissingTileHeight),
  as_pair(u"json/invalid/tileset_missing_image_path.json", error::TilesetMissingImagePath),
  as_pair(u"json/invalid/tileset_missing_name.json", error::TilesetMissingName),
  as_pair(u"json/invalid/could_not_read_external_tileset.json", error::CouldNotReadExternalTileset));
// clang-format on

TACTILE_REGISTER_TEST_P(JsonMapParser, values);