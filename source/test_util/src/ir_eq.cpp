// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/test_util/ir_eq.hpp"

#include <algorithm>  // find_if
#include <format>     // format
#include <iostream>   // cout

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace tactile::test {

void expect_eq(const ir::NamedAttribute& attr1,
               const ir::NamedAttribute& attr2,
               const AttributeEqMode mode)
{
  EXPECT_EQ(attr1.name, attr2.name);

  if (mode == AttributeEqMode::kTiled &&
      (attr1.value.is_vector() || attr2.value.is_vector())) {
    std::clog << std::format("[expect_eq] Skipping check for vector attribute '{}'\n",
                             attr1.name);
  }
  else {
    EXPECT_EQ(attr1.value, attr2.value);
  }
}

void expect_eq(const ir::AttachedComponent& component1,
               const ir::AttachedComponent& component2,
               const AttributeEqMode mode)
{
  EXPECT_EQ(component1.type, component2.type);

  for (const auto& attr1 : component1.attributes) {
    const auto attr2_iter = std::ranges::find_if(
        component2.attributes,
        [&](const ir::NamedAttribute& attr2) { return attr2.name == attr1.name; });

    ASSERT_NE(attr2_iter, component2.attributes.end());
    expect_eq(attr1, *attr2_iter, mode);
  }
}

void expect_eq(const ir::Component& component1,
               const ir::Component& component2,
               const AttributeEqMode mode)
{
  EXPECT_EQ(component1.name, component2.name);

  for (const auto& attr1 : component1.attributes) {
    const auto attr2_iter = std::ranges::find_if(
        component2.attributes,
        [&](const ir::NamedAttribute& attr2) { return attr2.name == attr1.name; });

    ASSERT_NE(attr2_iter, component2.attributes.end());
    expect_eq(attr1, *attr2_iter, mode);
  }
}

void expect_eq(const ir::Metadata& meta1,
               const ir::Metadata& meta2,
               const AttributeEqMode mode)
{
  EXPECT_EQ(meta1.name, meta2.name);
  ASSERT_EQ(meta1.properties.size(), meta2.properties.size());
  ASSERT_EQ(meta1.components.size(), meta2.components.size());

  for (const auto& prop1 : meta1.properties) {
    const auto prop2_iter = std::ranges::find_if(
        meta2.properties,
        [&](const ir::NamedAttribute& prop2) { return prop2.name == prop1.name; });

    ASSERT_NE(prop2_iter, meta2.properties.end());
    expect_eq(prop1, *prop2_iter, mode);
  }

  for (const auto& component1 : meta1.components) {
    const auto component2_iter =
        std::ranges::find_if(meta2.components, [&](const ir::AttachedComponent& component2) {
          return component2.type == component1.type;
        });

    ASSERT_NE(component2_iter, meta2.components.end());
    expect_eq(component1, *component2_iter, mode);
  }
}

void expect_eq(const ir::Object& object1,
               const ir::Object& object2,
               const AttributeEqMode mode)
{
  EXPECT_EQ(object1.id, object2.id);
  EXPECT_EQ(object1.type, object2.type);
  EXPECT_EQ(object1.position, object2.position);
  EXPECT_EQ(object1.size, object2.size);
  EXPECT_EQ(object1.tag, object2.tag);
  EXPECT_EQ(object1.visible, object2.visible);

  expect_eq(object1.meta, object2.meta, mode);
}

void expect_eq(const ir::Layer& layer1, const ir::Layer& layer2, const AttributeEqMode mode)
{
  EXPECT_EQ(layer1.id, layer2.id);
  EXPECT_EQ(layer1.type, layer2.type);
  EXPECT_EQ(layer1.opacity, layer2.opacity);
  EXPECT_EQ(layer1.extent, layer2.extent);

  switch (layer1.type) {
    case LayerType::kTileLayer: {
      EXPECT_EQ(layer1.extent, layer2.extent);
      EXPECT_THAT(layer1.tiles, testing::ContainerEq(layer2.tiles));
      break;
    }
    case LayerType::kObjectLayer: {
      EXPECT_EQ(layer1.objects.size(), layer2.objects.size());

      for (const auto& object1 : layer1.objects) {
        const auto object2_iter = std::ranges::find_if(
            layer2.objects,
            [&](const ir::Object& object2) { return object2.id == object1.id; });

        ASSERT_NE(object2_iter, layer2.objects.end());
        expect_eq(object1, *object2_iter, mode);
      }

      break;
    }
    case LayerType::kGroupLayer: {
      EXPECT_EQ(layer1.layers.size(), layer2.layers.size());

      for (const auto& sublayer1 : layer1.layers) {
        const auto sublayer2_iter = std::ranges::find_if(
            layer2.layers,
            [&](const ir::Layer& sublayer2) { return sublayer2.id == sublayer1.id; });

        ASSERT_NE(sublayer2_iter, layer2.layers.end());
        expect_eq(sublayer1, *sublayer2_iter, mode);
      }

      break;
    }
  }

  expect_eq(layer1.meta, layer2.meta, mode);
}

void expect_eq(const ir::Tile& tile1, const ir::Tile& tile2, const AttributeEqMode mode)
{
  EXPECT_EQ(tile1.index, tile2.index);
  ASSERT_EQ(tile1.objects.size(), tile2.objects.size());
  ASSERT_EQ(tile1.animation.size(), tile2.animation.size());

  for (const auto& object1 : tile1.objects) {
    const auto object2_iter = std::ranges::find_if(
        tile2.objects,
        [&](const ir::Object& object2) { return object2.id == object1.id; });

    ASSERT_NE(object2_iter, tile2.objects.end());
    expect_eq(object1, *object2_iter, mode);
  }

  for (usize index = 0; index < tile1.animation.size(); ++index) {
    const auto& frame1 = tile1.animation.at(index);
    const auto& frame2 = tile2.animation.at(index);

    EXPECT_EQ(frame1.tile_index, frame2.tile_index);
    EXPECT_EQ(frame1.duration, frame2.duration);
  }

  expect_eq(tile1.meta, tile2.meta, mode);
}

void expect_eq(const ir::Tileset& tileset1,
               const ir::Tileset& tileset2,
               const AttributeEqMode mode)
{
  EXPECT_EQ(tileset1.tile_size, tileset2.tile_size);
  EXPECT_EQ(tileset1.tile_count, tileset2.tile_count);
  EXPECT_EQ(tileset1.column_count, tileset2.column_count);
  EXPECT_EQ(tileset1.image_size, tileset2.image_size);
  EXPECT_EQ(tileset1.image_path, tileset2.image_path);
  EXPECT_EQ(tileset1.is_embedded, tileset2.is_embedded);
  ASSERT_EQ(tileset1.tiles.size(), tileset2.tiles.size());

  for (const auto& tile1 : tileset1.tiles) {
    const auto tile2_iter = std::ranges::find_if(tileset2.tiles, [&](const ir::Tile& tile2) {
      return tile2.index == tile1.index;
    });

    ASSERT_NE(tile2_iter, tileset2.tiles.end());
    expect_eq(tile1, *tile2_iter, mode);
  }

  expect_eq(tileset1.meta, tileset2.meta, mode);
}

void expect_eq(const ir::TilesetRef& tileset_ref1,
               const ir::TilesetRef& tileset_ref2,
               const AttributeEqMode mode)
{
  EXPECT_EQ(tileset_ref1.first_tile_id, tileset_ref2.first_tile_id);

  expect_eq(tileset_ref1.tileset, tileset_ref2.tileset, mode);
}

void expect_eq(const ir::TileFormat& format1, const ir::TileFormat& format2)
{
  EXPECT_EQ(format1.encoding, format2.encoding);
  EXPECT_EQ(format1.compression, format2.compression);
  EXPECT_EQ(format1.compression_level, format2.compression_level);
}

void expect_eq(const ir::Map& map1, const ir::Map& map2, const AttributeEqMode mode)
{
  EXPECT_EQ(map1.extent, map2.extent);
  EXPECT_EQ(map1.tile_size, map2.tile_size);
  EXPECT_EQ(map1.next_layer_id, map2.next_layer_id);
  EXPECT_EQ(map1.next_object_id, map2.next_object_id);
  EXPECT_EQ(map1.components.size(), map2.components.size());
  EXPECT_EQ(map1.tilesets.size(), map2.tilesets.size());
  ASSERT_EQ(map1.layers.size(), map2.layers.size());

  for (const auto& component1 : map1.components) {
    const auto component2_iter = std::ranges::find_if(
        map2.components,
        [&](const ir::Component& component2) { return component2.name == component1.name; });

    ASSERT_NE(component2_iter, map2.components.end());
    expect_eq(component1, *component2_iter, mode);
  }

  for (const auto& tileset_ref1 : map1.tilesets) {
    const auto tileset_ref2_iter =
        std::ranges::find_if(map2.tilesets, [&](const ir::TilesetRef& tileset_ref2) {
          return tileset_ref2.first_tile_id == tileset_ref1.first_tile_id;
        });

    ASSERT_NE(tileset_ref2_iter, map2.tilesets.end());
    expect_eq(tileset_ref1, *tileset_ref2_iter, mode);
  }

  for (usize index = 0; index < map1.layers.size(); ++index) {
    const auto& layer1 = map1.layers.at(index);
    const auto& layer2 = map2.layers.at(index);

    expect_eq(layer1, layer2, mode);
  }

  expect_eq(map1.tile_format, map2.tile_format);
  expect_eq(map1.meta, map2.meta, mode);
}

}  // namespace tactile::test
