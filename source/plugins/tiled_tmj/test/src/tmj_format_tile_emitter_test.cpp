// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_tile_emitter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/test_util/document_view_mocks.hpp"
#include "tactile/test_util/ir.hpp"

namespace tactile::test {

// tactile::emit_tiled_tmj_tile
TEST(TmjFormatTileEmitter, EmitPlainTile)
{
  const ir::Tile tile {
    .meta = ir::Metadata {},
    .index = TileIndex {54},
    .objects = {},
    .animation = {},
  };

  const TileViewMock tile_view {nullptr, tile};

  EXPECT_CALL(tile_view, get_index).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, animation_frame_count).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, object_count).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, get_meta).Times(testing::AtLeast(1));

  const auto tile_json = emit_tiled_tmj_tile(tile_view);

  EXPECT_EQ(tile_json.at("id"), tile.index);
  EXPECT_FALSE(tile_json.contains("animation"));
  EXPECT_FALSE(tile_json.contains("objectgroup"));
  EXPECT_FALSE(tile_json.contains("properties"));
}

// tactile::emit_tiled_tmj_tile
TEST(TmjFormatTileEmitter, EmitAnimatedTile)
{
  const ir::Tile tile {
    .meta = ir::Metadata {},
    .index = TileIndex {68},
    .objects = {},
    .animation =
        {
          ir::AnimationFrame {TileIndex {68}, std::chrono::milliseconds {1}},
          ir::AnimationFrame {TileIndex {69}, std::chrono::milliseconds {2}},
          ir::AnimationFrame {TileIndex {70}, std::chrono::milliseconds {3}},
        },
  };

  const auto& frame1 = tile.animation.at(0);
  const auto& frame2 = tile.animation.at(1);
  const auto& frame3 = tile.animation.at(2);

  const TileViewMock tile_view {nullptr, tile};

  EXPECT_CALL(tile_view, get_index).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, animation_frame_count).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, get_animation_frame).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, object_count).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, get_meta).Times(testing::AtLeast(1));

  const auto tile_json = emit_tiled_tmj_tile(tile_view);

  EXPECT_EQ(tile_json.at("id"), tile.index);
  EXPECT_FALSE(tile_json.contains("objectgroup"));
  EXPECT_FALSE(tile_json.contains("properties"));
  ASSERT_TRUE(tile_json.contains("animation"));

  EXPECT_EQ(tile_json.at("animation").at(0).at("tileid"), frame1.tile_index);
  EXPECT_EQ(tile_json.at("animation").at(0).at("duration"), frame1.duration.count());

  EXPECT_EQ(tile_json.at("animation").at(1).at("tileid"), frame2.tile_index);
  EXPECT_EQ(tile_json.at("animation").at(1).at("duration"), frame2.duration.count());

  EXPECT_EQ(tile_json.at("animation").at(2).at("tileid"), frame3.tile_index);
  EXPECT_EQ(tile_json.at("animation").at(2).at("duration"), frame3.duration.count());
}

// tactile::emit_tiled_tmj_tile
TEST(TmjFormatTileEmitter, EmitTileWithObjects)
{
  const ir::Tile tile {
    .meta = ir::Metadata {},
    .index = TileIndex {123},
    .objects =
        {
          make_ir_object(ObjectID {1}),
          make_ir_object(ObjectID {2}),
        },
    .animation = {},
  };

  const TileViewMock tile_view {nullptr, tile};

  EXPECT_CALL(tile_view, get_index).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, animation_frame_count).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, object_count).Times(testing::AtLeast(1));
  EXPECT_CALL(tile_view, get_meta).Times(testing::AtLeast(1));

  const auto tile_json = emit_tiled_tmj_tile(tile_view);

  EXPECT_EQ(tile_json.at("id"), tile.index);
  EXPECT_FALSE(tile_json.contains("animation"));
  EXPECT_FALSE(tile_json.contains("properties"));
  ASSERT_TRUE(tile_json.contains("objectgroup"));

  EXPECT_EQ(tile_json.at("objectgroup")
                .at("objects")
                .get_ref<const nlohmann::json::array_t&>()
                .capacity(),
            tile.objects.size());
}

}  // namespace tactile::test
