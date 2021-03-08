#include <gtest/gtest.h>

#include <QStringView>  // QStringView
#include <utility>      // pair, make_pair

#include "gtest_macros.hpp"
#include "map_parser.hpp"
#include "xml_engine.hpp"

using namespace tactile;

using path_error_pair = std::pair<QStringView, parse::parse_error>;
using error = parse::parse_error;

TACTILE_DEFINE_TEST_P(XmlMapParser, path_error_pair)
{
  const auto [path, code] = GetParam();

  const parse::map_parser<parse::xml_engine> parser{path.toString()};

  if (code != parse::parse_error::none) {
    EXPECT_FALSE(parser);
  } else {
    EXPECT_TRUE(parser);
  }

  EXPECT_EQ(code, parser.error_code());
}

[[nodiscard]] constexpr auto as_pair(const QStringView path, const error code)
    -> path_error_pair
{
  return std::make_pair(path, code);
}

// clang-format off
inline const auto values = testing::Values(
  as_pair(u"xml/embedded.tmx", error::none),
  as_pair(u"xml/external.tmx", error::none),
  as_pair(u"foo/bar/gandalf.tmx", error::map_file_not_found),
  as_pair(u"xml/bad/could_not_parse_file.tmx", error::could_not_parse_file),
  as_pair(u"xml/bad/layer_missing_width.tmx", error::layer_missing_width),
  as_pair(u"xml/bad/layer_missing_height.tmx", error::layer_missing_height),
  as_pair(u"xml/bad/layer_missing_id.tmx", error::layer_missing_id),
  as_pair(u"xml/bad/layer_could_not_parse_tile.tmx", error::layer_could_not_parse_tile),
  as_pair(u"xml/bad/tileset_missing_first_gid.tmx", error::tileset_missing_first_gid),
  as_pair(u"xml/bad/tileset_missing_tile_width.tmx", error::tileset_missing_tile_width),
  as_pair(u"xml/bad/tileset_missing_tile_height.tmx", error::tileset_missing_tile_height),
  as_pair(u"xml/bad/tileset_missing_image_path.tmx", error::tileset_missing_image_path),
  as_pair(u"xml/bad/tileset_missing_name.tmx", error::tileset_missing_name),
//  as_pair(u"xml/bad/could_not_create_tileset.tmx", error::could_not_create_tileset),
  as_pair(u"xml/bad/could_not_read_external_tileset.tmx", error::could_not_read_external_tileset),
  as_pair(u"xml/bad/external_tileset_does_not_exist.tmx", error::external_tileset_does_not_exist),
  as_pair(u"xml/bad/map_missing_next_layer_id.tmx", error::map_missing_next_layer_id)
);
// clang-format on

TACTILE_REGISTER_TEST_P(XmlMapParser, values);
