// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_tile_emitter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/test_util/document_view_mocks.hpp"

namespace tactile::test {

// tactile::emit_tiled_tmj_tile
TEST(TmjFormatTileEmitter, EmitPlainTile)
{
  const TileIndex tile_index {54};

  const TileViewMock tile {};

  EXPECT_CALL(tile, get_index).WillOnce(testing::Return(tile_index));
  EXPECT_CALL(tile, animation_frame_count);
  EXPECT_CALL(tile, object_count);
  EXPECT_CALL(tile, get_meta);

  const auto tile_json = emit_tiled_tmj_tile(tile);

  EXPECT_EQ(tile_json.at("id"), tile_index);
  EXPECT_FALSE(tile_json.contains("animation"));
  EXPECT_FALSE(tile_json.contains("objectgroup"));
  EXPECT_FALSE(tile_json.contains("properties"));
}

// tactile::emit_tiled_tmj_tile
TEST(TmjFormatTileEmitter, EmitAnimatedTile)
{
  const TileIndex tile_index {68};

  const TileViewMock tile {};

  EXPECT_CALL(tile, get_index).WillOnce(testing::Return(tile_index));
  EXPECT_CALL(tile, animation_frame_count).WillRepeatedly(testing::Return(3));
  EXPECT_CALL(tile, get_animation_frame)
      .WillOnce(testing::Return(std::make_pair(TileIndex {68}, Milliseconds {1})))
      .WillOnce(testing::Return(std::make_pair(TileIndex {69}, Milliseconds {2})))
      .WillOnce(testing::Return(std::make_pair(TileIndex {70}, Milliseconds {3})));
  EXPECT_CALL(tile, object_count);
  EXPECT_CALL(tile, get_meta);

  const auto tile_json = emit_tiled_tmj_tile(tile);

  EXPECT_EQ(tile_json.at("id"), tile_index);
  EXPECT_FALSE(tile_json.contains("objectgroup"));
  EXPECT_FALSE(tile_json.contains("properties"));
  ASSERT_TRUE(tile_json.contains("animation"));

  EXPECT_EQ(tile_json.at("animation").at(0).at("tileid"), TileIndex {68});
  EXPECT_EQ(tile_json.at("animation").at(0).at("duration"), 1);

  EXPECT_EQ(tile_json.at("animation").at(1).at("tileid"), TileIndex {69});
  EXPECT_EQ(tile_json.at("animation").at(1).at("duration"), 2);

  EXPECT_EQ(tile_json.at("animation").at(2).at("tileid"), TileIndex {70});
  EXPECT_EQ(tile_json.at("animation").at(2).at("duration"), 3);
}

// tactile::emit_tiled_tmj_tile
TEST(TmjFormatTileEmitter, EmitTileWithObjects)
{
  const TileIndex tile_index {123};

  const TileViewMock tile {};

  EXPECT_CALL(tile, get_index).WillRepeatedly(testing::Return(tile_index));
  EXPECT_CALL(tile, animation_frame_count);
  EXPECT_CALL(tile, object_count).WillRepeatedly(testing::Return(2));
  EXPECT_CALL(tile, get_meta);

  const auto tile_json = emit_tiled_tmj_tile(tile);

  EXPECT_EQ(tile_json.at("id"), tile_index);
  EXPECT_FALSE(tile_json.contains("animation"));
  EXPECT_FALSE(tile_json.contains("properties"));
  ASSERT_TRUE(tile_json.contains("objectgroup"));

  EXPECT_EQ(tile_json.at("objectgroup")
                .at("objects")
                .get_ref<const nlohmann::json::array_t&>()
                .capacity(),
            2);
}

}  // namespace tactile::test
