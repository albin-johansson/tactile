// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/test_util/ir_presets.hpp"

#include <utility>  // move

#include "tactile/test_util/ir.hpp"

namespace tactile::test {

auto make_complex_ir_metadata(std::string name) -> ir::Metadata
{
  auto meta = make_ir_metadata(std::move(name));

  meta.properties = {
    {"p_str", Attribute {"foobar"}},
    {"p_path", Attribute {std::filesystem::path {"foo/bar.txt"}}},
    {"p_int", Attribute {42}},
    {"p_int2", Attribute {Int2 {1, 2}}},
    {"p_int3", Attribute {Int3 {1, 2, 3}}},
    {"p_int4", Attribute {Int4 {1, 2, 3, 4}}},
    {"p_float", Attribute {1.5f}},
    {"p_float2", Attribute {Float2 {1, 2}}},
    {"p_float3", Attribute {Float3 {1, 2, 3}}},
    {"p_float4", Attribute {Float4 {1, 2, 3, 4}}},
    {"p_bool", Attribute {true}},
    {"p_color", Attribute {UColor {0x12, 0x34, 0x56, 0x78}}},
    {"p_object", Attribute {ObjectRef {99}}},
  };

  // TODO components

  return meta;
}

auto make_complex_ir_object(const ObjectID id, const ObjectType type) -> ir::Object
{
  auto object = make_ir_object(id, type, Float2 {12, 34}, Float2 {56, 78});

  object.meta = make_complex_ir_metadata(std::format("complex_object {}", id));
  object.tag = "this-is-a-tag";

  return object;
}

auto make_complex_ir_object_layer(const LayerID id, ObjectID& next_object_id) -> ir::Layer
{
  auto object_layer = make_ir_object_layer(id);

  object_layer.meta = make_complex_ir_metadata(std::format("complex_layer {}", id));
  object_layer.opacity = 0.50f;
  object_layer.visible = false;

  object_layer.objects.reserve(3);
  object_layer.objects = {
    make_complex_ir_object(next_object_id++, ObjectType::kPoint),
    make_complex_ir_object(next_object_id++, ObjectType::kRect),
    make_complex_ir_object(next_object_id++, ObjectType::kEllipse),
  };

  return object_layer;
}

auto make_complex_ir_tile_layer(const LayerID id, const MatrixExtent& extent) -> ir::Layer
{
  auto tile_layer = make_ir_tile_layer(id, extent);

  tile_layer.meta = make_complex_ir_metadata(std::format("complex_layer {}", id));
  tile_layer.opacity = 1.0f;
  tile_layer.visible = true;

  TileID tile_id {1};
  for (ssize row = 0; row < extent.rows; ++row) {
    for (ssize col = 0; col < extent.cols; ++col) {
      const auto u_row = static_cast<usize>(row);
      const auto u_col = static_cast<usize>(col);
      tile_layer.tiles[u_row][u_col] = tile_id;
      ++tile_id;
    }
  }

  return tile_layer;
}

// root
// ├── tile_layer1
// ├── tile_layer2
// └── group1
//    ├── tile_layer3
//    ├── group2
//    │   ├── group3
//    │   ├── object_layer
//    │   │   └── ...
//    │   └── tile_layer5
//    └── tile_layer4
auto make_complex_ir_group_layer(LayerID& next_layer_id,
                                 ObjectID& next_object_id,
                                 const MatrixExtent& extent) -> ir::Layer
{
  auto root = make_ir_group_layer(next_layer_id++);

  auto tile_layer1 = make_complex_ir_tile_layer(next_layer_id++, extent);
  auto tile_layer2 = make_complex_ir_tile_layer(next_layer_id++, extent);
  auto tile_layer3 = make_complex_ir_tile_layer(next_layer_id++, extent);
  auto tile_layer4 = make_complex_ir_tile_layer(next_layer_id++, extent);
  auto tile_layer5 = make_complex_ir_tile_layer(next_layer_id++, extent);

  auto object_layer = make_complex_ir_object_layer(next_layer_id++, next_object_id);

  auto group3 = make_ir_group_layer(next_layer_id++);
  group3.meta = make_complex_ir_metadata("G3");

  auto group2 = make_ir_group_layer(next_layer_id++);
  group2.layers.reserve(3);
  group2.layers.push_back(std::move(group3));
  group2.layers.push_back(std::move(object_layer));
  group2.layers.push_back(std::move(tile_layer5));
  group2.meta = make_complex_ir_metadata("G2");

  auto group1 = make_ir_group_layer(next_layer_id++);
  group1.layers.reserve(3);
  group1.layers.push_back(std::move(tile_layer3));
  group1.layers.push_back(std::move(group2));
  group1.layers.push_back(std::move(tile_layer4));
  group1.meta = make_complex_ir_metadata("G1");

  root.layers.reserve(3);
  root.layers.push_back(std::move(tile_layer1));
  root.layers.push_back(std::move(tile_layer2));
  root.layers.push_back(std::move(group1));

  return root;
}

auto make_complex_ir_tile(const TileIndex index, ObjectID& next_object_id) -> ir::Tile
{
  auto tile = make_ir_tile(index);

  tile.meta = make_complex_ir_metadata(std::format("complex_tile {}", index));

  tile.objects.reserve(3);
  tile.objects.push_back(make_complex_ir_object(next_object_id++, ObjectType::kPoint));
  tile.objects.push_back(make_complex_ir_object(next_object_id++, ObjectType::kRect));
  tile.objects.push_back(make_complex_ir_object(next_object_id++, ObjectType::kEllipse));

  tile.animation.reserve(4);
  tile.animation.push_back(ir::AnimationFrame {TileIndex {1}, Milliseconds {100}});
  tile.animation.push_back(ir::AnimationFrame {TileIndex {2}, Milliseconds {101}});
  tile.animation.push_back(ir::AnimationFrame {TileIndex {3}, Milliseconds {102}});
  tile.animation.push_back(ir::AnimationFrame {TileIndex {4}, Milliseconds {103}});

  return tile;
}

auto make_complex_ir_tileset(TileID& next_tile_id, ObjectID& next_object_id) -> ir::TilesetRef
{
  auto tileset_ref = make_ir_tileset_ref(next_tile_id);

  tileset_ref.tileset.meta = make_complex_ir_metadata("complex_tileset");
  tileset_ref.tileset.tiles.reserve(3);
  tileset_ref.tileset.tiles.push_back(make_complex_ir_tile(TileIndex {0}, next_object_id));
  tileset_ref.tileset.tiles.push_back(make_complex_ir_tile(TileIndex {1}, next_object_id));
  tileset_ref.tileset.tiles.push_back(make_complex_ir_tile(TileIndex {2}, next_object_id));

  return tileset_ref;
}

auto make_complex_ir_map(const ir::TileFormat& tile_format) -> ir::Map
{
  const MatrixExtent extent {8, 10};

  auto map = make_ir_map(extent);

  map.meta = make_complex_ir_metadata("complex_map");
  map.tile_size = Int2 {40, 30};
  map.next_layer_id = 10;
  map.next_object_id = 100;
  map.tile_format = tile_format;

  // TODO components

  TileID next_tile_id {10};
  map.tilesets.push_back(make_complex_ir_tileset(next_tile_id, map.next_object_id));

  map.layers.reserve(3);
  map.layers = {
    make_complex_ir_tile_layer(map.next_layer_id++, map.extent),
    make_complex_ir_object_layer(map.next_layer_id++, map.next_object_id),
    make_complex_ir_group_layer(map.next_layer_id, map.next_object_id, map.extent),
  };

  return map;
}

}  // namespace tactile::test
