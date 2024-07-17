// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/layer_common.hpp"

#include <gtest/gtest.h>

#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/test_util/ir.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/test/ir_comparison.hpp"

namespace tactile::test {

// tactile::make_layer [Registry&, const ir::Layer&]
TEST(LayerCommon, MakeLayerWithTileLayerIR)
{
  Registry registry {};

  auto ir_layer = make_ir_tile_layer(LayerID {99}, MatrixExtent {8, 6});
  ir_layer.opacity = 0.75f;
  ir_layer.visible = false;
  ir_layer.meta.properties.push_back(ir::NamedAttribute {"A", Attribute {1}});
  ir_layer.meta.properties.push_back(ir::NamedAttribute {"B", Attribute {2}});

  {
    TileID tile_id {1};
    for (ssize row = 0; row < ir_layer.extent.rows; ++row) {
      for (ssize col = 0; col < ir_layer.extent.cols; ++col) {
        ir_layer.tiles[static_cast<usize>(row)][static_cast<usize>(col)] = tile_id;
        ++tile_id;
      }
    }
  }

  const auto layer_id = make_layer(registry, ir_layer);
  ASSERT_TRUE(is_tile_layer(registry, layer_id));

  compare_layer(registry, layer_id, ir_layer);
}

// tactile::make_layer [Registry&, const ir::Layer&]
TEST(LayerCommon, MakeLayerWithObjectLayerIR)
{
  Registry registry {};

  auto ir_layer = make_ir_object_layer(LayerID {123});
  ir_layer.opacity = 0.50f;
  ir_layer.meta.properties.push_back(ir::NamedAttribute {"foo", Attribute {"bar"}});

  ir_layer.objects = {
    make_ir_object(ObjectID {1}, ObjectType::kPoint),
    make_ir_object(ObjectID {2}, ObjectType::kRect),
    make_ir_object(ObjectID {3}, ObjectType::kEllipse),
  };

  const auto layer_id = make_layer(registry, ir_layer);
  ASSERT_TRUE(is_object_layer(registry, layer_id));

  compare_layer(registry, layer_id, ir_layer);
}

// tactile::make_layer [Registry&, const ir::Layer&]
TEST(LayerCommon, MakeLayerWithGroupLayerIR)
{
  Registry registry {};

  auto ir_layer = make_ir_group_layer(LayerID {1});
  ir_layer.meta.properties.push_back(ir::NamedAttribute {"1", Attribute {1}});
  ir_layer.meta.properties.push_back(ir::NamedAttribute {"2", Attribute {2}});
  ir_layer.meta.properties.push_back(ir::NamedAttribute {"3", Attribute {3}});

  ir_layer.layers = {
    make_ir_group_layer(LayerID {1},
                        {
                          make_ir_object_layer(LayerID {4}),
                          make_ir_tile_layer(LayerID {5}, MatrixExtent {5, 5}),
                          make_ir_group_layer(LayerID {6},
                                              {
                                                make_ir_object_layer(LayerID {7}),
                                                make_ir_group_layer(LayerID {8}),
                                                make_ir_object_layer(LayerID {9}),
                                              }),
                        }),
    make_ir_tile_layer(LayerID {2}, MatrixExtent {5, 5}),
    make_ir_object_layer(LayerID {3}),
  };

  const auto layer_id = make_layer(registry, ir_layer);
  ASSERT_TRUE(is_group_layer(registry, layer_id));

  compare_layer(registry, layer_id, ir_layer);
}

}  // namespace tactile::test
