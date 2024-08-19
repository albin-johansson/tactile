// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/layer_common.hpp"

#include "tactile/base/io/save/ir.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

auto make_layer(Registry& registry, const ir::Layer& ir_layer) -> EntityID
{
  EntityID layer_id {kInvalidEntity};

  switch (ir_layer.type) {
    case LayerType::kTileLayer: {
      layer_id = make_tile_layer(registry, ir_layer.extent);

      for (ssize row = 0; row < ir_layer.extent.rows; ++row) {
        for (ssize col = 0; col < ir_layer.extent.cols; ++col) {
          const auto tile_id =
              ir_layer.tiles[static_cast<usize>(row)][static_cast<usize>(col)];
          set_layer_tile(registry, layer_id, MatrixIndex {row, col}, tile_id);
        }
      }

      break;
    }
    case LayerType::kObjectLayer: {
      layer_id = make_object_layer(registry);

      auto& object_layer = registry.get<CObjectLayer>(layer_id);
      object_layer.objects.reserve(ir_layer.objects.size());

      for (const auto& ir_object : ir_layer.objects) {
        object_layer.objects.push_back(make_object(registry, ir_object));
      }

      break;
    }
    case LayerType::kGroupLayer: {
      layer_id = make_group_layer(registry);

      auto& group_layer = registry.get<CGroupLayer>(layer_id);
      group_layer.layers.reserve(ir_layer.layers.size());

      for (const auto& ir_sublayer : ir_layer.layers) {
        group_layer.layers.push_back(make_layer(registry, ir_sublayer));
      }

      break;
    }
    default: throw Exception {"invalid layer type"};
  }

  auto& layer = registry.get<CLayer>(layer_id);
  layer.persistent_id = ir_layer.id;
  layer.opacity = ir_layer.opacity;
  layer.visible = ir_layer.visible;

  convert_ir_metadata(registry, layer_id, ir_layer.meta);

  TACTILE_ASSERT(is_layer(registry, layer_id));
  return layer_id;
}

void destroy_layer(Registry& registry, const EntityID layer_id)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id) ||    //
                 is_object_layer(registry, layer_id) ||  //
                 is_group_layer(registry, layer_id));

  if (registry.has<CTileLayer>(layer_id)) {
    destroy_tile_layer(registry, layer_id);
  }
  else if (registry.has<CObjectLayer>(layer_id)) {
    destroy_object_layer(registry, layer_id);
  }
  else if (registry.has<CGroupLayer>(layer_id)) {
    destroy_group_layer(registry, layer_id);
  }
}

}  // namespace tactile
