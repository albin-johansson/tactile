#include <gtest/gtest.h>

#include <QStringView>  // QStringView
#include <utility>      // pair, make_pair

#include "gtest_macros.hpp"
#include "map_parser.hpp"
#include "xml_engine.hpp"

using namespace tactile;

using path_error_pair = std::pair<QStringView, parse::ParseError>;
using error = parse::ParseError;

TACTILE_DEFINE_TEST_P(XmlMapParser, path_error_pair)
{
  const auto [path, code] = GetParam();

  const parse::MapParser<parse::XmlEngine> parser{path.toString()};

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
  as_pair(u"xml/embedded.tmx", error::None),
  as_pair(u"xml/external.tmx", error::None),
  as_pair(u"foo/bar/gandalf.tmx", error::MapFileNotFound),
  as_pair(u"xml/bad/could_not_parse_file.tmx", error::CouldNotParseFile),
  as_pair(u"xml/bad/layer_missing_width.tmx", error::LayerMissingWidth),
  as_pair(u"xml/bad/layer_missing_height.tmx", error::LayerMissingHeight),
  as_pair(u"xml/bad/layer_missing_id.tmx", error::LayerMissingId),
  as_pair(u"xml/bad/layer_could_not_parse_tile.tmx", error::LayerCouldNotParseTile),
  as_pair(u"xml/bad/tileset_missing_first_gid.tmx", error::TilesetMissingFirstGid),
  as_pair(u"xml/bad/tileset_missing_tile_width.tmx", error::TilesetMissingTileWidth),
  as_pair(u"xml/bad/tileset_missing_tile_height.tmx", error::TilesetMissingTileHeight),
  as_pair(u"xml/bad/tileset_missing_image_path.tmx", error::TilesetMissingImagePath),
  as_pair(u"xml/bad/tileset_missing_name.tmx", error::TilesetMissingName),
//  as_pair(u"xml/bad/could_not_create_tileset.tmx", error::could_not_create_tileset),
  as_pair(u"xml/bad/could_not_read_external_tileset.tmx", error::CouldNotReadExternalTileset),
  as_pair(u"xml/bad/external_tileset_does_not_exist.tmx", error::ExternalTilesetDoesNotExist),
  as_pair(u"xml/bad/map_missing_next_layer_id.tmx", error::MapMissingNextLayerId)
);
// clang-format on

TACTILE_REGISTER_TEST_P(XmlMapParser, values);
