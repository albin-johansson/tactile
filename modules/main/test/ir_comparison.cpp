// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "ir_comparison.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

void expect_eq(const ir::AttachedComponent& component1,
               const ir::AttachedComponent& component2)
{
  EXPECT_EQ(component1.type, component2.type);
  EXPECT_EQ(component1.attributes, component2.attributes);
}

void expect_eq(const ir::Component& component1, const ir::Component& component2)
{
  EXPECT_EQ(component1.name, component2.name);
  EXPECT_EQ(component1.attributes, component2.attributes);
}

void expect_eq(const ir::Metadata& metadata1, const ir::Metadata& metadata2)
{
  EXPECT_EQ(metadata1.name, metadata2.name);

  ASSERT_EQ(metadata1.properties.size(), metadata2.properties.size());
  EXPECT_EQ(metadata1.properties, metadata2.properties);

  ASSERT_EQ(metadata1.components.size(), metadata2.components.size());
  for (usize comp_index = 0; comp_index < metadata1.components.size(); ++comp_index) {
    const auto& component1 = metadata1.components.at(comp_index);
    const auto& component2 = metadata2.components.at(comp_index);
    expect_eq(component1, component2);
  }
}

void expect_eq(const ir::Object& object1, const ir::Object& object2)
{
  expect_eq(object1.meta, object2.meta);

  EXPECT_EQ(object1.type, object2.type);
  EXPECT_EQ(object1.tag, object2.tag);
  EXPECT_EQ(object1.x, object2.x);
  EXPECT_EQ(object1.y, object2.y);
  EXPECT_EQ(object1.width, object2.width);
  EXPECT_EQ(object1.height, object2.height);
  EXPECT_EQ(object1.visible, object2.visible);
}

void expect_eq(const ir::Layer& layer1, const ir::Layer& layer2)
{
  expect_eq(layer1.meta, layer2.meta);

  EXPECT_EQ(layer1.id, layer2.id);
  EXPECT_EQ(layer1.type, layer2.type);
  EXPECT_EQ(layer1.opacity, layer2.opacity);
  EXPECT_EQ(layer1.visible, layer2.visible);

  EXPECT_EQ(layer1.width, layer2.width);
  EXPECT_EQ(layer1.height, layer2.height);
  EXPECT_EQ(layer1.tiles, layer2.tiles);

  ASSERT_EQ(layer1.objects.size(), layer2.objects.size());
  for (usize object_index = 0; object_index < layer1.objects.size(); ++object_index) {
    const auto& object1 = layer1.objects.at(object_index);
    const auto& object2 = layer2.objects.at(object_index);
    expect_eq(object1, object2);
  }

  ASSERT_EQ(layer1.layers.size(), layer2.layers.size());
  for (usize layer_index = 0; layer_index < layer1.layers.size(); ++layer_index) {
    const auto& sublayer1 = layer1.layers.at(layer_index);
    const auto& sublayer2 = layer2.layers.at(layer_index);
    expect_eq(sublayer1, sublayer2);
  }
}

void expect_eq(const ir::TileFormat& format1, const ir::TileFormat& format2)
{
  EXPECT_EQ(format1.encoding, format2.encoding);
  EXPECT_EQ(format1.compression, format2.compression);
  EXPECT_EQ(format1.zlib_level, format2.zlib_level);
  EXPECT_EQ(format1.zstd_level, format2.zstd_level);
}

void expect_eq(const ir::AnimationFrame& frame1, const ir::AnimationFrame& frame2)
{
  EXPECT_EQ(frame1.tile_index, frame2.tile_index);
  EXPECT_EQ(frame1.duration, frame2.duration);
}

void expect_eq(const ir::Tile& tile1, const ir::Tile& tile2)
{
  expect_eq(tile1.meta, tile2.meta);

  EXPECT_EQ(tile1.index, tile2.index);

  ASSERT_EQ(tile1.objects.size(), tile2.objects.size());
  for (usize object_index = 0; object_index < tile1.objects.size(); ++object_index) {
    const auto& object1 = tile1.objects.at(object_index);
    const auto& object2 = tile2.objects.at(object_index);
    expect_eq(object1, object2);
  }

  ASSERT_EQ(tile1.animation.size(), tile2.animation.size());
  for (usize frame_index = 0; frame_index < tile1.animation.size(); ++frame_index) {
    const auto& frame1 = tile1.animation.at(frame_index);
    const auto& frame2 = tile2.animation.at(frame_index);
    expect_eq(frame1, frame2);
  }
}

void expect_eq(const ir::Tileset& tileset1, const ir::Tileset& tileset2)
{
  expect_eq(tileset1.meta, tileset2.meta);

  EXPECT_EQ(tileset1.tile_width, tileset2.tile_width);
  EXPECT_EQ(tileset1.tile_height, tileset2.tile_height);
  EXPECT_EQ(tileset1.tile_count, tileset2.tile_count);
  EXPECT_EQ(tileset1.column_count, tileset2.column_count);
  EXPECT_EQ(tileset1.image_path, tileset2.image_path);
  EXPECT_EQ(tileset1.image_width, tileset2.image_width);
  EXPECT_EQ(tileset1.image_height, tileset2.image_height);

  ASSERT_EQ(tileset1.tiles.size(), tileset2.tiles.size());
  for (usize tile_index = 0; tile_index < tileset1.tiles.size(); ++tile_index) {
    const auto& tile1 = tileset1.tiles.at(tile_index);
    const auto& tile2 = tileset2.tiles.at(tile_index);
    expect_eq(tile1, tile2);
  }
}

void expect_eq(const ir::TilesetRef& tileset_ref1, const ir::TilesetRef& tileset_ref2)
{
  EXPECT_EQ(tileset_ref1.first_tile_id, tileset_ref2.first_tile_id);
  expect_eq(tileset_ref1.tileset, tileset_ref2.tileset);
}

void expect_eq(const ir::Map& map1, const ir::Map& map2)
{
  expect_eq(map1.meta, map2.meta);
  expect_eq(map1.tile_format, map2.tile_format);

  EXPECT_EQ(map1.row_count, map2.row_count);
  EXPECT_EQ(map1.col_count, map2.col_count);
  EXPECT_EQ(map1.tile_width, map2.tile_width);
  EXPECT_EQ(map1.tile_height, map2.tile_height);
  EXPECT_EQ(map1.next_layer_id, map2.next_layer_id);
  EXPECT_EQ(map1.next_object_id, map2.next_object_id);

  ASSERT_EQ(map1.tilesets.size(), map2.tilesets.size());
  for (usize tileset_index = 0; tileset_index < map1.tilesets.size(); ++tileset_index) {
    const auto& tileset_ref1 = map1.tilesets.at(tileset_index);
    const auto& tileset_ref2 = map2.tilesets.at(tileset_index);
    expect_eq(tileset_ref1, tileset_ref2);
  }

  ASSERT_EQ(map1.layers.size(), map2.layers.size());
  for (usize layer_index = 0; layer_index < map1.layers.size(); ++layer_index) {
    const auto& layer1 = map1.layers.at(layer_index);
    const auto& layer2 = map2.layers.at(layer_index);
    expect_eq(layer1, layer2);
  }

  ASSERT_EQ(map1.components.size(), map2.components.size());
  for (usize comp_index = 0; comp_index < map1.components.size(); ++comp_index) {
    const auto& component1 = map1.components.at(comp_index);
    const auto& component2 = map2.components.at(comp_index);
    expect_eq(component1, component2);
  }
}

}  // namespace tactile::test
