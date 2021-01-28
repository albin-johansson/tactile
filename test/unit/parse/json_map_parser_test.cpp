#include <gtest/gtest.h>

#include <QStringView>  // QStringView
#include <utility>      // pair, make_pair

#include "json_engine.hpp"
#include "map_parser.hpp"
#include "gtest_macros.hpp"

using namespace tactile;

using path_error_pair = std::pair<QStringView, tmx::parse_error>;
using error = tmx::parse_error;

TACTILE_DEFINE_TEST_P(JsonMapParser, path_error_pair)
{
  const auto [path, code] = GetParam();

  const QFileInfo file{path.toString()};
  const tmx::map_parser<tmx::json_engine> parser{file};

  if (code != tmx::parse_error::none) {
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
  as_pair(u"json/embedded.json", error::none),
  as_pair(u"json/external.json", error::none),

  as_pair(u"json/invalid/does_not_exist.json", error::map_file_not_found),
  as_pair(u"json/invalid/could_not_parse.json", error::could_not_parse_file),
  as_pair(u"json/invalid/could_not_parse_tile.json", error::layer_could_not_parse_tile),
  as_pair(u"json/invalid/missing_layers.json", error::map_missing_layers),
  as_pair(u"json/invalid/missing_next_layer_id.json", error::map_missing_next_layer_id),
  as_pair(u"json/invalid/layer_missing_width.json", error::layer_missing_width),
  as_pair(u"json/invalid/layer_missing_height.json", error::layer_missing_height),
  as_pair(u"json/invalid/layer_missing_type.json", error::layer_missing_type),
  as_pair(u"json/invalid/layer_missing_id.json", error::layer_missing_id),
  as_pair(u"json/invalid/missing_tilesets.json", error::map_missing_tilesets),
  as_pair(u"json/invalid/tileset_missing_first_gid.json", error::tileset_missing_first_gid),
  as_pair(u"json/invalid/tileset_missing_tile_width.json", error::tileset_missing_tile_width),
  as_pair(u"json/invalid/tileset_missing_tile_height.json", error::tileset_missing_tile_height),
  as_pair(u"json/invalid/tileset_missing_image_path.json", error::tileset_missing_image_path),
  as_pair(u"json/invalid/tileset_missing_name.json", error::tileset_missing_name),
  as_pair(u"json/invalid/could_not_read_external_tileset.json", error::could_not_read_external_tileset));
// clang-format on

TACTILE_REGISTER_TEST_P(JsonMapParser, values);