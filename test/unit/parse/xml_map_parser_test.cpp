#include <gtest/gtest.h>

#include <QStringView>  // QStringView
#include <utility>      // pair, make_pair

#include "gtest_macros.hpp"
#include "map_parser.hpp"
#include "xml_engine.hpp"

using namespace tactile;

using path_error_pair = std::pair<QStringView, ParseError>;

TACTILE_DEFINE_TEST_P(XmlMapParser, path_error_pair)
{
  const auto [path, code] = GetParam();

  const MapParser<XmlEngine> parser{path.toString()};

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
  AsPair(u"xml/embedded.tmx", ParseError::None),
  AsPair(u"xml/external.tmx", ParseError::None),
  AsPair(u"foo/bar/gandalf.tmx", ParseError::MapFileNotFound),
  AsPair(u"xml/bad/could_not_parse_file.tmx", ParseError::CouldNotParseFile),
  AsPair(u"xml/bad/layer_missing_width.tmx", ParseError::LayerMissingWidth),
  AsPair(u"xml/bad/layer_missing_height.tmx", ParseError::LayerMissingHeight),
  AsPair(u"xml/bad/layer_missing_id.tmx", ParseError::LayerMissingId),
  AsPair(u"xml/bad/layer_could_not_parse_tile.tmx", ParseError::LayerCouldNotParseTile),
  AsPair(u"xml/bad/tileset_missing_first_gid.tmx", ParseError::TilesetMissingFirstGid),
  AsPair(u"xml/bad/tileset_missing_tile_width.tmx", ParseError::TilesetMissingTileWidth),
  AsPair(u"xml/bad/tileset_missing_tile_height.tmx", ParseError::TilesetMissingTileHeight),
  AsPair(u"xml/bad/tileset_missing_image_path.tmx", ParseError::TilesetMissingImagePath),
  AsPair(u"xml/bad/tileset_missing_name.tmx", ParseError::TilesetMissingName),
//  as_pair(u"xml/bad/could_not_create_tileset.tmx", error::could_not_create_tileset),
  AsPair(u"xml/bad/could_not_read_external_tileset.tmx", ParseError::CouldNotReadExternalTileset),
  AsPair(u"xml/bad/external_tileset_does_not_exist.tmx", ParseError::ExternalTilesetDoesNotExist),
  AsPair(u"xml/bad/map_missing_next_layer_id.tmx", ParseError::MapMissingNextLayerId)
);
// clang-format on

TACTILE_REGISTER_TEST_P(XmlMapParser, values);
