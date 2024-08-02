// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_map_emitter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/test_util/document_view_mocks.hpp"
#include "tactile/test_util/ir.hpp"

namespace tactile::test {

// tactile::emit_tiled_tmj_map
TEST(TmjFormatMapEmitter, EmitEmptyMap)
{
  const auto ir_map = make_ir_map(MatrixExtent {5, 4});
  const MapViewMock map_view {ir_map};

  EXPECT_CALL(map_view, get_tile_size);
  EXPECT_CALL(map_view, get_extent);
  EXPECT_CALL(map_view, get_next_layer_id);
  EXPECT_CALL(map_view, get_next_object_id);
  EXPECT_CALL(map_view, get_tile_encoding).Times(0);
  EXPECT_CALL(map_view, get_tile_compression).Times(0);
  EXPECT_CALL(map_view, get_compression_level);
  EXPECT_CALL(map_view, layer_count);
  EXPECT_CALL(map_view, tileset_count);
  EXPECT_CALL(map_view, component_count);
  EXPECT_CALL(map_view, get_meta);

  const auto map_json = emit_tiled_tmj_map(map_view);

  EXPECT_EQ(map_json.at("type"), "map");
  EXPECT_EQ(map_json.at("tiledversion"), "1.9.0");
  EXPECT_EQ(map_json.at("version"), "1.7");
  EXPECT_EQ(map_json.at("renderorder"), "right-down");
  EXPECT_EQ(map_json.at("infinite"), false);
  EXPECT_EQ(map_json.at("orientation"), "orthogonal");
  EXPECT_EQ(map_json.at("width"), ir_map.extent.cols);
  EXPECT_EQ(map_json.at("height"), ir_map.extent.rows);
  EXPECT_EQ(map_json.at("tilewidth"), ir_map.tile_size.x());
  EXPECT_EQ(map_json.at("tileheight"), ir_map.tile_size.y());
  EXPECT_EQ(map_json.at("nextlayerid"), ir_map.next_layer_id);
  EXPECT_EQ(map_json.at("nextobjectid"), ir_map.next_object_id);
  EXPECT_EQ(map_json.at("name"), ir_map.meta.name);
  EXPECT_EQ(map_json.at("compressionlevel"), -1);
}

}  // namespace tactile::test
