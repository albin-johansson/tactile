// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/tile_layer.hpp"

#include <gtest/gtest.h>

#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/dense_tile_matrix.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

class TileLayerTest : public testing::Test
{
 protected:
  Registry mRegistry {};
};

/// \trace tactile::is_tile_layer
TEST_F(TileLayerTest, IsTileLayer)
{
  EXPECT_FALSE(is_tile_layer(mRegistry, kInvalidEntity));
  EXPECT_FALSE(is_tile_layer(mRegistry, mRegistry.make_entity()));

  const auto tile_layer_entity =
      make_tile_layer(mRegistry, MatrixExtent {2, 2});
  EXPECT_TRUE(is_tile_layer(mRegistry, tile_layer_entity));
}

/// \trace tactile::make_tile_layer
TEST_F(TileLayerTest, MakeTileLayer)
{
  const MatrixExtent extent {5, 6};
  const auto tile_layer_entity = make_tile_layer(mRegistry, extent);

  EXPECT_TRUE(is_layer(mRegistry, tile_layer_entity));
  EXPECT_TRUE(is_tile_layer(mRegistry, tile_layer_entity));
  EXPECT_TRUE(mRegistry.has<CMeta>(tile_layer_entity));
  EXPECT_TRUE(mRegistry.has<CLayer>(tile_layer_entity));
  EXPECT_TRUE(mRegistry.has<CTileLayer>(tile_layer_entity));
  EXPECT_TRUE(mRegistry.has<CDenseTileLayer>(tile_layer_entity));
  EXPECT_FALSE(mRegistry.has<CSparseTileLayer>(tile_layer_entity));

  const auto& meta = mRegistry.get<CMeta>(tile_layer_entity);
  const auto& layer = mRegistry.get<CLayer>(tile_layer_entity);
  const auto& dense_tile_layer =
      mRegistry.get<CDenseTileLayer>(tile_layer_entity);

  EXPECT_EQ(meta.name, "");
  EXPECT_EQ(meta.properties.size(), 0);
  EXPECT_EQ(meta.components.size(), 0);

  EXPECT_EQ(layer.persistent_id, nothing);
  EXPECT_EQ(layer.opacity, 1.0f);
  EXPECT_TRUE(layer.visible);

  EXPECT_EQ(dense_tile_layer.tiles.get_extent(), extent);
  for (usize row = 0; row < extent.rows; ++row) {
    for (usize col = 0; col < extent.cols; ++col) {
      const MatrixIndex index {row, col};
      EXPECT_EQ(dense_tile_layer.tiles.at(index), kEmptyTile);
    }
  }
}

/// \trace tactile::destroy_tile_layer
TEST_F(TileLayerTest, DestroyTileLayer)
{
  const auto tile_layer_entity =
      make_tile_layer(mRegistry, MatrixExtent {5, 4});

  EXPECT_TRUE(mRegistry.is_valid(tile_layer_entity));
  EXPECT_EQ(mRegistry.count<CMeta>(), 1);
  EXPECT_EQ(mRegistry.count<CLayer>(), 1);
  EXPECT_EQ(mRegistry.count<CTileLayer>(), 1);
  EXPECT_EQ(mRegistry.count<CDenseTileLayer>(), 1);
  EXPECT_EQ(mRegistry.count<CSparseTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count(), 4);

  destroy_tile_layer(mRegistry, tile_layer_entity);

  EXPECT_FALSE(mRegistry.is_valid(tile_layer_entity));
  EXPECT_EQ(mRegistry.count<CMeta>(), 0);
  EXPECT_EQ(mRegistry.count<CLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CDenseTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CSparseTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count(), 0);
}

/// \trace tactile::to_dense_tile_layer
/// \trace tactile::to_sparse_tile_layer
TEST_F(TileLayerTest, DenseSparseConversion)
{
  const MatrixExtent extent {6, 4};
  const auto tile_layer_entity = make_tile_layer(mRegistry, extent);

  EXPECT_TRUE(mRegistry.has<CDenseTileLayer>(tile_layer_entity));
  EXPECT_FALSE(mRegistry.has<CSparseTileLayer>(tile_layer_entity));

  {
    auto& dense_tile_layer = mRegistry.get<CDenseTileLayer>(tile_layer_entity);
    EXPECT_EQ(dense_tile_layer.tiles.get_extent(), extent);

    dense_tile_layer.tiles[MatrixIndex {0, 0}] = TileID {42};
    dense_tile_layer.tiles[MatrixIndex {0, 1}] = TileID {73};
    dense_tile_layer.tiles[MatrixIndex {3, 2}] = TileID {99};
    dense_tile_layer.tiles[MatrixIndex {5, 3}] = TileID {36};
  }

  to_sparse_tile_layer(mRegistry, tile_layer_entity);

  EXPECT_FALSE(mRegistry.has<CDenseTileLayer>(tile_layer_entity));
  EXPECT_TRUE(mRegistry.has<CSparseTileLayer>(tile_layer_entity));

  {
    const auto& sparse_tile_layer =
        mRegistry.get<CSparseTileLayer>(tile_layer_entity);
    EXPECT_EQ(sparse_tile_layer.tiles.get_extent(), extent);

    EXPECT_EQ(sparse_tile_layer.tiles.at(MatrixIndex {0, 0}), TileID {42});
    EXPECT_EQ(sparse_tile_layer.tiles.at(MatrixIndex {0, 1}), TileID {73});
    EXPECT_EQ(sparse_tile_layer.tiles.at(MatrixIndex {3, 2}), TileID {99});
    EXPECT_EQ(sparse_tile_layer.tiles.at(MatrixIndex {5, 3}), TileID {36});
  }

  to_dense_tile_layer(mRegistry, tile_layer_entity);

  EXPECT_TRUE(mRegistry.has<CDenseTileLayer>(tile_layer_entity));
  EXPECT_FALSE(mRegistry.has<CSparseTileLayer>(tile_layer_entity));

  {
    const auto& dense_tile_layer =
        mRegistry.get<CDenseTileLayer>(tile_layer_entity);
    EXPECT_EQ(dense_tile_layer.tiles.get_extent(), extent);

    EXPECT_EQ(dense_tile_layer.tiles.at(MatrixIndex {0, 0}), TileID {42});
    EXPECT_EQ(dense_tile_layer.tiles.at(MatrixIndex {0, 1}), TileID {73});
    EXPECT_EQ(dense_tile_layer.tiles.at(MatrixIndex {3, 2}), TileID {99});
    EXPECT_EQ(dense_tile_layer.tiles.at(MatrixIndex {5, 3}), TileID {36});
  }
}

}  // namespace tactile
