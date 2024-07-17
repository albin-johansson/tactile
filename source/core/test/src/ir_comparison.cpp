// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/test/ir_comparison.hpp"

#include <algorithm>  // find_if

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/tile/animation.hpp"
#include "tactile/core/tile/tile.hpp"

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

    for (usize index = 0, count = animation->frames.size(); index < count; ++index) {
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
  const auto& tile_data = get_tile_layer_data(registry, layer_id);

  EXPECT_EQ(layer.persistent_id, ir_layer.id);
  EXPECT_EQ(layer.opacity, ir_layer.opacity);
  EXPECT_EQ(layer.visible, ir_layer.visible);

  ASSERT_EQ(tile_data.get_extent(), ir_layer.extent);
  for (ssize row = 0; row < ir_layer.extent.rows; ++row) {
    for (ssize col = 0; col < ir_layer.extent.cols; ++col) {
      const MatrixIndex index {row, col};

      const auto ir_tile_id = ir_layer.tiles[static_cast<usize>(row)][static_cast<usize>(col)];
      EXPECT_EQ(tile_data.at(index), ir_tile_id)
          << "tiles at (" << row << ';' << col << ") don't match";
    }
  }
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

  for (usize index = 0, count = object_layer.objects.size(); index < count; ++index) {
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

  for (usize index = 0, count = group_layer.layers.size(); index < count; ++index) {
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

  for (usize index = 0, count = tile.objects.size(); index < count; ++index) {
    const auto object_id = tile.objects.at(index);
    const auto& ir_object = ir_tile.objects.at(index);
    compare_object(registry, object_id, ir_object);
  }

  ir_comparison::compare_tile_animation(registry, tile_id, ir_tile);
  compare_meta(registry, tile_id, ir_tile.meta);
}

}  // namespace tactile::test
