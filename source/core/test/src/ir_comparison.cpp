// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/test/ir_comparison.hpp"

#include <algorithm>  // find_if

#include <gtest/gtest.h>

#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/core/io/texture.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/tile/animation.hpp"
#include "tactile/core/tile/tile.hpp"
#include "tactile/core/tile/tileset.hpp"

namespace tactile::test {
namespace ir_comparison {

void compare_tile_animation(const Registry& registry,
                            const EntityID tile_id,
                            const ir::Tile& ir_tile)
{
  const auto* animation = registry.find<CAnimation>(tile_id);
  EXPECT_EQ(animation == nullptr, ir_tile.animation.empty());

  if (animation != nullptr) {
    EXPECT_EQ(animation->frame_index, 0);
    ASSERT_EQ(animation->frames.size(), ir_tile.animation.size());

    for (std::size_t index = 0, count = animation->frames.size(); index < count; ++index) {
      const auto& frame = animation->frames.at(index);
      const auto& ir_frame = ir_tile.animation.at(index);

      EXPECT_EQ(frame.tile_index, ir_frame.tile_index);
      EXPECT_EQ(frame.duration, ir_frame.duration);
    }
  }
  else {
    EXPECT_EQ(ir_tile.animation.size(), 0);
  }
}

}  // namespace ir_comparison

void compare_meta(const Registry& registry,
                  const EntityID meta_id,
                  const ir::Metadata& ir_meta)
{
  ASSERT_TRUE(is_meta(registry, meta_id));

  const auto& meta = registry.get<CMeta>(meta_id);

  EXPECT_EQ(meta.name, ir_meta.name);
  ASSERT_EQ(meta.properties.size(), ir_meta.properties.size());
  ASSERT_EQ(meta.components.size(), ir_meta.components.size());

  for (const auto& [prop_name, prop_value] : meta.properties) {
    const auto iter = std::ranges::find_if(
        ir_meta.properties,
        [&](const ir::NamedAttribute& ir_property) { return prop_name == ir_property.name; });

    ASSERT_NE(iter, ir_meta.properties.end())
        << "context '" << meta.name << "' is missing property: " << prop_name;
    EXPECT_EQ(prop_value, iter->value);
  }

  // TODO components
}

void compare_object(const Registry& registry,
                    const EntityID object_id,
                    const ir::Object& ir_object)
{
  ASSERT_TRUE(is_object(registry, object_id));

  compare_meta(registry, object_id, ir_object.meta);

  const auto& object = registry.get<CObject>(object_id);

  EXPECT_EQ(object.id, ir_object.id);
  EXPECT_EQ(object.type, ir_object.type);
  EXPECT_EQ(object.position, ir_object.position);
  EXPECT_EQ(object.size, ir_object.size);
  EXPECT_EQ(object.tag, ir_object.tag);
  EXPECT_EQ(object.is_visible, ir_object.visible);
}

void compare_tile_layer(const Registry& registry,
                        const EntityID layer_id,
                        const ir::Layer& ir_layer)
{
  ASSERT_TRUE(is_tile_layer(registry, layer_id));

  compare_meta(registry, layer_id, ir_layer.meta);

  const auto& layer = registry.get<CLayer>(layer_id);
  const auto& tile_layer = registry.get<CTileLayer>(layer_id);

  EXPECT_EQ(layer.persistent_id, ir_layer.id);
  EXPECT_EQ(layer.opacity, ir_layer.opacity);
  EXPECT_EQ(layer.visible, ir_layer.visible);

  ASSERT_EQ(tile_layer.extent, ir_layer.extent);
  each_layer_tile(registry, layer_id, [&](const Index2D& index, const TileID tile_id) {
    const auto ir_tile_id = ir_layer.tiles[index.y][index.x];
    EXPECT_EQ(tile_id, ir_tile_id)
        << "tiles at (" << index.y << ';' << index.x << ") don't match";
  });
}

void compare_object_layer(const Registry& registry,
                          const EntityID layer_id,
                          const ir::Layer& ir_layer)
{
  ASSERT_TRUE(is_object_layer(registry, layer_id));

  compare_meta(registry, layer_id, ir_layer.meta);

  const auto& layer = registry.get<CLayer>(layer_id);
  const auto& object_layer = registry.get<CObjectLayer>(layer_id);

  EXPECT_EQ(layer.persistent_id, ir_layer.id);
  EXPECT_EQ(layer.opacity, ir_layer.opacity);
  EXPECT_EQ(layer.visible, ir_layer.visible);

  ASSERT_EQ(object_layer.objects.size(), ir_layer.objects.size());
  EXPECT_EQ(object_layer.objects.capacity(), ir_layer.objects.size());

  for (std::size_t index = 0, count = object_layer.objects.size(); index < count; ++index) {
    const auto object_id = object_layer.objects.at(index);
    const auto& ir_object = ir_layer.objects.at(index);
    compare_object(registry, object_id, ir_object);
  }
}

void compare_group_layer(const Registry& registry,
                         const EntityID layer_id,
                         const ir::Layer& ir_layer)
{
  ASSERT_TRUE(is_group_layer(registry, layer_id));

  compare_meta(registry, layer_id, ir_layer.meta);

  const auto& layer = registry.get<CLayer>(layer_id);
  const auto& group_layer = registry.get<CGroupLayer>(layer_id);

  EXPECT_EQ(layer.persistent_id, ir_layer.id);
  EXPECT_EQ(layer.opacity, ir_layer.opacity);
  EXPECT_EQ(layer.visible, ir_layer.visible);

  ASSERT_EQ(group_layer.layers.size(), ir_layer.layers.size());
  EXPECT_EQ(group_layer.layers.capacity(), ir_layer.layers.size());

  for (std::size_t index = 0, count = group_layer.layers.size(); index < count; ++index) {
    const auto sublayer_id = group_layer.layers.at(index);
    const auto& ir_sublayer = ir_layer.layers.at(index);
    compare_layer(registry, sublayer_id, ir_sublayer);
  }
}

void compare_layer(const Registry& registry,
                   const EntityID layer_id,
                   const ir::Layer& ir_layer)
{
  switch (ir_layer.type) {
    case LayerType::kTileLayer:   compare_tile_layer(registry, layer_id, ir_layer); break;
    case LayerType::kObjectLayer: compare_object_layer(registry, layer_id, ir_layer); break;
    case LayerType::kGroupLayer:  compare_group_layer(registry, layer_id, ir_layer); break;
    default:                      FAIL() << "invalid layer type";
  }
}

void compare_tile(const Registry& registry, const EntityID tile_id, const ir::Tile& ir_tile)
{
  ASSERT_TRUE(is_tile(registry, tile_id));

  const auto& tile = registry.get<CTile>(tile_id);
  EXPECT_EQ(tile.index, ir_tile.index);

  ASSERT_EQ(tile.objects.size(), ir_tile.objects.size());
  EXPECT_EQ(tile.objects.capacity(), ir_tile.objects.size());

  for (std::size_t index = 0, count = tile.objects.size(); index < count; ++index) {
    const auto object_id = tile.objects.at(index);
    const auto& ir_object = ir_tile.objects.at(index);
    compare_object(registry, object_id, ir_object);
  }

  ir_comparison::compare_tile_animation(registry, tile_id, ir_tile);
  compare_meta(registry, tile_id, ir_tile.meta);
}

void compare_tileset(const Registry& registry,
                     const EntityID tileset_id,
                     const ir::TilesetRef& ir_tileset_ref)
{
  ASSERT_TRUE(is_tileset(registry, tileset_id));

  const auto& ir_tileset = ir_tileset_ref.tileset;

  const auto& tileset = registry.get<CTileset>(tileset_id);
  const auto& tileset_instance = registry.get<CTilesetInstance>(tileset_id);
  const auto& texture = registry.get<CTexture>(tileset_id);

  EXPECT_EQ(tileset_instance.tile_range.first_id, ir_tileset_ref.first_tile_id);
  EXPECT_EQ(tileset_instance.tile_range.count, ir_tileset.tile_count);
  EXPECT_EQ(tileset_instance.is_embedded, ir_tileset.is_embedded);

  EXPECT_EQ(tileset.tile_size, ir_tileset.tile_size);
  EXPECT_EQ(tileset.extent.cols, ir_tileset.column_count);

  EXPECT_NE(texture.id, TextureID {0});
  EXPECT_EQ(texture.size, ir_tileset.image_size);
  EXPECT_EQ(texture.path, ir_tileset.image_path);

  ASSERT_EQ(tileset.tiles.size(), ir_tileset.tile_count);
  EXPECT_EQ(tileset.tiles.capacity(), ir_tileset.tile_count);

  const auto& tile_cache = registry.get<CTileCache>();

  for (std::size_t index = 0, count = tileset.tiles.size(); index < count; ++index) {
    const auto tile_id = tileset.tiles.at(index);
    EXPECT_NE(tile_id, kInvalidEntity) << "tile #" << index << " is invalid";
    EXPECT_TRUE(is_tile(registry, tile_id));

    const auto global_tile_id =
        tileset_instance.tile_range.first_id + saturate_cast<TileID>(index);

    ASSERT_TRUE(tile_cache.tileset_mapping.contains(global_tile_id))
        << "tile " << global_tile_id << " is not in tile cache";
    EXPECT_EQ(tile_cache.tileset_mapping.at(global_tile_id), tileset_id);
  }

  for (const auto& ir_tile : ir_tileset.tiles) {
    const auto tile_index = saturate_cast<std::size_t>(ir_tile.index);
    const auto tile_id = tileset.tiles.at(tile_index);
    compare_tile(registry, tile_id, ir_tile);
  }

  compare_meta(registry, tileset_id, ir_tileset.meta);
}

void compare_map(const Registry& registry, const EntityID map_id, const ir::Map& ir_map)
{
  ASSERT_TRUE(is_map(registry, map_id));

  const auto& map = registry.get<CMap>(map_id);
  const auto& id_cache = registry.get<CMapIdCache>(map_id);
  const auto& tile_format = registry.get<CTileFormat>(map_id);

  EXPECT_EQ(map.orientation, TileOrientation::kOrthogonal);
  EXPECT_EQ(map.tile_size, ir_map.tile_size);
  EXPECT_EQ(map.extent, ir_map.extent);
  EXPECT_EQ(map.active_tileset, kInvalidEntity);
  EXPECT_EQ(map.active_layer, kInvalidEntity);
  ASSERT_EQ(map.attached_tilesets.size(), ir_map.tilesets.size());
  ASSERT_TRUE(is_group_layer(registry, map.root_layer));

  // TODO next_tile_id
  EXPECT_EQ(id_cache.next_layer_id, ir_map.next_layer_id);
  EXPECT_EQ(id_cache.next_object_id, ir_map.next_object_id);

  EXPECT_EQ(tile_format.encoding, TileEncoding::kPlainText);  // TODO
  EXPECT_EQ(tile_format.compression, ir_map.tile_format.compression);
  EXPECT_EQ(tile_format.comp_level, ir_map.tile_format.compression_level);

  for (std::size_t index = 0, count = map.attached_tilesets.size(); index < count; ++index) {
    const auto tileset_id = map.attached_tilesets.at(index);
    const auto& ir_tileset_ref = ir_map.tilesets.at(index);
    compare_tileset(registry, tileset_id, ir_tileset_ref);
  }

  const auto& root_layer = registry.get<CGroupLayer>(map.root_layer);
  for (std::size_t index = 0, count = ir_map.layers.size(); index < count; ++index) {
    const auto layer_id = root_layer.layers.at(index);
    const auto& ir_layer = ir_map.layers.at(index);
    compare_layer(registry, layer_id, ir_layer);
  }

  compare_meta(registry, map_id, ir_map.meta);
}

}  // namespace tactile::test
