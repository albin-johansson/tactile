// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_tileset_emitter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/test_util/document_view_mocks.hpp"

namespace tactile::test {

// tactile::emit_tiled_tmj_tileset
TEST(TmjFormatTilesetEmitter, EmitEmbeddedTileset)
{
  using testing::Return;
  using testing::ReturnRef;

  const SaveFormatWriteOptions options {
    .base_dir = std::filesystem::current_path(),
    .use_external_tilesets = false,
    .use_indentation = false,
    .fold_tile_layer_data = false,
  };

  std::unordered_map<TileID, TmjFormatExternalTilesetData> external_tilesets {};
  const TilesetViewMock tileset {};

  const TileID tile_id {42};
  const std::size_t column_count {3};
  const std::size_t tile_count {16};
  const Int2 tile_size {16, 16};
  const Int2 image_size {96, 64};
  const std::filesystem::path image_path {"assets/images/dummy.png"};

  EXPECT_CALL(tileset, get_first_tile_id).WillRepeatedly(Return(tile_id));
  EXPECT_CALL(tileset, tile_count).WillRepeatedly(Return(tile_count));
  EXPECT_CALL(tileset, tile_definition_count);
  EXPECT_CALL(tileset, column_count).WillRepeatedly(Return(column_count));
  EXPECT_CALL(tileset, get_tile_size).WillRepeatedly(Return(tile_size));
  EXPECT_CALL(tileset, get_image_size).WillRepeatedly(Return(image_size));
  EXPECT_CALL(tileset, get_image_path).WillRepeatedly(ReturnRef(image_path));
  EXPECT_CALL(tileset, get_meta);
  EXPECT_CALL(tileset, get_filename).Times(0);

  const auto tileset_json = emit_tiled_tmj_tileset(tileset, options, external_tilesets);

  EXPECT_EQ(tileset_json.at("firstgid"), tile_id);
  EXPECT_EQ(tileset_json.at("columns"), column_count);
  EXPECT_EQ(tileset_json.at("tilewidth"), tile_size.x());
  EXPECT_EQ(tileset_json.at("tileheight"), tile_size.y());
  EXPECT_EQ(tileset_json.at("tilecount"), tile_count);
  EXPECT_EQ(tileset_json.at("image"), image_path.string());
  EXPECT_EQ(tileset_json.at("imagewidth"), image_size.x());
  EXPECT_EQ(tileset_json.at("imageheight"), image_size.y());
  EXPECT_EQ(tileset_json.at("margin"), 0);
  EXPECT_EQ(tileset_json.at("spacing"), 0);

  EXPECT_FALSE(external_tilesets.contains(tile_id));
}

// tactile::emit_tiled_tmj_tileset
TEST(TmjFormatTilesetEmitter, EmitExternalTileset)
{
  using testing::Return;
  using testing::ReturnRef;

  const SaveFormatWriteOptions options {
    .base_dir = std::filesystem::current_path(),
    .use_external_tilesets = true,
    .use_indentation = false,
    .fold_tile_layer_data = false,
  };

  std::unordered_map<TileID, TmjFormatExternalTilesetData> external_tilesets {};
  const TilesetViewMock tileset {};

  const TileID tile_id {15};
  const std::size_t column_count {3};
  const std::size_t tile_count {16};
  const Int2 tile_size {16, 16};
  const Int2 image_size {96, 64};
  const std::filesystem::path image_path {"assets/images/dummy.png"};

  EXPECT_CALL(tileset, get_first_tile_id).WillRepeatedly(Return(tile_id));
  EXPECT_CALL(tileset, tile_count).WillRepeatedly(Return(tile_count));
  EXPECT_CALL(tileset, tile_definition_count);
  EXPECT_CALL(tileset, column_count).WillRepeatedly(Return(column_count));
  EXPECT_CALL(tileset, get_tile_size).WillRepeatedly(Return(tile_size));
  EXPECT_CALL(tileset, get_image_size).WillRepeatedly(Return(image_size));
  EXPECT_CALL(tileset, get_image_path).WillRepeatedly(ReturnRef(image_path));
  EXPECT_CALL(tileset, get_meta);
  EXPECT_CALL(tileset, get_filename).WillRepeatedly(Return("dummy_tileset"));

  const auto tileset_json = emit_tiled_tmj_tileset(tileset, options, external_tilesets);

  EXPECT_EQ(tileset_json.at("firstgid"), tile_id);
  EXPECT_EQ(tileset_json.at("source"), "dummy_tileset.tsj");

  ASSERT_TRUE(external_tilesets.contains(tile_id));
  const auto& [external_tileset_path, external_tileset_json] = external_tilesets.at(tile_id);

  EXPECT_EQ(external_tileset_path, options.base_dir / "dummy_tileset.tsj");
  EXPECT_EQ(external_tileset_json.at("columns"), column_count);
  EXPECT_EQ(external_tileset_json.at("tilewidth"), tile_size.x());
  EXPECT_EQ(external_tileset_json.at("tileheight"), tile_size.y());
  EXPECT_EQ(external_tileset_json.at("tilecount"), tile_count);
  EXPECT_EQ(external_tileset_json.at("image"), image_path.string());
  EXPECT_EQ(external_tileset_json.at("imagewidth"), image_size.x());
  EXPECT_EQ(external_tileset_json.at("imageheight"), image_size.y());
  EXPECT_EQ(external_tileset_json.at("margin"), 0);
  EXPECT_EQ(external_tileset_json.at("spacing"), 0);
}

}  // namespace tactile::test
