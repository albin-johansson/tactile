// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/tile_layer.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/io/tile_io.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

class TileLayerTest : public testing::TestWithParam<bool>
{
 protected:
  Registry mRegistry {};
  bool mTestingDenseLayer {};

  void SetUp() override
  {
    mTestingDenseLayer = GetParam();
  }

  [[nodiscard]]
  auto make_test_layer(const MatrixExtent& extent = {5, 5})
  {
    const auto layer_id = make_tile_layer(mRegistry, extent);

    if (mTestingDenseLayer) {
      convert_to_dense_tile_layer(mRegistry, layer_id);
    }
    else {
      convert_to_sparse_tile_layer(mRegistry, layer_id);
    }

    return layer_id;
  }

  void check_tile_validity(const EntityID layer_id, const MatrixExtent& expected_extent)
  {
    const auto& tile_layer = mRegistry.get<CTileLayer>(layer_id);
    EXPECT_EQ(tile_layer.extent, expected_extent);

    for (MatrixExtent::value_type row = 0; row < expected_extent.rows; ++row) {
      for (MatrixExtent::value_type col = 0; col < expected_extent.cols; ++col) {
        const MatrixIndex index {row, col};
        EXPECT_TRUE(get_layer_tile(mRegistry, layer_id, index).has_value());
      }
    }
  }

  void test_resize(const EntityID layer_id, const MatrixExtent& original_extent)
  {
    const auto resize_and_validate = [&](const MatrixExtent& extent) {
      resize_tile_layer(mRegistry, layer_id, extent);
      check_tile_validity(layer_id, extent);
    };

    resize_and_validate(MatrixExtent {original_extent.rows + 1, original_extent.cols});
    resize_and_validate(MatrixExtent {original_extent.rows - 1, original_extent.cols});

    resize_and_validate(MatrixExtent {original_extent.rows, original_extent.cols + 1});
    resize_and_validate(MatrixExtent {original_extent.rows, original_extent.cols - 1});

    resize_and_validate(MatrixExtent {original_extent.rows + 2, original_extent.cols + 3});
    resize_and_validate(MatrixExtent {original_extent.rows - 2, original_extent.cols - 3});

    resize_and_validate(MatrixExtent {0, 0});
    resize_and_validate(MatrixExtent {20, 20});
    resize_and_validate(MatrixExtent {30, 10});
    resize_and_validate(MatrixExtent {10, 30});

    EXPECT_ANY_THROW(resize_tile_layer(mRegistry, layer_id, MatrixExtent {-1, 0}));
    EXPECT_ANY_THROW(resize_tile_layer(mRegistry, layer_id, MatrixExtent {0, -1}));
  }
};

INSTANTIATE_TEST_SUITE_P(DenseAndSparse, TileLayerTest, testing::Bool());

// tactile::is_tile_layer
TEST_P(TileLayerTest, IsTileLayer)
{
  EXPECT_FALSE(is_tile_layer(mRegistry, kInvalidEntity));
  EXPECT_FALSE(is_tile_layer(mRegistry, mRegistry.make_entity()));

  const auto layer_id = make_test_layer();
  EXPECT_TRUE(is_tile_layer(mRegistry, layer_id));
}

// tactile::make_tile_layer
TEST_P(TileLayerTest, MakeTileLayer)
{
  constexpr MatrixExtent extent {5, 6};
  const auto layer_id = make_test_layer(extent);

  EXPECT_TRUE(is_layer(mRegistry, layer_id));
  EXPECT_TRUE(is_tile_layer(mRegistry, layer_id));
  EXPECT_TRUE(mRegistry.has<CMeta>(layer_id));
  EXPECT_TRUE(mRegistry.has<CLayer>(layer_id));
  EXPECT_TRUE(mRegistry.has<CTileLayer>(layer_id));

  if (mTestingDenseLayer) {
    EXPECT_TRUE(mRegistry.has<CDenseTileLayer>(layer_id));
  }
  else {
    EXPECT_TRUE(mRegistry.has<CSparseTileLayer>(layer_id));
  }

  const auto& meta = mRegistry.get<CMeta>(layer_id);
  const auto& layer = mRegistry.get<CLayer>(layer_id);

  EXPECT_EQ(meta.name, "");
  EXPECT_EQ(meta.properties.size(), 0);
  EXPECT_EQ(meta.components.size(), 0);

  EXPECT_EQ(layer.persistent_id, std::nullopt);
  EXPECT_EQ(layer.opacity, 1.0f);
  EXPECT_TRUE(layer.visible);

  each_layer_tile(mRegistry, layer_id, [](const MatrixIndex& index, const TileID tile_id) {
    EXPECT_EQ(tile_id, kEmptyTile);
  });
}

// tactile::destroy_tile_layer
TEST_P(TileLayerTest, DestroyTileLayer)
{
  const auto layer_id = make_test_layer();

  EXPECT_TRUE(mRegistry.is_valid(layer_id));
  EXPECT_EQ(mRegistry.count<CMeta>(), 1);
  EXPECT_EQ(mRegistry.count<CLayer>(), 1);
  EXPECT_EQ(mRegistry.count<CTileLayer>(), 1);
  EXPECT_EQ(mRegistry.count<CDenseTileLayer>(), mTestingDenseLayer ? 1 : 0);
  EXPECT_EQ(mRegistry.count<CSparseTileLayer>(), mTestingDenseLayer ? 0 : 1);
  EXPECT_EQ(mRegistry.count(), 4);

  destroy_tile_layer(mRegistry, layer_id);

  EXPECT_FALSE(mRegistry.is_valid(layer_id));
  EXPECT_EQ(mRegistry.count<CMeta>(), 0);
  EXPECT_EQ(mRegistry.count<CLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CDenseTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CSparseTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count(), 0);
}

// tactile::convert_to_dense_tile_layer
// tactile::convert_to_sparse_tile_layer
TEST_P(TileLayerTest, DenseSparseConversion)
{
  const auto layer_id = make_test_layer(MatrixExtent {6, 4});

  convert_to_dense_tile_layer(mRegistry, layer_id);

  ASSERT_TRUE(mRegistry.has<CDenseTileLayer>(layer_id));
  ASSERT_FALSE(mRegistry.has<CSparseTileLayer>(layer_id));

  {
    auto& dense = mRegistry.get<CDenseTileLayer>(layer_id);
    dense.tiles.at(0).at(0) = TileID {42};
    dense.tiles.at(0).at(1) = TileID {73};
    dense.tiles.at(3).at(2) = TileID {99};
    dense.tiles.at(5).at(3) = TileID {36};
  }

  convert_to_sparse_tile_layer(mRegistry, layer_id);

  ASSERT_TRUE(mRegistry.has<CSparseTileLayer>(layer_id));
  ASSERT_FALSE(mRegistry.has<CDenseTileLayer>(layer_id));

  {
    const auto& sparse = mRegistry.get<CSparseTileLayer>(layer_id);
    EXPECT_EQ(sparse.tiles.size(), 4);
    EXPECT_EQ(sparse.tiles.at(MatrixIndex {0, 0}), TileID {42});
    EXPECT_EQ(sparse.tiles.at(MatrixIndex {0, 1}), TileID {73});
    EXPECT_EQ(sparse.tiles.at(MatrixIndex {3, 2}), TileID {99});
    EXPECT_EQ(sparse.tiles.at(MatrixIndex {5, 3}), TileID {36});
  }

  convert_to_dense_tile_layer(mRegistry, layer_id);

  ASSERT_TRUE(mRegistry.has<CDenseTileLayer>(layer_id));
  ASSERT_FALSE(mRegistry.has<CSparseTileLayer>(layer_id));

  {
    const auto& dense = mRegistry.get<CDenseTileLayer>(layer_id);
    EXPECT_EQ(dense.tiles.at(0).at(0), TileID {42});
    EXPECT_EQ(dense.tiles.at(0).at(1), TileID {73});
    EXPECT_EQ(dense.tiles.at(3).at(2), TileID {99});
    EXPECT_EQ(dense.tiles.at(5).at(3), TileID {36});
  }
}

// tactile::resize_tile_layer
TEST_P(TileLayerTest, ResizeTileLayer)
{
  constexpr MatrixExtent extent {5, 5};
  const auto layer_id = make_test_layer(extent);
  test_resize(layer_id, extent);
}

// tactile::serialize_tile_layer
TEST_P(TileLayerTest, SerializeTileLayer)
{
  constexpr MatrixExtent extent {3, 3};
  const auto layer_id = make_test_layer(extent);

  convert_to_dense_tile_layer(mRegistry, layer_id);

  {
    auto& dense = mRegistry.get<CDenseTileLayer>(layer_id);
    dense.tiles[0][0] = TileID {11};
    dense.tiles[0][1] = TileID {12};
    dense.tiles[0][2] = TileID {13};
    dense.tiles[1][0] = TileID {21};
    dense.tiles[1][1] = TileID {22};
    dense.tiles[1][2] = TileID {23};
    dense.tiles[2][0] = TileID {31};
    dense.tiles[2][1] = TileID {32};
    dense.tiles[2][2] = TileID {33};
  }

  if (!mTestingDenseLayer) {
    convert_to_sparse_tile_layer(mRegistry, layer_id);
  }

  const auto serialized_tiles = serialize_tile_layer(mRegistry, layer_id);
  const auto deserialized_tiles =
      parse_raw_tile_matrix(serialized_tiles, extent, TileIdFormat::kTactile);

  ASSERT_TRUE(deserialized_tiles.has_value());

  convert_to_dense_tile_layer(mRegistry, layer_id);

  const auto& dense = mRegistry.get<CDenseTileLayer>(layer_id);
  EXPECT_THAT(dense.tiles, testing::ContainerEq(*deserialized_tiles));
}

// tactile::set_layer_tile
// tactile::get_layer_tile
TEST_P(TileLayerTest, SetLayerTile)
{
  constexpr MatrixExtent extent {10, 10};
  const auto layer_id = make_test_layer(extent);

  EXPECT_NO_THROW(set_layer_tile(mRegistry, layer_id, MatrixIndex {-1, 0}, TileID {99}));
  EXPECT_NO_THROW(set_layer_tile(mRegistry, layer_id, MatrixIndex {0, -1}, TileID {99}));
  EXPECT_NO_THROW(
      set_layer_tile(mRegistry, layer_id, MatrixIndex {extent.rows, 0}, TileID {99}));
  EXPECT_NO_THROW(
      set_layer_tile(mRegistry, layer_id, MatrixIndex {0, extent.cols}, TileID {99}));

  auto set_and_verify = [this, layer_id](const MatrixIndex& index, const TileID tile_id) {
    set_layer_tile(mRegistry, layer_id, index, tile_id);
    EXPECT_EQ(get_layer_tile(mRegistry, layer_id, index), tile_id);
  };

  set_and_verify(MatrixIndex {0, 0}, TileID {1});
  set_and_verify(MatrixIndex {0, extent.cols - 1}, TileID {2});
  set_and_verify(MatrixIndex {extent.rows - 1, 0}, TileID {3});
  set_and_verify(MatrixIndex {extent.rows - 1, extent.cols - 1}, TileID {4});

  set_and_verify(MatrixIndex {3, 6}, TileID {184});
  set_and_verify(MatrixIndex {9, 4}, TileID {865});
}

}  // namespace tactile
